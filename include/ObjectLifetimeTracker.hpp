#pragma once
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <atomic>
#include <DynamicOutput/Output.hpp>
#include <Unreal/FWeakObjectPtr.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/AGameModeBase.hpp>

#include "UClass.hpp"

namespace RC::Unreal {
    class UObject;
    class AActor;
    class AGameModeBase;
}

/// Give a UObject a serial number if it does not have one, so that an
/// FWeakObjectPtr captured from it can actually resolve.
///
/// UE4SS's FWeakObjectPtr::operator= is NOT the engine's:
///
///     engine   ObjectSerialNumber = GUObjectArray.AllocateSerialNumber(Index);
///     UE4SS    ObjectSerialNumber = IndexToObject(Index)->GetSerialNumber();
///
/// The engine's ALLOCATES lazily. UE4SS's only READS, and the field is zeroed
/// by FUObjectArray::AllocateUObjectIndex and stays zero until something takes
/// a weak pointer -- while Get() early-returns null for any handle whose stored
/// serial is zero. So a handle captured from an object nothing has
/// weak-referenced yet NEVER resolves, and IsActorAlive answers "dead" for it
/// forever.
///
/// That is not a corner case here: TrackSpecificObject exists precisely for
/// objects a caller means to hold, and it is usually called the moment the
/// object appears. Without this, every explicitly tracked object reads dead
/// forever, silently breaking any logic downstream that depends on it
/// actually being alive.
///
/// So do what the engine does: stamp a serial when the slot has none, with the
/// same compare-from-zero, so a concurrent engine allocation wins and we adopt
/// its number rather than overwriting it. The values come from a private high
/// range rather than FUObjectArray::MasterSerialNumber -- that counter is one
/// more offset to be wrong about, and the only property a serial needs is that
/// a recycled slot never sees its previous value, which FreeUObjectIndex
/// guarantees by zeroing the field.
inline void EnsureUObjectSerialNumber(const RC::Unreal::UObjectBase* object) {
    using namespace RC::Unreal;
    if (!object) return;

    auto* obj = const_cast<UObject*>(std::bit_cast<const UObject*>(object));
    const int32_t index = obj->GetInternalIndex();
    if (index < 0) return;
    FUObjectItem* item = FUObjectArray::IndexToObject(index);
    if (!item) return;

    int32_t* slot = &item->GetSerialNumber();
    if (*slot != 0) return; // the engine already allocated one

    // High enough that the engine's counter -- which starts at 1000 and
    // increments once per first-ever weak reference -- cannot reach us.
    static std::atomic<int32_t> next{0x40000000};
    int32_t expected = 0;
    std::atomic_ref<int32_t>(*slot).compare_exchange_strong(
        expected, next.fetch_add(1, std::memory_order_relaxed));
}

/// ObjectLifetimeTracker provides RAII-style lifetime tracking for UObjects with additional state monitoring
/// 
/// By default tracks:
/// - Objects derived from AGameModeBase
/// - Objects with "mainPlayer" in their name
///
/// Additional objects can be tracked by:
/// - Registering specific UClass types
/// - Registering name patterns to match against
///
/// The tracker maintains detailed object state including:
/// - Validity status
/// - Object name
/// - Memory address
/// - Object flags
///
/// WHAT IT CANNOT TELL YOU. Everything here is keyed on the object's ADDRESS,
/// and addresses are reused. "Is something alive at X" and "is X still the
/// object I stored" are different questions, and only the first one is
/// answerable from a map like this one. A caller that keeps a UObject* across
/// frames needs an FWeakObjectPtr captured alongside it; see IsActorAlive.
///
/// Example usage:
/// @code
/// auto& tracker = ObjectLifetimeTracker::Get();
/// 
/// // Track all Car objects
/// tracker.RegisterTrackedType(Car::StaticClass());
/// 
/// // Track objects with specific names
/// tracker.RegisterTrackedName(L"specialThing");
///
/// // Check if object is still valid
/// if (tracker.IsActorAlive(someObject)) {
///     // Safe to use object
/// }
/// @endcode
class ObjectLifetimeTracker {
public:
    /// Stores detailed information about a tracked object
    struct ObjectInfo {
        bool isValid{false};           ///< Whether the object is currently valid
        std::wstring name;            ///< The object's name
        uintptr_t address{0};         ///< Memory address of the object
        RC::Unreal::EObjectFlags flags{}; ///< Current object flags
        /// Set by TrackSpecificObject, which is the only context where taking
        /// one is safe: the caller is on the game thread and holds a pointer it
        /// knows is live. The create listener deliberately does NOT take one --
        /// it fires from inside a GUObjectArray notification, and allocating a
        /// serial number there is GUObjectArray work under GUObjectArray work.
        RC::Unreal::FWeakObjectPtr weak{};
        bool hasWeak{false};
    };

