
#include "engine/include/CharacterMovement.hpp"

#include "engine/include/enums/EMovementMode.hpp"

namespace RC::Unreal
{
    IMPLEMENT_EXTERNAL_OBJECT_CLASS(CharacterMovementComponent)

    Function<CharacterMovementComponent::BeginPlaySignature> CharacterMovementComponent::BeginPlayInternal;
    Function<CharacterMovementComponent::TickSignature> CharacterMovementComponent::TickInternal;
    std::unordered_map<File::StringType, uint32_t> CharacterMovementComponent::VTableLayoutMap;

    EMovementMode CharacterMovementComponent::MovementMode()
    {
        // get property by name
        auto MovementModeProp = GetPropertyByNameInChain(STR("MovementMode"));
        if (!MovementModeProp)
        {
            Output::send<LogLevel::Error>(STR("MovementMode property not found\n"));
            return EMovementMode::MOVE_None;
        }
        // get ptr
        auto* MovementModePtr = MovementModeProp->ContainerPtrToValuePtr<uint8_t>(this);
        if (!MovementModePtr)
        {
            Output::send<LogLevel::Error>(STR("Failed to get MovementMode\n"));
            return EMovementMode::MOVE_None;
        }
        // dereference the pointer
        return static_cast<EMovementMode>(*MovementModePtr);
    }

    auto CharacterMovementComponent::SetWalkableFloorZ(float InWalkableFloorZ) -> void
    {
        UE_BEGIN_NATIVE_FUNCTION_BODY("/Script/Engine.CharacterMovementComponent:SetWalkableFloorZ")
        UE_COPY_PROPERTY(InWalkableFloorZ, float)
        UE_CALL_FUNCTION()
    }

    auto CharacterMovementComponent::SetWalkableFloorAngle(float InWalkableFloorAngle) -> void
    {
        UE_BEGIN_NATIVE_FUNCTION_BODY("/Script/Engine.CharacterMovementComponent:SetWalkableFloorAngle")
        UE_COPY_PROPERTY(InWalkableFloorAngle, float)
        UE_CALL_FUNCTION()
    }

    auto CharacterMovementComponent::SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode) -> void
    {
        UE_BEGIN_NATIVE_FUNCTION_BODY("/Script/Engine.CharacterMovementComponent:SetMovementMode")
        UE_COPY_PROPERTY(NewMovementMode, EMovementMode)
        UE_COPY_PROPERTY(NewCustomMode, uint8)
        UE_CALL_FUNCTION()
    }

    auto CharacterMovementComponent::K2_GetWalkableFloorZ() -> float
    {
        UE_BEGIN_NATIVE_FUNCTION_BODY("/Script/Engine.CharacterMovementComponent:K2_GetWalkableFloorZ")
        UE_CALL_FUNCTION()
        UE_RETURN_PROPERTY(float)
    }

    // 
}
