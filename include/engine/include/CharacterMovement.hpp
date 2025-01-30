#pragma once

#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/World.hpp>
#include <Unreal/UScriptStruct.hpp>
#include <Unreal/BPMacros.hpp>

#include "enums/EMovementMode.hpp"

namespace RC::Unreal
{   
    // Forward declarations
    class ACharacter;
    class UPrimitiveComponent;
    class USceneComponent;
    
    /**
     * CharacterMovementComponent handles movement logic for the associated Character owner.
     * Movement is affected by many factors including input, rotation, walk/run speed, etc.
     */
    class CharacterMovementComponent : public UObject 
    {
        DECLARE_EXTERNAL_OBJECT_CLASS(CharacterMovementComponent, Engine)

    public:
        static std::unordered_map<File::StringType, uint32_t> VTableLayoutMap;

    public:
        using BeginPlaySignature = void(CharacterMovementComponent* Context);
        static Function<BeginPlaySignature> BeginPlayInternal;
        
        using TickSignature = void(CharacterMovementComponent* Context, float DeltaSeconds);
        static Function<TickSignature> TickInternal;

    public:
        // Core Character Movement Properties
        
        /** Character movement component belongs to */
        ACharacter* CharacterOwner{};

        EMovementMode MovementMode();
        
        /** Scale of gravity for this component */
        float GravityScale{};
        
        /** Maximum height character can step up */
        float MaxStepHeight{};
        
        /** Initial velocity when jumping */
        float JumpZVelocity{};
        
        /** Fraction of JumpZVelocity to use when automatically "jumping off" of a base actor that's not allowed to be a base for a character */
        float JumpOffJumpZFactor{};
        
        /** Angle in degrees of max walkable surface */
        float WalkableFloorAngle{};
        
        /** Z component of normal of steepest walkable surface, computed from WalkableFloorAngle */
        float WalkableFloorZ{};

    public:
        // UFunction interfaces -> START
        
        /** Sets the z component of the walkable floor normal */
        auto SetWalkableFloorZ(float InWalkableFloorZ) -> void;
        
        /** Sets the walkable floor angle */
        auto SetWalkableFloorAngle(float InWalkableFloorAngle) -> void;
        
        /** Updates current movement mode */
        auto SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) -> void;
        
        /** Sets groups to ignore mask for avoidance */
        //auto SetGroupsToIgnoreMask(FNavAvoidanceMask GroupMask) -> void;
        
        /** Sets groups to ignore */
        auto SetGroupsToIgnore(int32 GroupFlags) -> void;
        
        /** Gets walkable floor Z */
        auto K2_GetWalkableFloorZ() -> float;
        
        /** Gets walkable floor angle */
        auto K2_GetWalkableFloorAngle() -> float;
        
        /** Gets modified max acceleration */
        auto K2_GetModifiedMaxAcceleration() -> float;
        
        /** Checks if currently walking */
        auto IsWalking() -> bool;
        
        /** Gets valid perch radius */
        auto GetValidPerchRadius() -> float;
        
        /** Gets movement base */
        auto GetMovementBase() -> UPrimitiveComponent*;
        
        /** Gets character owner */
        auto GetCharacterOwner() -> ACharacter*;
        
        /** Disables movement */
        auto DisableMovement() -> void;
        
        /** Clears accumulated forces */
        auto ClearAccumulatedForces() -> void;
        
        /** Adds an impulse to movement */
        auto AddImpulse(FVector Impulse, bool bVelocityChange) -> void;
        
        /** Adds a force to movement */
        auto AddForce(FVector Force) -> void;
        // UFunction interfaces -> END
    };
}