    /// Get the singleton instance of ObjectLifetimeTracker
    static ObjectLifetimeTracker& Get() {
        static ObjectLifetimeTracker instance;
        return instance;
    }


    /// Is there a live object AT THIS ADDRESS?
    ///
    /// Read that sentence literally, because it is not the question most
    /// callers mean. This class is keyed on the address, and the create
    /// listener marks every newly constructed UObject valid -- so once an actor
    /// dies and something else is allocated into its slot, this returns true
    /// again for a pointer whose original object is long gone. It is answering
    /// honestly; the pointer is just no longer evidence of what the caller
    /// thinks it is.
    ///
    /// If you are holding a UObject* across frames, this is NOT enough. Pair
    /// the pointer with an FWeakObjectPtr captured when you stored it, and
    /// compare against it before trusting the raw pointer again.
    ///
    /// @param actor The UObject to check
    /// @return true if an object is live at this address
    bool IsActorAlive(const RC::Unreal::UObjectBase* actor) {
        if (!actor) return false;

        // NOTHING that touches the UObject may run under objectsLock. The
        // deletion listener blocks on this lock, and it is called from
        // GUObjectArray during purge while that holds its own internal lock;
        // GetName walks an outer chain and can reach back into GUObjectArray,
        // which would be a lock-order inversion and a hang at GC time.
        RC::Unreal::FWeakObjectPtr weak{};
        bool hasWeak = false;
        {
            std::lock_guard lock(objectsLock);
            auto it = liveObjects.find(actor);
            if (it == liveObjects.end()) return false;
            if (!it->second.isValid) return false;
            weak = it->second.weak;
            hasWeak = it->second.hasWeak;
        }

        // `actor` itself must never be dereferenced here. Between the map
        // lookup and any read of the object, GC on another thread can free
        // it, so touching it here would fault on exactly the input this
        // check exists to reject.
        //
        // FWeakObjectPtr::Get() resolves through GUObjectArray by index and
        // rejects PendingKill and Unreachable without touching object memory
        // at all, so it is both safer and stronger than a flags check.
        if (hasWeak) {
            return static_cast<const void*>(weak.Get()) == static_cast<const void*>(actor);
        }

        // Create-listener-only entry: nobody ever asked us to hold this one, so
        // there is no handle to resolve. The map is the whole answer, which is
        // what it was for these objects before as well -- the RF_BeginDestroyed
        // branch never ran on them either, because their flags were never
        // populated.
        return true;
    }

    /// How often a create/delete notification had to WAIT for the lock instead
    /// of taking it uncontended. Both listeners block rather than give up,
    /// because a dropped deletion would leave an entry reading isValid==true
    /// forever, and IsActorAlive would keep vouching for freed memory.
    /// Nonzero counts are expected under GC load, not a sign of a bug.
    struct Contention {
        unsigned long long creates{0};
        unsigned long long deletes{0};
    };
    Contention GetContention() const {
        return {contendedCreates.load(std::memory_order_relaxed),
                contendedDeletes.load(std::memory_order_relaxed)};
    }

