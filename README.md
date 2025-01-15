# libvotv


[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![CMake 3.18+](https://img.shields.io/badge/CMake-3.18+-green.svg)](https://cmake.org/)
[![VotV pa0081_0008](https://img.shields.io/badge/VotV-pa0081__0008-orange.svg)](https://mrdrnose.itch.io/votvVoices_of_the_Void/)
[![UE4SS](https://img.shields.io/badge/UE4SS-Required-red.svg)](https://docs.ue4ss.com/)
[![Header Only](https://img.shields.io/badge/Header--Only-True-brightgreen.svg)]()

A modern C++ header-only library for interacting with Voices of the Void game state, providing type-safe access to game objects, player state, and environment variables.

## Features

- 🎮 Type-safe game state access through a single header
- 🔒 Memory-safe field manipulation
- 🎯 Direct access to core game systems
- 🧰 Modern C++ wrapper around UE4/UE4SS objects
- 📡 Object lifetime tracking utilities
- 🔧 Struct manipulation macros for clean field access of custom fields

## Table of Contents

- [Prerequisites](#prerequisites)
- [Version Compatibility](#version-compatibility)
- [Installation](#installation)
- [Usage](#usage)
- [Utility Classes](#utility-classes)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Contributing](#contributing)
- [Credits](#credits)

## Prerequisites

- CMake 3.18 or higher
- [UE4SS C++ Dev Environment Setup](https://github.com/modestimpala/RE-UE4SS)

## Version Compatibility ⚠️

This library targets Voices of the Void version pa0081_0008. The memory offsets are version-specific:
- If using a different game version, offsets will need to be updated
- Current offsets were verified against pa0081_0008
- Using incorrect offsets can cause crashes or undefined behavior

## Installation

The library consists of three headers:
- `game.hpp` - Main game state and objects
- `ObjectLifetimeTracker.hpp` - UObject lifetime management
- `StructUtil.hpp` - Field access macros and utilities

### As a CMake Subproject

1. Add the repository as a submodule:
```bash
git submodule add https://github.com/modestimpala/libvotv.git extern/libvotv
```

2. Add to your CMakeLists.txt:
```cmake
add_subdirectory(extern/libvotv)
target_link_libraries(your_project PRIVATE libvotv)
```

### Manual Installation / IDE Setup

Since the library is header-only:

1. Copy the three headers to your project
2. Add their location to your include path
3. Ensure UE4SS headers are available

## Usage

### Basic Setup

```cpp
#include <votv/game.hpp>  // Single header for all game functionality
#include <votv/ObjectLifetimeTracker.hpp>

using namespace votv::game;

// Get existing game mode from UE4SS
GameMode* gameMode = /* get from UE4SS */;

// Validate object is still alive
auto& tracker = ObjectLifetimeTracker::Get();
if (!tracker.IsActorAlive(gameMode)) return;

// Access player
MainPlayer* player = gameMode->mainPlayer;
if (!tracker.IsActorAlive(player)) return;

// Check player state
if (player->inWater && player->air < 50.0f) {
    // Handle low air situation
}
```

## Utility Classes

### ObjectLifetimeTracker

The `ObjectLifetimeTracker` provides safe UObject lifetime tracking.

Initialize the ObjectLifetimeTracker as early as possible in your mod lifecycle:

```cpp
auto on_unreal_init() -> void override
    auto& tracker = ObjectLifetimeTracker::Get(); // Get instance early
    // Rest of initialization...
}
```

Early initialization ensures no objects created during startup are missed by the tracking system.

Example:

```cpp
#include <ObjectLifetimeTracker.hpp>

// Get singleton instance
auto& tracker = ObjectLifetimeTracker::Get();

// Track specific types
tracker.RegisterTrackedType(Car::StaticClass());
tracker.RegisterTrackedName(L"specialDoor");

// Check if objects are still valid
if (tracker.IsActorAlive(someActor)) {
    // Safe to use
}
```

Features:
- Default tracking of GameMode and MainPlayer objects
- Registering custom types/names to track
- Thread-safe operation
- RAII-based resource management
- Protection against use-after-free

### StructUtil Macros

`StructUtil.hpp` provides type-safe field access macros:

```cpp
// Regular field access
FIELD(0x0600, MainPlayer*, mainPlayer);  // Generates getters/setters

// Vector field with component access
VECTOR_INT_FIELD(0x02F8, timeZ);  // Adds _x, _y, _z component access

// Enum field with type safety
ENUM_FIELD(0x01E1, GameModes::Type, gamemodeType);

// Bit field for flags
BIT_FIELD(0x0480, 0x01, someFlag);

// Array field access
ARRAY_FIELD(0x0208, RC::Unreal::TArray<float>, scores);
```

Features:
- Automatic getter/setter generation
- Type-safe field access
- Support for vectors, enums, arrays and bit fields
- Property syntax support through __declspec
- All methods marked noexcept where appropriate
- Proper reference handling (lvalue/rvalue)

## API Reference

### Core Game Classes (game.hpp)

All game functionality is contained in a single header under the `votv::game` namespace. 

```cpp
namespace votv::game {
    class GameMode;
    class MainPlayer;
    class DayNightCycle;
    class Car;
    class Door;
    class KerfurOmega;
    class GameInst;
    class SaveSlot;
}

PowerInfo GetPowerInfo() const;  // Get complete power system state
TimeInfo GetTimeInfo() const; // Get complete time info
WeatherInfo GetWeatherInfo() const; // Get complete weather info

// Game Instance
class GameInst : public RC::Unreal::UObject {
    SaveSlot* save;             // Active save
    GameModes::Type gamemodeType; // Game mode Type (Sandbox, Story, etc)
    bool opened;                // Instance opened
    int32_t startDay;           // Starting day
    ...
};

// Save slot
class SaveSlot : public RC::Unreal::UObject {
    float food;                 // Food level
    float sleep;                // Sleep level
    int32_t Points;             // Money
    float health;               // Current health
    ...
};

```

See header file for complete API.

## Examples

### Safe Object Access Pattern

```cpp
#include <votv/game.hpp>
#include <votv/ObjectLifetimeTracker.hpp>

void SafeGameObjectAccess(votv::game::GameMode* gameMode) {
    auto& tracker = ObjectLifetimeTracker::Get();
    
    // Validate game mode
    if (!tracker.IsActorAlive(gameMode)) return;

    // Access and validate player
    auto* player = gameMode->mainPlayer;
    if (!tracker.IsActorAlive(player)) return;

    // Safe to use objects
    auto state = player->GetState();
    auto power = gameMode->GetPowerInfo();

    if (state.inWater && power.ratio < 0.2f) {
        // Handle dangerous situation
    }
}
```

### Custom Object Tracking

```cpp
#include <votv/game.hpp>
#include <votv/ObjectLifetimeTracker.hpp>

void SetupTracking() {
    auto& tracker = ObjectLifetimeTracker::Get();

    // Track all cars
    tracker.RegisterTrackedType(Car::StaticClass());

    // Track special objects
    tracker.RegisterTrackedName(L"prop_important");

    // Later cleanup if needed
    tracker.UnregisterTrackedType(Car::StaticClass());
    tracker.UnregisterTrackedName(L"prop_important");
}
```

### Time and Weather Management

```cpp
#include <votv/game.hpp>

void HandleTimeChange(DayNightCycle* dnc) {
    // Get complete time state
    auto timeInfo = dnc->GetTimeInfo();
    auto weather = dnc->GetWeatherInfo();
    

    // Access raw day counter
    int32_t currentDay = dnc->current_day();
    
    // Modify time
    dnc->set_timeZ(0, 0, currentDay + 1); 
}
```

## Defining Custom Fields from UE4SS Header Dumps

See [UE4SS Dumpers](https://docs.ue4ss.com/dev/feature-overview/dumpers.html#c-header-generator)

See [UE4SS Dumper Converter](https://github.com/modestimpala/RE-UE4SS/blob/main/DumpConverter.md)

Make sure DumpOffsetsAndSizes and LoadAllAssetsBeforeGeneratingCXXHeaders are 1 then dump headers from main menu.

Find specific class, then map offsets:


```cpp
// Dumped cremator.hpp
class Acremator_C : public Aactor_save_C
{
    ...
    bool IsClosed;                                                                    // 0x0351 (size: 0x1)
    ...
}

#include <votv/game.hpp>
#include <StructUtil.hpp>

// Your custom wrapper class (e.g., extra_game.hpp)
class Cremator : public RC::Unreal::AActor {
public:
    FIELD(0x0351, bool, isClosed);    // Maps to IsClosed from dump
};
```

And so on for any other custom fields needed.

## Contributing

1. Fork the repository
2. Create your feature branch (git checkout -b feature/extra-offsets)
3. Commit your changes (git commit -m 'More offsets')
4. Push to the branch (git push origin feature/extra-offsets)
5. Open a Pull Request


## Credits

- MrDrNose for VotV
- UE4SS and their Discord Community
- agersant (gbvsr-frame-meter)

