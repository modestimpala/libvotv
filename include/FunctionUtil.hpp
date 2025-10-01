#pragma once
#include <Unreal/UFunction.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/FMemory.hpp>
#include <DynamicOutput/Output.hpp>
#include <bit>
#include <malloc.h>

namespace votv::util {

using RC::Unreal::UFunction;
using RC::Unreal::UObject;
using RC::Unreal::AActor;
using RC::Unreal::FName;
using RC::Unreal::FMemory;
using RC::Unreal::UClass;

class FunctionUtil {
public:
    static UFunction* FindFunction(UObject* object, const wchar_t* function_name) {
        if (!object) return nullptr;

        auto func = object->GetFunctionByName(function_name);
        if (func) return func;

        UClass* object_class = object->GetClassPrivate();
        if (!object_class) return nullptr;

        for (UFunction* function : object_class->ForEachFunctionInChain()) {
            if (function->GetName() == function_name) {
                return function;
            }
        }

        return nullptr;
    }

    static bool CallFunctionWithFName(AActor* actor, UFunction* function, 
                                     const wchar_t* param_name, const std::wstring& value) {
        if (!actor || !function) return false;

        try {
            auto param_size = function->GetParmsSize();
            auto param_data = static_cast<RC::Unreal::uint8*>(_malloca(param_size));
            
            FMemory::Memzero(param_data, param_size);
            
            auto property = function->FindProperty(FName(param_name));
            if (!property) {
                _freea(param_data);
                return false;
            }
            
            auto param = std::bit_cast<FName*>(&param_data[property->GetOffset_Internal()]);
            *param = FName(value.c_str(), RC::Unreal::FNAME_Add);
            
            actor->ProcessEvent(function, param_data);
            _freea(param_data);
            
            return true;
        } catch (...) {
            return false;
        }
    }

    struct FNameParam {
        const wchar_t* name;
        std::wstring value;
    };

    static bool CallFunction(AActor* actor, UFunction* function,
                           std::initializer_list<FNameParam> params = {},
                           bool* out_return = nullptr,
                           const wchar_t* return_name = STR("ReturnValue")) {
        if (!actor || !function) return false;

        try {
            auto param_size = function->GetParmsSize();
            auto param_data = static_cast<RC::Unreal::uint8*>(_malloca(param_size));
            
            FMemory::Memzero(param_data, param_size);
            
            for (const auto& param : params) {
                auto property = function->FindProperty(FName(param.name));
                if (!property) {
                    _freea(param_data);
                    return false;
                }
                
                auto param_ptr = std::bit_cast<FName*>(&param_data[property->GetOffset_Internal()]);
                *param_ptr = FName(param.value.c_str(), RC::Unreal::FNAME_Add);
            }
            
            actor->ProcessEvent(function, param_data);
            
            if (out_return) {
                auto return_property = function->FindProperty(FName(return_name));
                if (return_property && return_property->GetOffset_Internal() != INDEX_NONE) {
                    auto ret_value = std::bit_cast<bool*>(&param_data[return_property->GetOffset_Internal()]);
                    *out_return = *ret_value;
                }
            }
            
            _freea(param_data);
            return true;
        } catch (...) {
            return false;
        }
    }

    template<typename T>
    static T* GetPropertyValue(UObject* object, const wchar_t* property_name) {
        if (!object) return nullptr;
        return object->GetValuePtrByPropertyNameInChain<T>(property_name);
    }

    template<typename T>
    static bool SetPropertyValue(UObject* object, const wchar_t* property_name, const T& value) {
        if (!object) return false;
        
        auto* ptr = object->GetValuePtrByPropertyNameInChain<T>(property_name);
        if (!ptr) return false;
        
        *ptr = value;
        return true;
    }
};

}