    /// Register a UClass to be tracked by the lifetime system
    /// All objects of this class or its child classes will be tracked
    /// @param classToTrack The UClass to track (e.g. Car::StaticClass())
    void RegisterTrackedType(RC::Unreal::UClass* classToTrack) {
        if (!classToTrack) {
            RC::Output::send<RC::LogLevel::Warning>(STR("Attempted to register null class type\n"));
            return;
        }
        std::lock_guard lock(objectsLock);
        trackedTypes.insert(classToTrack);
        RC::Output::send<RC::LogLevel::Verbose>(STR("Registered tracked type: {}\n"), 
            classToTrack->GetName().c_str());
    }

    /// Register a name pattern to track objects by
    /// Any object whose name contains this string will be tracked
    /// @param nameToTrack Wide string to match against object names
    void RegisterTrackedName(const std::wstring& nameToTrack) {
        if (nameToTrack.empty()) {
            RC::Output::send<RC::LogLevel::Warning>(STR("Attempted to register empty name pattern\n"));
            return;
        }
        std::lock_guard lock(objectsLock);
        trackedNames.insert(nameToTrack);
        RC::Output::send<RC::LogLevel::Verbose>(STR("Registered tracked name pattern: {}\n"), 
            nameToTrack.c_str());
    }

    /// Remove a UClass from the tracking system
    /// @param classToTrack The UClass to stop tracking
    /// @note Existing tracked objects will remain tracked until deletion
    void UnregisterTrackedType(RC::Unreal::UClass* classToTrack) {
        std::lock_guard lock(objectsLock);
        trackedTypes.erase(classToTrack);
    }

    /// Remove a name pattern from the tracking system
    /// @param nameToTrack The name pattern to stop tracking
    /// @note Existing tracked objects will remain tracked until deletion
    void UnregisterTrackedName(const std::wstring& nameToTrack) {
        std::lock_guard lock(objectsLock);
        trackedNames.erase(nameToTrack);
    }

    /// Clear all tracking data and patterns
    /// Removes all tracked objects, types, and name patterns
    void ClearAllTracking() {
        std::lock_guard lock(objectsLock);
        liveObjects.clear();
        trackedTypes.clear();
        trackedNames.clear();
        RC::Output::send<RC::LogLevel::Verbose>(STR("Cleared all object tracking\n"));
    }

    /// Explicitly track a specific UObject instance
    ///
    /// This is where the weak handle comes from, so it is what makes
    /// IsActorAlive meaningful for this object: the caller is on the game
    /// thread and holds a pointer it knows is live, which is the only context
    /// in which capturing one is safe.
    ///
    /// @param object The UObject pointer to track
    /// @return true if the object was successfully added to tracking
    bool TrackSpecificObject(const RC::Unreal::UObjectBase* object) {
        if (!object) {
            return false;
        }

        // Outside the lock, because both of these walk GUObjectArray and
        // objectsLock is held by the delete listener while GUObjectArray
        // purges. Same lock-order rule as IsActorAlive.
        RC::Unreal::FWeakObjectPtr weak{};
        bool hasWeak = false;
        try {
            // Must come FIRST. Without it the handle below is captured with
            // serial 0 and can never resolve, which makes IsActorAlive answer
            // "dead" for exactly the objects a caller cared enough to track.
            EnsureUObjectSerialNumber(object);
            weak = std::bit_cast<RC::Unreal::UObject*>(object);
            hasWeak = true;
            // Deliberately NOT verified against a resolve here. Once the serial
            // is stamped, a handle that fails to resolve is the engine saying
            // the object is already PendingKill or Unreachable -- a correct
            // answer, and one we want IsActorAlive to keep giving. Falling back
            // to address-only on that would turn a dying object back into a
            // live one, which is the direction that hurts.
        } catch (...) {
            // Leaves the entry address-only; still usable, just without the
            // stronger resolve check.
        }

        std::lock_guard lock(objectsLock);

        // Not an early return on "already tracking": the create listener has
        // already inserted every object into the map by the time a caller
        // gets here, so bailing out on that check would mean this call never
        // actually attaches a handle. Upgrade the existing entry in place.
        auto& info = liveObjects[object];
        info.isValid = true;
        if (info.name.empty()) info.name = L"pending";
        info.address = reinterpret_cast<uintptr_t>(object);
        if (hasWeak && !info.hasWeak) {
            info.weak = weak;
            info.hasWeak = true;
        }
        return true;
    }

    
    /// Find all tracked objects of a specific UClass type
    /// @param classToFind The UClass to search for
    /// @return Vector of pairs containing the object pointer and its info
    std::vector<std::pair<const RC::Unreal::UObjectBase*, ObjectInfo>> FindObjectsByClass(RC::Unreal::UClass* classToFind) {
        if (!classToFind) {
            RC::Output::send<RC::LogLevel::Warning>(STR("Attempted to search with null class type\n"));
            return {};
        }

        std::vector<std::pair<const RC::Unreal::UObjectBase*, ObjectInfo>> results;
        std::lock_guard lock(objectsLock);

        for (const auto& [obj, info] : liveObjects) {
            auto uobject = std::bit_cast<RC::Unreal::UObject*>(obj);
            if (uobject && uobject->IsA(classToFind)) {
                results.emplace_back(obj, info);
            }
        }

        return results;
    }

