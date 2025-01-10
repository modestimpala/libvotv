#pragma once
#include <Unreal/AActor.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/FString.hpp>
#include <StructUtil.hpp>

#include "AGameMode.hpp"

namespace votv::game {

// All offsets are for Voices of the Void version pa0081_0008
// If you're using a different version, offsets may need to be updated
#ifndef VOTV_VERSION
#define VOTV_VERSION "pa0081_0008"
#endif

// Forward declarations
class MainPlayer;
class SaveSlot;
class DayNightCycle;
class GameInst;
class GameMode;

// Game Modes enum
namespace GameModes {
    enum Type {
        Story = 0,
        Infinite = 1,
        Tutorial = 2,
        Custom = 3,
        Sandbox = 4,
        Halloween = 5,
        Ambient = 6,
        Solar = 7,
        Max = 8
    };
}

// Main Game Mode class
class GameMode : public RC::Unreal::AGameMode {
public:
    // Direct field access
    FIELD(0x0600, MainPlayer*, mainPlayer);
    FIELD(0x0480, SaveSlot*, saveSlot);
    FIELD(0x06F8, bool, isHalloween);
    FIELD(0x0420, DayNightCycle*, daynightCycle);
    FIELD(0x08D8, GameInst*, gameInstance);
    
    // Player state fields
    FIELD(0x0BB4, float, sleepTime);
    FIELD(0x0760, bool, hasVirus);
    FIELD(0x04BC, bool, isSleeping);
    FIELD(0x0538, bool, isDreaming);
    FIELD(0x0874, bool, IsFlying);
    FIELD(0x0E74, bool, isInMainMenu);
    FIELD(0x08A9, bool, backroomsEnabled);
    FIELD(0x08D0, bool, canDamage);
    FIELD(0x0BD0, bool, isImmortal);

    // Power system fields
    FIELD(0x0380, float, powerCalculating);
    FIELD(0x0384, float, powerDownloading);
    FIELD(0x0388, float, powerCoordinates);
    FIELD(0x038C, float, powerPlaying);
    FIELD(0x0390, float, powerLight);
    FIELD(0x0394, float, totalPower);
    FIELD(0x0398, float, usedPower);
    FIELD(0x039C, float, powerRatio);

    // Convenient struct wrapper for power info
    struct PowerInfo {
        float calculating;
        float downloading;
        float coordinates;
        float playing;
        float light;
        float total;
        float used;
        float ratio;
    };

    PowerInfo GetPowerInfo() const {
        return {
            powerCalculating,
            powerDownloading,
            powerCoordinates,
            powerPlaying,
            powerLight,
            totalPower,
            usedPower,
            powerRatio
        };
    }
};

// Main Player class
class MainPlayer : public RC::Unreal::AActor {
public:
    // Direct field access
    FIELD(0x08A0, bool, inWater);
    FIELD(0x08BA, bool, underwater);
    FIELD(0x08BB, bool, noclip);
    FIELD(0x08BC, bool, crouching);
    FIELD(0x0978, bool, dead);
    FIELD(0x0A24, float, air);
    FIELD(0x0A48, bool, wakingUp);
    FIELD(0x0A80, float, foodDrain);
    FIELD(0x0A84, float, sleepDrain);
    FIELD(0x0A90, bool, mirror);
    FIELD(0x0BC0, bool, combat);
    FIELD(0x0BC1, bool, hasGlasses);
    FIELD(0x0BC2, bool, hasFlashlight);
    FIELD(0x0B80, GameMode*, gameMode);
    FIELD(0x06D0, AActor*, pickup_actor);

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
            this->wakingUp,
            this->foodDrain,
            this->sleepDrain,
            this->mirror,
            this->combat,
            this->hasGlasses,
            this->hasFlashlight
        };
    }
};

// Car (ATV) class
class Car : public RC::Unreal::AActor {
public:
    FIELD(0x0468, bool, isDriving);
    FIELD(0x046C, float, fuel);
    FIELD(0x0481, bool, broken);
    FIELD(0x047C, float, health);
    FIELD(0x04C8, bool, underwater);
    
    bool hasFuel() const { return fuel > 0.0f; }
};

// Kerfur Omega class
class KerfurOmega : public RC::Unreal::AActor {
public:
    FIELD(0x07A0, bool, sentient);
    FIELD(0x07F0, float, meow);
    FIELD(0x0810, bool, skipFaceAssign);
};

