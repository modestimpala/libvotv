#pragma once
#include "game.hpp"
#include "engine/include/CharacterMovement.hpp"


namespace votv::game
{
    
// Main Player class
class MainPlayer : public RC::Unreal::AActor {
public:
    UE4SS_FIELD(bool, underwater);
    UE4SS_FIELD(bool, inWater);
    UE4SS_FIELD(bool, noclip);
    UE4SS_FIELD(bool, crouching);
    UE4SS_FIELD(bool, dead);
    UE4SS_FIELD(float, air);
    UE4SS_FIELD(float, foodDrain);
    UE4SS_FIELD(float, sleepDrain);
    UE4SS_FIELD(bool, mirror);
    UE4SS_FIELD(bool, combat);
    UE4SS_FIELD(bool, hasGlasses);
    UE4SS_FIELD(bool, hasFlashlight);
    UE4SS_FIELD(GameMode*, gameMode);
    UE4SS_FIELD(AActor*, pickup_actor);
    UE4SS_FIELD(AActor*, holdObject);
    UE4SS_FIELD(Hook*, activeHook);
    UE4SS_FIELD(AActor*, droppedItem);
    UE4SS_FIELD(AActor*, sittingOn);
    UE4SS_FIELD(UObject*, sitOnComponent);
    UE4SS_FIELD(UObject*, sitPlace);
    UE4SS_FIELD(AActor*, lookAtActor);

    UE4SS_FIELD(float, animTimer);
    UE4SS_FIELD(float, defSpeed);
    UE4SS_FIELD(bool, input_run);
    UE4SS_FIELD(bool, input_alt);
    UE4SS_FIELD(float, armLength);
    UE4SS_FIELD(bool, flashlight);
    UE4SS_FIELD(bool, isLookAt);
    UE4SS_FIELD(float, mouseSens);
    UE4SS_FIELD(float, inpF_forward);
    UE4SS_FIELD(float, inpF_back);
    UE4SS_FIELD(float, inpF_rig);
    UE4SS_FIELD(float, inpF_lef);
    UE4SS_FIELD(bool, input_rotate);
    UE4SS_FIELD(float, camSpeed);
    UE4SS_FIELD(bool, grabsHeavy);
    UE4SS_FIELD(bool, s_invertX);
    UE4SS_FIELD(bool, s_invertY);
    UE4SS_FIELD(float, kickTime);
    UE4SS_FIELD(bool, isKicked);
    UE4SS_FIELD(bool, canKick);
    UE4SS_FIELD(bool, isRagdoll);
    UE4SS_FIELD(bool, hulkMode);
    UE4SS_FIELD(bool, arcade);
    UE4SS_FIELD(bool, input_jump);
    UE4SS_FIELD(bool, onWater);
    UE4SS_FIELD(float, reload);
    UE4SS_FIELD(bool, input_fire);
    UE4SS_FIELD(bool, input_crouch);
    UE4SS_FIELD(RC::Unreal::FName, holdName);
    UE4SS_FIELD(WeaponStruct, weapon);
    UE4SS_FIELD(float, burningTime);
    UE4SS_FIELD(bool, deactivateMouseInput);
    UE4SS_FIELD(bool, fallTp);
    UE4SS_FIELD(float, underwaterDepth);
    UE4SS_FIELD(bool, isWakingUp);
    UE4SS_FIELD(bool, skipTolerance);
    UE4SS_FIELD(bool, isInFPBanimation);
    UE4SS_FIELD(float, headbobAnim);
    UE4SS_FIELD(float, headbobStrength);
    UE4SS_FIELD(float, headbobTiltStrength);
    UE4SS_FIELD(bool, noWakeup);
    UE4SS_FIELD(float, sleepDraining);
    UE4SS_FIELD(float, agil);
    UE4SS_FIELD(bool, isMirror);
    UE4SS_FIELD(bool, dropped);
    UE4SS_FIELD(bool, radialMenu);
    UE4SS_FIELD(bool, scubaAir);
    UE4SS_FIELD(bool, lastDroppedItem_deleted);
    UE4SS_FIELD(bool, canGetUp);
    UE4SS_FIELD(float, grab_speed);
    UE4SS_FIELD(bool, AutoRagdollGetup);
    UE4SS_FIELD(bool, calClimb);
    UE4SS_FIELD(bool, deactivateHeightRagdoll);
    UE4SS_FIELD(bool, velmaMode);
    UE4SS_FIELD(bool, crankFlashlight);


    void ignite(float fuel)
    {
        auto function = GetFunctionByName(STR("ignite"));
        if (!function)
        {
            RC::Output::send<RC::LogLevel::Error>(STR("ignite function not found\n"));
            return;
        }
        std::vector<uint8_t> paramBuffer(function->GetParmsSize());
        void* parameters = paramBuffer.data();
        float* parameter1 = function->GetPropertyByName(STR("fuel"))->ContainerPtrToValuePtr<float>(parameters);
        *parameter1 = fuel;
        ProcessEvent(function, parameters);
    }