    /// Find all tracked objects whose names contain the specified string
    /// @param namePattern The string to search for in object names
    /// @param caseSensitive Whether to perform case-sensitive matching
    /// @return Vector of pairs containing the object pointer and its info
    std::vector<std::pair<const RC::Unreal::UObjectBase*, ObjectInfo>> FindObjectsByName(
        const std::wstring& namePattern, 
        bool caseSensitive = true
    ) {
        if (namePattern.empty()) {
            RC::Output::send<RC::LogLevel::Warning>(STR("Attempted to search with empty name pattern\n"));
            return {};
        }

        std::vector<std::pair<const RC::Unreal::UObjectBase*, ObjectInfo>> results;
        std::lock_guard lock(objectsLock);

        for (const auto& [obj, info] : liveObjects) {
            bool matches;
            if (caseSensitive) {
                matches = info.name.find(namePattern) != std::wstring::npos;
            } else {
                // Convert both strings to lowercase for case-insensitive comparison
                std::wstring lowerName = info.name;
                std::wstring lowerPattern = namePattern;
                std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::towlower);
                std::transform(lowerPattern.begin(), lowerPattern.end(), lowerPattern.begin(), ::towlower);
                matches = lowerName.find(lowerPattern) != std::wstring::npos;
            }

            if (matches) {
                results.emplace_back(obj, info);
            }
        }

        return results;
    }

