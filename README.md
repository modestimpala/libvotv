# libvotv

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![CMake 3.18+](https://img.shields.io/badge/CMake-3.18+-green.svg)](https://cmake.org/)
[![VotV 0.9.0](https://img.shields.io/badge/VotV-0.9.0-orange.svg)](https://mrdrnose.itch.io/votvVoices_of_the_Void/)
[![Header Only](https://img.shields.io/badge/Header--Only-brightgreen.svg)]()

Header-only C++ library for Voices of the Void modding. Type-safe game object access with memory safety utilities.

## Features

- Type-safe wrappers for game objects (Player, GameMode, Car, etc.)
- Object lifetime tracking to prevent use-after-free
- Utility functions for common modding patterns
- Field access macros for custom UE4 structs
- Hook parameter extraction helpers

## Prerequisites

- CMake 3.18+
- [UE4SS C++ Dev Environment](https://github.com/modestimpala/RE-UE4SS)
- Lib targets VotV 0.9.0

## Installation

### CMake

```bash
git submodule add https://github.com/modestimpala/libvotv.git extern/libvotv
```

```cmake
add_subdirectory(extern/libvotv)
target_link_libraries(your_mod PRIVATE libvotv)
```

### Manual

Copy headers to your project and add to include path:
- `game.hpp`
- `StructUtil.hpp`
- `ObjectLifetimeTracker.hpp`
- `FunctionUtil.hpp`
- `CommonUtil.hpp`

## Quick Start

### Game Objects (game.hpp)

```cpp
class GameMode {
    MainPlayer* mainPlayer;
    SaveSlot* saveSlot;
    DayNightCycle* daynightCycle;
    float totalPower;
    float usedPower;
    bool isHalloween;
    // ...
    
    PowerInfo GetPowerInfo() const;
};

class MainPlayer {
    bool underwaterPlayer;
    bool inWater;
    bool dead;
    float air;
    float foodDrain;
    float sleepDrain;
    AActor* holding_actor;
    // ...

    PlayerState GetState() const;
};

class DayNightCycle {
    int32_t current_day() const;
    float phase;
    float rain;
    bool isRaining;
    // ...
    
    TimeInfo GetTimeInfo() const;
    WeatherInfo GetWeatherInfo() const;
};
```

Also includes: `ATV`, `Bed`, `Grime`, `GrowingPlant`, `ServerBox`, `SitBox`, `Padlock`, `ToolGun`, `VidCam`, `Workbench`, `ATM`, `SleepingBag`, `Whiteboard`

Enums: `GameModes`, `Seasons`, `ToolCategories`, `FarmPlantTypes`, `PhotographicTypes`, `HotkeyActions`, `TrashTypes`, `PhysicalModules`

See [game.hpp](include/game.hpp) for complete API.

## Utilities

### ObjectLifetimeTracker

Prevents use-after-free by tracking UObject lifetimes.

```cpp
auto& tracker = ObjectLifetimeTracker::Get();

// Track custom types
tracker.RegisterTrackedType(Car::StaticClass());
tracker.RegisterTrackedName(L"door");

// Check validity
if (tracker.IsActorAlive(someActor)) {
    // Safe to use
}

// Find tracked objects
auto cars = tracker.FindObjectsByClass(Car::StaticClass());
```

### FunctionUtil

Call Blueprint functions without boilerplate.

```cpp
auto* actor = /* get actor */;
auto* func = FunctionUtil::FindFunction(actor, STR("SomeFunction"));

// No parameters
FunctionUtil::CallFunction(actor, func);

// Single parameter
FunctionUtil::CallFunction(actor, func, {{"Name", username}});

// Multiple parameters
FunctionUtil::CallFunction(actor, func, {
    {"Name", username},
    {"Message", message}
});

// With return value
bool result;
FunctionUtil::CallFunction(actor, func, {{"Name", username}}, &result);
```

### CommonUtil

```cpp
// String conversion
auto wide = StringConv::ToWide("hello");
auto narrow = StringConv::ToNarrow(L"hello");

// Color utilities
auto color = Color::FromHex("#FF5733");
uint32_t bgra = color.ToPackedBGRA();

// Async worker
AsyncWorker worker;
worker.queue_task([&]() {
    // Background task
});

// Safe parameter allocation (RAII)
ParamGuard params(function->GetParmsSize());
if (!params) return false;
// Automatically freed on scope exit

// Hook parameter extraction
auto username = HookUtil::ExtractParamAsString(ctx, STR("Chatter"));
if (username) {
    // Use *username
}
```

## Custom Fields

Map UE4SS header dump offsets to C++ classes:

1. Generate headers with UE4SS dumper (enable DumpOffsetsAndSizes)
2. Find your class in dumps
3. Map fields using macros

```cpp
// From UE4SS dump: Acremator_C
// bool IsClosed; // 0x0351 (size: 0x1)

#include <votv/game.hpp>
#include <StructUtil.hpp>

class Cremator : public RC::Unreal::AActor {
public:
    UE4SS_FIELD(bool, isClosed);  // Generates getter/setter
};
```

Available macros:
- `UE4SS_FIELD(type, name)` - Basic field
- `UE4SS_FIELD_PTR(type, name)` - Pointer field
- `UE4SS_VECTOR_FIELD(name)` - FVector field
- `UE4SS_INT_VECTOR_FIELD(name)` - FIntVector with component access
- `UE4SS_ENUM_FIELD(enum, name)` - Enum field

See [UE4SS Dumpers](https://docs.ue4ss.com/dev/feature-overview/dumpers.html) for dump generation.

## Examples

### Safe Object Pattern

```cpp
void DoSomething(GameMode* gm) {
    auto& tracker = ObjectLifetimeTracker::Get();
    
    if (!tracker.IsActorAlive(gm)) return;
    if (!tracker.IsActorAlive(gm->mainPlayer)) return;
    
    // Safe to use both objects
    auto state = gm->mainPlayer->GetState();
    auto power = gm->GetPowerInfo();
}
```

### Calling Blueprint Functions

```cpp
auto* func = FunctionUtil::FindFunction(actor, STR("TwitchChat"));
if (!func) return;

FunctionUtil::CallFunction(actor, func, {
    {"Name", StringConv::ToWide(username)},
    {"Message", StringConv::ToWide(message)}
});
```

### Hook with Parameter Extraction

```cpp
UObjectGlobals::RegisterHook(
    STR("/Game/Path/To/Function"),
    [](auto& ctx, void*) {}, // pre
    [](auto& ctx, void*) {   // post
        if (auto name = HookUtil::ExtractParamAsString(ctx, STR("Chatter"))) {
            // Use *name
        }
    },
    nullptr
);
```

## Contributing

Submit PRs for:
- Additional game object mappings
- Offset updates for new game versions
- Utility function improvements
- Bug fixes

## Credits

- MrDrNose - VotV
- UE4SS and their Discord Community
- agersant (gbvsr-frame-meter)