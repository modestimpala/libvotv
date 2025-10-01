#pragma once
#include <string>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <memory>
#include <DynamicOutput/Output.hpp>

namespace votv::util {

struct FColor {
    uint8_t r, g, b;
    
    static FColor FromHex(const std::string& hex_color) {
        std::string hex = hex_color;
        if (hex[0] == '#') hex = hex.substr(1);
        if (hex.length() != 6) throw std::invalid_argument("Invalid hex color format");
        
        return {
            static_cast<uint8_t>(std::stoul(hex.substr(0, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stoul(hex.substr(2, 2), nullptr, 16)),
            static_cast<uint8_t>(std::stoul(hex.substr(4, 2), nullptr, 16))
        };
    }
    
    uint32_t ToPackedBGRA(uint8_t alpha = 255) const {
        return (static_cast<uint32_t>(b) << 24) |
               (static_cast<uint32_t>(g) << 16) |
               (static_cast<uint32_t>(r) << 8) |
               static_cast<uint32_t>(alpha);
    }
};

class StringConv {
public:
    static std::wstring ToWide(const std::string& str) {
        return std::wstring(str.begin(), str.end());
    }
    
    static std::string ToNarrow(const std::wstring& wstr) {
        return std::string(wstr.begin(), wstr.end());
    }
};

class AsyncWorker {
public:
    AsyncWorker() : shutdown_(false) {
        worker_thread_ = std::make_unique<std::jthread>([this](std::stop_token token) {
            worker_loop(token);
        });
    }
    
    ~AsyncWorker() {
        shutdown();
    }
    
    void queue_task(std::function<void()> task) {
        {
            std::lock_guard lock(queue_mutex_);
            task_queue_.push(std::move(task));
        }
        cv_.notify_one();
    }
    
    void shutdown() {
        shutdown_ = true;
        cv_.notify_all();
    }
    
    bool is_shutting_down() const {
        return shutdown_;
    }

private:
    void worker_loop(std::stop_token stop_token) {
        while (!stop_token.stop_requested() && !shutdown_) {
            std::function<void()> task;
            
            {
                std::unique_lock lock(queue_mutex_);
                cv_.wait(lock, [this, &stop_token] {
                    return !task_queue_.empty() || stop_token.stop_requested() || shutdown_;
                });
                
                if (stop_token.stop_requested() || shutdown_) break;
                
                if (!task_queue_.empty()) {
                    task = std::move(task_queue_.front());
                    task_queue_.pop();
                }
            }
            
            if (task) {
                try {
                    task();
                } catch (const std::exception& e) {
                    RC::Output::send<RC::LogLevel::Error>(
                        STR("[AsyncWorker] Task error: {}\n"),
                        StringConv::ToWide(e.what())
                    );
                }
            }
        }
    }
    
    std::queue<std::function<void()>> task_queue_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    std::atomic<bool> shutdown_;
    std::unique_ptr<std::jthread> worker_thread_;
};

class SafeCall {
public:
    template<typename Callable>
    static bool Execute(Callable&& func, const char* operation_name = "operation") {
        try {
            return func();
        } catch (const std::exception& e) {
            RC::Output::send<RC::LogLevel::Error>(
                STR("[SafeCall] Exception in {}: {}\n"),
                StringConv::ToWide(operation_name),
                StringConv::ToWide(e.what())
            );
            return false;
        } catch (...) {
            RC::Output::send<RC::LogLevel::Error>(
                STR("[SafeCall] Unknown exception in {}\n"),
                StringConv::ToWide(operation_name)
            );
            return false;
        }
    }
};

class ParamGuard {
public:
    ParamGuard(size_t size) : data_(static_cast<uint8_t*>(_malloca(size))), size_(size) {
        if (data_) {
            memset(data_, 0, size_);
        }
    }
    
    ~ParamGuard() {
        if (data_) _freea(data_);
    }
    
    ParamGuard(const ParamGuard&) = delete;
    ParamGuard& operator=(const ParamGuard&) = delete;
    
    uint8_t* get() { return data_; }
    operator uint8_t*() { return data_; }
    explicit operator bool() const { return data_ != nullptr; }
    
private:
    uint8_t* data_;
    size_t size_;
};

class HookUtil {
public:
    static std::optional<std::string> ExtractParamAsString(
        RC::Unreal::UnrealScriptFunctionCallableContext& ctx,
        const wchar_t* param_name)
    {
        using namespace RC::Unreal;
        
        UFunction* function = ctx.TheStack.Node();
        if (!function) return std::nullopt;
        
        for (FProperty* property : function->ForEachPropertyInChain())
        {
            if (property->GetFName() == FName(param_name))
            {
                void* valuePtr = property->ContainerPtrToValuePtr<void>(ctx.TheStack.Locals());
                if (!valuePtr) return std::nullopt;
                
                FName fname = *static_cast<FName*>(valuePtr);
                std::wstring wstr = fname.ToString();
                return StringConv::ToNarrow(wstr);
            }
        }
        
        return std::nullopt;
    }
    
    static std::optional<RC::Unreal::FName> ExtractFNameParam(
        RC::Unreal::UnrealScriptFunctionCallableContext& ctx,
        const wchar_t* param_name)
    {
        using namespace RC::Unreal;
        
        UFunction* function = ctx.TheStack.Node();
        if (!function) return std::nullopt;
        
        for (FProperty* property : function->ForEachPropertyInChain())
        {
            if (property->GetFName() == FName(param_name))
            {
                void* valuePtr = property->ContainerPtrToValuePtr<void>(ctx.TheStack.Locals());
                if (!valuePtr) return std::nullopt;
                
                return *static_cast<FName*>(valuePtr);
            }
        }
        
        return std::nullopt;
    }
};

}