// Base prop class
class Prop : public RC::Unreal::AActor {
public:
    FIELD(0x02D0, bool, isStatic);
    FIELD(0x02D2, bool, frozen);
    FIELD(0x02D3, bool, ignoreSave);
    FIELD(0x02D8, RC::Unreal::FString, key);
};

// Door class 
class Door : public RC::Unreal::AActor {
public:
    FIELD(0x0310, bool, isOpened);
    FIELD(0x0311, bool, isMoving);
    FIELD(0x0312, bool, isActive);
    FIELD(0x0313, bool, autoclose);
    FIELD(0x0380, bool, jammed);
};

class GameInst : public RC::Unreal::UObject {
public:
    FIELD(0x01A8, SaveSlot*, save);
    FIELD(0x01B0, RC::Unreal::FString, SlotName);
    FIELD(0x01E0, bool, opened);
    ENUM_FIELD(0x01E1, GameModes::Type, gamemodeType);
    // 0x01E2 - 0x01E3 is padding (2 bytes)
    FIELD(0x01E4, int32_t, startDay);
    FIELD(0x0228, bool, isSaveReset);
    FIELD(0x0229, bool, loadObjects);
    // 0x022A - 0x022F is padding (6 bytes)
    FIELD(0x0250, RC::Unreal::FVector, locationDistance);
    FIELD(0x025C, RC::Unreal::FName, subArea);
    FIELD(0x0264, bool, antibreatherKickout);
    // 0x0265 - 0x0267 is padding (3 bytes)
    FIELD(0x0268, RC::Unreal::FString, dwString);
    FIELD(0x0288, RC::Unreal::FString, dwString_2);
    FIELD(0x02A8, GameMode*, gamemode_);
    FIELD(0x02B0, bool, NewVar_0);
    
    GameInst() = default;
};

class SaveSlot : public RC::Unreal::UObject {
public:
    FIELD(0x0100, float, totalTime);
    FIELD(0x0104, float, food);
    FIELD(0x0108, float, sleep);
    FIELD(0x00B0, int32_t, Points);
    FIELD(0x0408, float, health);
    FIELD(0x0638, float, p_health);
    FIELD(0x063C, float, Day);
    FIELD(0x0854, float, maxHealth);
};


// Day/Night Cycle class
class DayNightCycle : public RC::Unreal::AActor {
public:
     // Time fields
    VECTOR_INT_FIELD(0x02F8, timeZ);
    // Create easy access to Z component which represents the day
    int32_t current_day() const { return get_timeZ_z(); }

	FIELD(0x0300, int32_t, timeZ_Z); // Day
    FIELD(0x02A8, float, day);
    FIELD(0x02AC, float, phase);
    FIELD(0x02B0, float, phaseSin);
    FIELD(0x02B4, float, phaseNormSin);
    FIELD(0x02B8, float, maxTime);
    FIELD(0x02BC, float, totalTime);
    FIELD(0x02C0, float, timeScale);

    // Weather fields
    FIELD(0x02F4, bool, isActive);
    FIELD(0x0304, bool, starActive);
    FIELD(0x0308, float, rain);
    FIELD(0x030C, bool, isRaining);
    FIELD(0x030D, bool, rainMuted);
    FIELD(0x0310, float, diffMult);
    FIELD(0x0318, float, sunHeight);
    FIELD(0x031C, float, rainSpeed);
    FIELD(0x0350, float, thickFog);

    // Convenient struct wrappers
    struct TimeInfo {
        int32_t current_day;
        float day;
        float phase;
        float phaseSin;
        float phaseNormSin;
        float maxTime;
        float totalTime;
        float timeScale;
    };

    struct WeatherInfo {
        bool isActive;
        bool starActive;
        float rain;
        bool isRaining;
        bool rainMuted;
        float diffMult;
        float sunHeight;
        float rainSpeed;
        float thickFog;
    };


    TimeInfo GetTimeInfo() const {
        return {
            current_day(), day, phase, phaseSin, phaseNormSin,
            maxTime, totalTime, timeScale
        };
    }

    WeatherInfo GetWeatherInfo() const {
        return {
            isActive, starActive, rain, isRaining, rainMuted,
            diffMult, sunHeight, rainSpeed, thickFog
        };
    }
};

} // namespace libvotv::game

// Credit: MrDrNose
// Credit: UE4SS
// Credit: ModestImpala