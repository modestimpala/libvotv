// ObjectLifetimeTracker.hpp
#pragma once
#include <unordered_map>
#include <mutex>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/Common.hpp>
#include <Unreal/AGameModeBase.hpp>

namespace RC::Unreal {
    class UObject;
    class AActor;
    class AGameModeBase;
}

/// ObjectLifetimeTracker provides RAII-style lifetime tracking for UObjects
/// 
/// By default tracks:
/// - Objects derived from AGameModeBase
/// - Objects with "mainPlayer" in their name
///
/// Additional objects can be tracked by:
/// - Registering specific UClass types
/// - Registering name patterns to match against
///
/// Example usage:
/// @code
/// auto& tracker = ObjectLifetimeTracker::Get();
/// 
///  Track all Car objects
/// tracker.RegisterTrackedType(Car::StaticClass());
/// 
///  Track objects with specific names
/// tracker.RegisterTrackedName(L"specialThing");
///
///  Check if object is still valid
/// if (tracker.IsActorAlive(someObject)) {
///      -- Safe to use object
/// }
/// @endcode
class ObjectLifetimeTracker {
public:
    /// Get the singleton instance of ObjectLifetimeTracker
    static ObjectLifetimeTracker& Get() {
        static ObjectLifetimeTracker instance;
        return instance;
    }

    /// Check if a UObject pointer is still valid
    /// @param actor The UObject to check
    /// @return true if the object is being tracked and hasn't been deleted
    bool IsActorAlive(const RC::Unreal::UObjectBase* actor) {
        if (!actor) return false;
        std::lock_guard<std::mutex> lock(objectsLock);
        return liveObjects.contains(actor);
    }

    /// Register a UClass to be tracked by the lifetime system
    /// All objects of this class or its child classes will be tracked
    /// @param classToTrack The UClass to track (e.g. Car::StaticClass())
    void RegisterTrackedType(RC::Unreal::UClass* classToTrack) {
        std::lock_guard<std::mutex> lock(objectsLock);
        trackedTypes.insert(classToTrack);
    }

    /// Remove a UClass from the tracking system
    /// @param classToTrack The UClass to stop tracking
    /// @note Existing tracked objects will remain tracked until deletion
    void UnregisterTrackedType(RC::Unreal::UClass* classToTrack) {
        std::lock_guard<std::mutex> lock(objectsLock);
        trackedTypes.erase(classToTrack);
    }

    /// Register a name pattern to track objects by
    /// Any object whose name contains this string will be tracked
    /// @param nameToTrack Wide string to match against object names
    void RegisterTrackedName(const std::wstring& nameToTrack) {
        std::lock_guard<std::mutex> lock(objectsLock);
        trackedNames.insert(nameToTrack);
    }

    /// Remove a name pattern from the tracking system
    /// @param nameToTrack The name pattern to stop tracking
    /// @note Existing tracked objects will remain tracked until deletion
    void UnregisterTrackedName(const std::wstring& nameToTrack) {
        std::lock_guard<std::mutex> lock(objectsLock);
        trackedNames.erase(nameToTrack);
    }
    

private:
    std::unordered_set<RC::Unreal::UClass*> trackedTypes;
    std::unordered_set<std::wstring> trackedNames;

    ObjectLifetimeTracker() {
        RC::Unreal::UObjectArray::AddUObjectCreateListener(&createListener);
        RC::Unreal::UObjectArray::AddUObjectDeleteListener(&deleteListener);
    }
    
    ~ObjectLifetimeTracker() {
        RC::Unreal::UObjectArray::RemoveUObjectCreateListener(&createListener);
        RC::Unreal::UObjectArray::RemoveUObjectDeleteListener(&deleteListener);
    }

    std::unordered_map<const RC::Unreal::UObjectBase*, bool> liveObjects;
    std::mutex objectsLock;

    struct CreateListener : public RC::Unreal::FUObjectCreateListener {
        void NotifyUObjectCreated(const RC::Unreal::UObjectBase* Object, RC::Unreal::int32 Index) override {
            auto& tracker = ObjectLifetimeTracker::Get();
            auto uobject = std::bit_cast<RC::Unreal::UObject*>(Object);

            std::lock_guard<std::mutex> lock(tracker.objectsLock);

            // Check default tracked types
            if (uobject->IsA(RC::Unreal::AGameModeBase::StaticClass())) {
                tracker.liveObjects[Object] = true;
                return;
            }

            // Check custom tracked types
            for (auto trackedType : tracker.trackedTypes) {
                if (uobject->IsA(trackedType)) {
                    tracker.liveObjects[Object] = true;
                    return;
                }
            }

            // Check name matches (including default mainPlayer)
            std::wstring objName = uobject->GetName();
            if (objName.find(L"mainPlayer") != std::wstring::npos) {
                tracker.liveObjects[Object] = true;
                return;
            }

            for (const auto& trackedName : tracker.trackedNames) {
                if (objName.find(trackedName) != std::wstring::npos) {
                    tracker.liveObjects[Object] = true;
                    return;
                }
            }
        }


        void OnUObjectArrayShutdown() override {
            RC::Unreal::UObjectArray::RemoveUObjectCreateListener(this);
        }
    };

    struct DeleteListener : public RC::Unreal::FUObjectDeleteListener {
        void NotifyUObjectDeleted(const RC::Unreal::UObjectBase* Object, RC::Unreal::int32 Index) override {
            auto& tracker = ObjectLifetimeTracker::Get();
            std::lock_guard<std::mutex> lock(tracker.objectsLock);
            tracker.liveObjects.erase(Object);
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

// Credit: UE4SS Discord
// Credit: ModestImpala