private:
    std::unordered_map<const RC::Unreal::UObjectBase*, ObjectInfo> liveObjects;
    std::unordered_set<RC::Unreal::UClass*> trackedTypes;
    std::unordered_set<std::wstring> trackedNames;
    std::mutex objectsLock;
    std::atomic<unsigned long long> contendedCreates{0};
    std::atomic<unsigned long long> contendedDeletes{0};

    /// Take objectsLock, counting the case where it was already held.
    /// Deliberately blocking: see Contention. The lock is never held across
    /// anything that can create or destroy a UObject, so this cannot deadlock
    /// against the notification it is protecting.
    template <typename Counter>
    static std::unique_lock<std::mutex> LockCounting(std::mutex& m, Counter& counter) {
        std::unique_lock<std::mutex> lock(m, std::try_to_lock);
        if (!lock.owns_lock()) {
            counter.fetch_add(1, std::memory_order_relaxed);
            lock.lock();
        }
        return lock;
    }

    /// Constructor sets up object creation and deletion listeners
    ObjectLifetimeTracker() {
        RC::Unreal::UObjectArray::AddUObjectCreateListener(&createListener);
        RC::Unreal::UObjectArray::AddUObjectDeleteListener(&deleteListener);
    }

    /// Destructor removes listeners
    ~ObjectLifetimeTracker() {
        RC::Unreal::UObjectArray::RemoveUObjectCreateListener(&createListener);
        RC::Unreal::UObjectArray::RemoveUObjectDeleteListener(&deleteListener);
    }

    /// Determines if an object should be tracked based on type and name rules
    /// @param uobject The object to evaluate
    /// @return true if the object matches tracking criteria
    bool ShouldTrackObject(RC::Unreal::UObject* uobject) {
        if (!uobject) return false;

        // Check if it's a GameMode
        if (uobject->IsA(RC::Unreal::AGameModeBase::StaticClass())) {
            return true;
        }

        // Check custom tracked types
        for (auto trackedType : trackedTypes) {
            if (uobject->IsA(trackedType)) {
                return true;
            }
        }

        // Check name patterns
        std::wstring objName = uobject->GetName();
        
        // Always track mainPlayer
        if (objName.find(L"mainPlayer") != std::wstring::npos) {
            return true;
        }

        // Check custom name patterns
        for (const auto& trackedName : trackedNames) {
            if (objName.find(trackedName) != std::wstring::npos) {
                return true;
            }
        }

        return false;
    }

    /// Listener for object creation events
    struct CreateListener : RC::Unreal::FUObjectCreateListener {
        /// Called when a new UObject is created
        void NotifyUObjectCreated(const RC::Unreal::UObjectBase* Object, RC::Unreal::int32 Index) override {
            if (!Object) return;
            
            try {
                auto& tracker = Get();
                // Blocking, for symmetry with the deletion listener: a dropped
                // creation at an address a dropped deletion also missed is the
                // combination that resurrects a freed pointer.
                auto lock = LockCounting(tracker.objectsLock, tracker.contendedCreates);

                // MINIMAL tracking - don't call any UObject methods yet
                // (the object is not fully constructed here, which is why
                // ShouldTrackObject cannot be consulted from this callback).
                ObjectInfo info;
                info.isValid = true;
                info.name = L"pending"; 
                info.address = reinterpret_cast<uintptr_t>(Object);
                info.flags = {};       
                
                tracker.liveObjects[Object] = info;
                
            } catch (...) {
                
            }
        }

        void OnUObjectArrayShutdown() override {
            RC::Unreal::UObjectArray::RemoveUObjectCreateListener(this);
        }
    };

    /// Listener for object deletion events  
    struct DeleteListener : RC::Unreal::FUObjectDeleteListener {
        void NotifyUObjectDeleted(const RC::Unreal::UObjectBase* Object, RC::Unreal::int32 Index) override {
            if (!Object) return;
            
            try {
                auto& tracker = Get();

                // NEVER give up on a deletion. A dropped one is the single
                // failure this class cannot absorb: the entry survives with
                // isValid==true, IsActorAlive keeps vouching for the address,
                // and a caller ends up dereferencing freed memory. Deletions
                // arrive on FAsyncPurge while the game thread can simultaneously
                // be inside IsActorAlive on the same lock, so this is a race
                // that happens under normal GC load, not an edge case.
                auto lock = LockCounting(tracker.objectsLock, tracker.contendedDeletes);

                tracker.liveObjects.erase(Object);
                
            } catch (...) {

            }
        }

        void OnUObjectArrayShutdown() override {
            RC::Unreal::UObjectArray::RemoveUObjectDeleteListener(this);
        }
    };

    static inline CreateListener createListener{};
    static inline DeleteListener deleteListener{};

    // Prevent copies
    ObjectLifetimeTracker(const ObjectLifetimeTracker&) = delete;
    ObjectLifetimeTracker& operator=(const ObjectLifetimeTracker&) = delete;
};