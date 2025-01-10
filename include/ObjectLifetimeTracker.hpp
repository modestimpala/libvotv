#pragma once
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <DynamicOutput/Output.hpp>
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
    };

    /// Get the singleton instance of ObjectLifetimeTracker
    static ObjectLifetimeTracker& Get() {
        static ObjectLifetimeTracker instance;
        return instance;
    }


    /// Check if a UObject pointer is still valid and not pending destruction
    /// @param actor The UObject to check
    /// @return true if the object is being tracked, valid, and not marked for destruction
    bool IsActorAlive(const RC::Unreal::UObjectBase* actor) {
        if (!actor) return false;
        
        std::lock_guard lock(objectsLock);
        auto it = liveObjects.find(actor);
        if (it == liveObjects.end()) return false;

        // Cast to UObject to check flags
        auto uobject = std::bit_cast<RC::Unreal::UObject*>(actor);
        if (!uobject) return false;

        // Check for pending kill or invalid flags
        auto flags = uobject->GetObjectFlags();
        if (flags & RC::Unreal::EObjectFlags::RF_BeginDestroyed) {
            // Object is being destroyed, remove from tracking
            liveObjects.erase(it);
            return false;
        }

        return it->second.isValid;
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

private:
    std::unordered_map<const RC::Unreal::UObjectBase*, ObjectInfo> liveObjects;
    std::unordered_set<RC::Unreal::UClass*> trackedTypes;
    std::unordered_set<std::wstring> trackedNames;
    std::mutex objectsLock;

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
        /// @param Object The newly created object
        /// @param Index The object's index in the UObject array
        void NotifyUObjectCreated(const RC::Unreal::UObjectBase* Object, RC::Unreal::int32 Index) override {
            auto& tracker = Get();
            auto uobject = std::bit_cast<RC::Unreal::UObject*>(Object);
            if (!uobject) return;

            std::lock_guard lock(tracker.objectsLock);
            
            if (tracker.ShouldTrackObject(uobject)) {
                ObjectInfo info;
                info.isValid = true;
                info.name = uobject->GetName();
                info.address = reinterpret_cast<uintptr_t>(Object);
                info.flags = uobject->GetObjectFlags();

                tracker.liveObjects[Object] = info;
            }
        }

        /// Called when the UObject array is being shut down
        void OnUObjectArrayShutdown() override {
            RC::Unreal::UObjectArray::RemoveUObjectCreateListener(this);
        }
    };

    /// Listener for object deletion events
    struct DeleteListener : RC::Unreal::FUObjectDeleteListener {
        /// Called when a UObject is being deleted
        /// @param Object The object being deleted
        /// @param Index The object's index in the UObject array
        void NotifyUObjectDeleted(const RC::Unreal::UObjectBase* Object, RC::Unreal::int32 Index) override {
            auto& tracker = Get();
            std::lock_guard lock(tracker.objectsLock);
            
            auto it = tracker.liveObjects.find(Object);
            if (it != tracker.liveObjects.end()) {
                tracker.liveObjects.erase(it);
            }
        }

        /// Called when the UObject array is being shut down
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