    // Convenient struct wrapper
    struct PlayerState {
        bool inWater;
        bool underwater;
        bool noclip;
        bool crouching;
        bool dead;
        float air;
        bool wakingUp;
        float foodDrain;
        float sleepDrain;
        bool mirror;
        bool combat;
        bool hasGlasses;
        bool hasFlashlight;
    };

    PlayerState GetState() const {
        return {
            this->inWater,
            this->underwater,
            this->noclip,
            this->crouching,
            this->dead,
            this->air,
            this->isWakingUp,
            this->foodDrain,
            this->sleepDrain,
            this->mirror,
            this->combat,
            this->hasGlasses,
            this->hasFlashlight
        };
    }
    
    auto GetVelocity() -> RC::Unreal::FVector
    {
        UE_BEGIN_NATIVE_FUNCTION_BODY("/Script/Engine.Actor:GetVelocity")
        UE_CALL_FUNCTION()
        UE_RETURN_PROPERTY(RC::Unreal::FVector)
    }

    auto DumpFunctions() -> void
    {
        for (UFunction* Function : GetClassPrivate()->ForEachFunctionInChain())
        {
            Output::send<LogLevel::Verbose>(STR("Function: {}\n"), Function->GetName());
        }
    }

    // These broke but worked before, need to fix
    /*
    auto GetCharacterMovementComponent() -> RC::Unreal::CharacterMovementComponent*
    {
        UStruct* Struct = IsA<UStruct>() ? std::bit_cast<UStruct*>(this) : GetClassPrivate();
        for (FProperty* Property : Struct->ForEachPropertyInChain())
        {
            Output::send<RC::LogLevel::Verbose>(STR("Property: {}\n"), Property->GetName());
        }
        // Get the FProperty for CharacterMovement
        auto CharacterMovementProp = GetPropertyByNameInChain(STR("CharacterMovement"));
        if (!CharacterMovementProp)
        {
            RC::Output::send<RC::LogLevel::Error>(STR("CharacterMovement property not found\n"));
            return nullptr;
        }

        // Use ContainerPtrToValuePtr to get the actual UCharacterMovementComponent pointer
        auto* CharacterMovementPtr = CharacterMovementProp->ContainerPtrToValuePtr<UObject*>(this);
        if (!CharacterMovementPtr || !*CharacterMovementPtr)
        {
            RC::Output::send<RC::LogLevel::Error>(STR("Failed to get CharacterMovement component\n"));
            return nullptr;
        }

        // Dereference the pointer to get the actual UObject
        UObject* CharacterMovement = *CharacterMovementPtr;
        
        return Cast<RC::Unreal::CharacterMovementComponent>(CharacterMovement);
    }

    auto SetVelocity(const RC::Unreal::FVector& NewVel, bool bAddToCurrent) -> void
    {
        UStruct* Struct = IsA<UStruct>() ? std::bit_cast<UStruct*>(this) : GetClassPrivate();
        for (FProperty* Property : Struct->ForEachPropertyInChain())
        {
            Output::send<RC::LogLevel::Verbose>(STR("Property: {}\n"), Property->GetName());
        }
        // Get the FProperty for CharacterMovement
        auto CharacterMovementProp = GetPropertyByNameInChain(STR("CharacterMovement"));
        if (!CharacterMovementProp)
        {
            RC::Output::send<RC::LogLevel::Error>(STR("CharacterMovement property not found\n"));
            return;
        }

        // Use ContainerPtrToValuePtr to get the actual UCharacterMovementComponent pointer
        auto* CharacterMovementPtr = CharacterMovementProp->ContainerPtrToValuePtr<UObject*>(this);
        if (!CharacterMovementPtr || !*CharacterMovementPtr)
        {
            RC::Output::send<RC::LogLevel::Error>(STR("Failed to get CharacterMovement component\n"));
            return;
        }

        // Dereference the pointer to get the actual UObject
        UObject* CharacterMovement = *CharacterMovementPtr;
    
        // Now we can use GetPropertyByName on the dereferenced pointer
        auto VelocityProp = CharacterMovement->GetPropertyByNameInChain(STR("Velocity"));
        if (VelocityProp)
        {
            //get ptr and offset
            auto* VelocityPtr = VelocityProp->ContainerPtrToValuePtr<RC::Unreal::FVector>(CharacterMovement);
            if (VelocityPtr)
            {
                // Set the velocity
                *VelocityPtr = NewVel;
            }
        } else
        {
            RC::Output::send<RC::LogLevel::Error>(STR("Velocity property not found\n"));
        }
    }*/
};
}
