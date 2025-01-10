/**
 * @file StructUtil.hpp
 * @brief Utility macros and structures for handling memory offsets and fields in C++.
 * 
 * This header file provides a set of macros and a structure to facilitate the declaration
 * and manipulation of fields at specific memory offsets within a class or structure.
 * It includes support for integer vectors, generic fields, array fields, bit fields, 
 * and enum fields.
 * 
 * @struct FIntVector
 * @brief A 3D vector of integers, optimized for stack allocation.
 * 
 * The FIntVector structure provides X, Y, Z components and basic initialization.
 * 
 * @macro VALIDATE_OFFSET(OFFSET)
 * @brief Validates that the given offset is within the allowed range.
 * 
 * This macro performs compile-time checks to ensure that the offset is non-negative
 * and does not exceed the maximum allowed value (0x10000).
 * 
 * @macro VECTOR_INT_FIELD(OFFSET, NAME)
 * @brief Declares a vector field at the specified memory offset with getters and setters.
 * 
 * This macro defines a vector field of type FIntVector at the given offset within a class
 * or structure. It provides getter and setter functions for the entire vector as well as
 * individual components (X, Y, Z).
 * 
 * @macro FIELD(OFFSET, TYPE, NAME)
 * @brief Declares a generic field at the specified memory offset with getters and setters.
 * 
 * This macro defines a field of the specified type at the given offset within a class or
 * structure. It provides getter and setter functions for the field.
 * 
 * @macro ARRAY_FIELD(OFFSET, TYPE, NAME)
 * @brief Declares an array field at the specified memory offset with getters and setters.
 * 
 * This macro defines an array field of the specified type at the given offset within a class
 * or structure. It provides getter and setter functions for individual elements of the array.
 * 
 * @macro BIT_FIELD(OFFSET, MASK, NAME)
 * @brief Declares a bit field at the specified memory offset with getters and setters.
 * 
 * This macro defines a bit field with the specified mask at the given offset within a class
 * or structure. It provides getter and setter functions for the bit field.
 * 
 * @macro ENUM_FIELD(OFFSET, ENUM_TYPE, NAME)
 * @brief Declares an enum field at the specified memory offset with getters and setters.
 * 
 * This macro defines an enum field of the specified type at the given offset within a class
 * or structure. It provides getter and setter functions for the enum field.
 */
#pragma once
#include <type_traits>

/// A 3D vector of integers, optimized for stack allocation
/// Provides X, Y, Z components and basic initialization
struct FIntVector {
    int32_t X;
    int32_t Y; 
    int32_t Z;

    /// Default constructor initializes to (0,0,0)
    FIntVector() noexcept : X(0), Y(0), Z(0) {}
    
    /// Construct vector from x,y,z components
    FIntVector(int32_t InX, int32_t InY, int32_t InZ) noexcept : X(InX), Y(InY), Z(InZ) {}
    
    // Prevent any accidental heap allocations
    FIntVector(const FIntVector&) noexcept = default;
    FIntVector& operator=(const FIntVector&) noexcept = default;
    FIntVector(FIntVector&&) noexcept = default;
    FIntVector& operator=(FIntVector&&) noexcept = default;
};

// Simple compile-time checks
#define VALIDATE_OFFSET(OFFSET) \
    static_assert((OFFSET) >= 0, "Offset must be non-negative"); \
    static_assert((OFFSET) < 0x10000, "Offset exceeds maximum allowed value");

