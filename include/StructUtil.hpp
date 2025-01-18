#pragma once
#include <Unreal/UObject.hpp>

struct FIntVector {
    int32_t X;
    int32_t Y; 
    int32_t Z;

    FIntVector() noexcept : X(0), Y(0), Z(0) {}
    FIntVector(int32_t InX, int32_t InY, int32_t InZ) noexcept : X(InX), Y(InY), Z(InZ) {}
    
    FIntVector(const FIntVector&) noexcept = default;
    FIntVector& operator=(const FIntVector&) noexcept = default;
    FIntVector(FIntVector&&) noexcept = default;
    FIntVector& operator=(FIntVector&&) noexcept = default;
};

struct FIntVector2D {
    int32_t X;
    int32_t Y;

    FIntVector2D() noexcept : X(0), Y(0) {}
    FIntVector2D(int32_t InX, int32_t InY) noexcept : X(InX), Y(InY) {}
    
    FIntVector2D(const FIntVector2D&) noexcept = default;
    FIntVector2D& operator=(const FIntVector2D&) noexcept = default;
    FIntVector2D(FIntVector2D&&) noexcept = default;
    FIntVector2D& operator=(FIntVector2D&&) noexcept = default;
};

// Macro for pointer type fields
#define UE4SS_FIELD_PTR(TYPE, NAME) \
    void set_##NAME(TYPE* value) \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<TYPE*>(STR(#NAME)); \
        if (ptr) { *ptr = value; } \
    } \
    \
    TYPE* get_##NAME() \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<TYPE*>(STR(#NAME)); \
        static TYPE* default_value{}; \
        return ptr ? *ptr : default_value; \
    } \
    \
    const TYPE* get_##NAME() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<TYPE*>(STR(#NAME)); \
        static TYPE* default_value{}; \
        return ptr ? *ptr : default_value; \
    } \
    __declspec(property(get = get_##NAME, put = set_##NAME)) TYPE* NAME

// Macro for value type fields
#define UE4SS_FIELD(TYPE, NAME) \
    void set_##NAME(TYPE value) \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<TYPE>(STR(#NAME)); \
        if (ptr) { *ptr = value; } \
    } \
    \
    TYPE get_##NAME() \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<TYPE>(STR(#NAME)); \
        static TYPE default_value{}; \
        return ptr ? *ptr : default_value; \
    } \
    \
    TYPE get_##NAME() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<TYPE>(STR(#NAME)); \
        static TYPE default_value{}; \
        return ptr ? *ptr : default_value; \
    } \
    __declspec(property(get = get_##NAME, put = set_##NAME)) TYPE NAME

// Vector field access
#define UE4SS_VECTOR_FIELD(NAME) \
    void set_##NAME(const RC::Unreal::FVector& value) \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<RC::Unreal::FVector>(STR(#NAME)); \
        if (ptr) { *ptr = value; } \
    } \
    \
    RC::Unreal::FVector get_##NAME() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<RC::Unreal::FVector>(STR(#NAME)); \
        return ptr ? *ptr : RC::Unreal::FVector{}; \
    } \
    __declspec(property(get = get_##NAME, put = set_##NAME)) RC::Unreal::FVector NAME

// Int vector field access with component getters
#define UE4SS_INT_VECTOR_FIELD(NAME) \
    void set_##NAME(const FIntVector& value) \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<FIntVector>(STR(#NAME)); \
        if (ptr) { *ptr = value; } \
    } \
    \
    void set_##NAME(int32_t x, int32_t y, int32_t z) \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<FIntVector>(STR(#NAME)); \
        if (ptr) { ptr->X = x; ptr->Y = y; ptr->Z = z; } \
    } \
    \
    FIntVector get_##NAME() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<FIntVector>(STR(#NAME)); \
        return ptr ? *ptr : FIntVector{}; \
    } \
    \
    int32_t get_##NAME##_x() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<FIntVector>(STR(#NAME)); \
        return ptr ? ptr->X : 0; \
    } \
    \
    int32_t get_##NAME##_y() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<FIntVector>(STR(#NAME)); \
        return ptr ? ptr->Y : 0; \
    } \
    \
    int32_t get_##NAME##_z() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<FIntVector>(STR(#NAME)); \
        return ptr ? ptr->Z : 0; \
    } \
    __declspec(property(get = get_##NAME, put = set_##NAME)) FIntVector NAME

// Enum field access
#define UE4SS_ENUM_FIELD(ENUM_TYPE, NAME) \
    void set_##NAME(ENUM_TYPE value) \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<uint8_t>(STR(#NAME)); \
        if (ptr) { *ptr = static_cast<uint8_t>(value); } \
    } \
    \
    ENUM_TYPE get_##NAME() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<uint8_t>(STR(#NAME)); \
        return ptr ? static_cast<ENUM_TYPE>(*ptr) : static_cast<ENUM_TYPE>(0); \
    } \
    __declspec(property(get = get_##NAME, put = set_##NAME)) ENUM_TYPE NAME

// Enum field access with custom member name
#define UE4SS_ENUM_FIELD_NAME(ENUM_TYPE, PROP_NAME, MEMBER_NAME) \
    void set_##MEMBER_NAME(ENUM_TYPE value) \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<uint8_t>(STR(#PROP_NAME)); \
        if (ptr) { *ptr = static_cast<uint8_t>(value); } \
    } \
    \
    ENUM_TYPE get_##MEMBER_NAME() const \
    { \
        auto* ptr = const_cast<UObject*>(static_cast<const UObject*>(this))->GetValuePtrByPropertyNameInChain<uint8_t>(STR(#PROP_NAME)); \
        return ptr ? static_cast<ENUM_TYPE>(*ptr) : static_cast<ENUM_TYPE>(0); \
    } \
    __declspec(property(get = get_##MEMBER_NAME, put = set_##MEMBER_NAME)) ENUM_TYPE MEMBER_NAME