/// Declare a vector field at the specified memory offset with getters/setters
#define VECTOR_INT_FIELD(OFFSET, NAME)                                    \
    VALIDATE_OFFSET(OFFSET)                                              \
    void set_##NAME(const FIntVector& value) noexcept                    \
    {                                                                    \
        auto vec = (FIntVector*)((char*)this + OFFSET);                 \
        *vec = value;                                                   \
    }                                                                   \
                                                                        \
    void set_##NAME(int32_t x, int32_t y, int32_t z) noexcept          \
    {                                                                   \
        auto vec = (FIntVector*)((char*)this + OFFSET);                \
        vec->X = x;                                                    \
        vec->Y = y;                                                    \
        vec->Z = z;                                                    \
    }                                                                  \
                                                                       \
    const FIntVector& get_##NAME() const noexcept                      \
    {                                                                  \
        return *((FIntVector*)((char*)this + OFFSET));                \
    }                                                                 \
                                                                      \
    int32_t get_##NAME##_x() const noexcept                          \
    {                                                                 \
        return ((FIntVector*)((char*)this + OFFSET))->X;             \
    }                                                                \
                                                                     \
    int32_t get_##NAME##_y() const noexcept                         \
    {                                                                \
        return ((FIntVector*)((char*)this + OFFSET))->Y;             \
    }                                                                \
                                                                     \
    int32_t get_##NAME##_z() const noexcept                         \
    {                                                                \
        return ((FIntVector*)((char*)this + OFFSET))->Z;             \
    }                                                                \
    __declspec(property(get = get_##NAME, put = set_##NAME)) FIntVector NAME

#define FIELD(OFFSET, TYPE, NAME)                                                \
    VALIDATE_OFFSET(OFFSET)                                                     \
    static_assert(!std::is_void_v<TYPE>, "Void type is not allowed");          \
    void set_##OFFSET(std::add_const_t<std::add_lvalue_reference_t<TYPE>> value) \
    {                                                                            \
        *(std::add_pointer_t<TYPE>)((char *)this + OFFSET) = value;              \
    }                                                                            \
                                                                                 \
    void set_##OFFSET(std::add_rvalue_reference_t<TYPE> value)                   \
    {                                                                            \
        *(std::add_pointer_t<TYPE>)((char *)this + OFFSET) = std::move(value);   \
    }                                                                            \
                                                                                 \
    std::add_lvalue_reference_t<TYPE> get_##OFFSET() const                       \
    {                                                                            \
        return *(std::add_pointer_t<TYPE>)((char *)this + OFFSET);               \
    }                                                                            \
    __declspec(property(get = get_##OFFSET, put = set_##OFFSET)) TYPE NAME

#define ARRAY_FIELD(OFFSET, TYPE, NAME)                                                                      \
    VALIDATE_OFFSET(OFFSET)                                                                                 \
    void set_##OFFSET(int index, std::add_const_t<std::add_lvalue_reference_t<array_elem_type<TYPE>>> value) \
    {                                                                                                        \
        ((std::decay_t<TYPE>)((char *)this + OFFSET))[index] = value;                                        \
    }                                                                                                        \
                                                                                                             \
    void set_##OFFSET(int index, std::add_rvalue_reference_t<array_elem_type<TYPE>> value)                   \
    {                                                                                                        \
        ((std::decay_t<TYPE>)((char *)this + OFFSET))[index] = std::move(value);                             \
    }                                                                                                        \
                                                                                                             \
    std::add_lvalue_reference_t<array_elem_type<TYPE>> get_##OFFSET(int index) const                         \
    {                                                                                                        \
        return ((std::decay_t<TYPE>)((char *)this + OFFSET))[index];                                         \
    }                                                                                                        \
    __declspec(property(get = get_##OFFSET, put = set_##OFFSET)) array_elem_type<TYPE> NAME[array_elem_count<TYPE>]

#define BIT_FIELD(OFFSET, MASK, NAME)                            \
    VALIDATE_OFFSET(OFFSET)                                     \
    static_assert((MASK) != 0, "Bit mask cannot be 0");        \
    void set_##OFFSET##_##MASK(bool value)                       \
    {                                                            \
        if (value)                                               \
            *(int *)((char *)this + OFFSET) |= MASK;             \
        else                                                     \
            *(int *)((char *)this + OFFSET) &= ~MASK;            \
    }                                                            \
                                                                 \
    bool get_##OFFSET##_##MASK() const                           \
    {                                                            \
        return (*(int *)((char *)this + OFFSET) & MASK) == MASK; \
    }                                                            \
    __declspec(property(get = get_##OFFSET##_##MASK, put = set_##OFFSET##_##MASK)) bool NAME

#define ENUM_FIELD(OFFSET, ENUM_TYPE, NAME)                                     \
    VALIDATE_OFFSET(OFFSET)                                                    \
    static_assert(std::is_enum_v<ENUM_TYPE>, "Type must be an enum");         \
    void set_##OFFSET(ENUM_TYPE value)                                         \
    {                                                                          \
        *(uint8_t*)((char *)this + OFFSET) = static_cast<uint8_t>(value);     \
    }                                                                          \
                                                                              \
    ENUM_TYPE get_##OFFSET() const                                            \
    {                                                                         \
        return static_cast<ENUM_TYPE>(*(uint8_t*)((char *)this + OFFSET));    \
    }                                                                         \
    __declspec(property(get = get_##OFFSET, put = set_##OFFSET)) ENUM_TYPE NAME