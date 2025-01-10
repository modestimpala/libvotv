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
class Hook;

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

struct WeaponStruct {
    FIELD(0x0008, float, damage);                         // Base weapon damage
    FIELD(0x000C, float, reload);                         // Reload time in seconds
    FIELD(0x0010, float, length);                         // Weapon length/range
    FIELD(0x0014, float, force);                          // Impact force
    FIELD(0x0018, bool, attack);                          // Attack state
    
};

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
    FIELD(0x0A80, float, foodDrain);
    FIELD(0x0A84, float, sleepDrain);
    FIELD(0x0A90, bool, mirror);
    FIELD(0x0BC0, bool, combat);
    FIELD(0x0BC1, bool, hasGlasses);
    FIELD(0x0BC2, bool, hasFlashlight);
    FIELD(0x0B80, GameMode*, gameMode);
    FIELD(0x06D0, AActor*, pickup_actor);
    FIELD(0x0920, AActor*, holdObject);
    FIELD(0x0928, Hook*, activeHook);
    FIELD(0x0958, AActor*, droppedItem);
    FIELD(0x0980, AActor*, sittingOn);
    FIELD(0x0988, UObject*, sitOnComponent); // Base UObject for sit stuff until implemented
    FIELD(0x0990, UObject*, sitPlace);
    FIELD(0x09A0, AActor*, lookAtActor);

    FIELD(0x06EC, float, animTimer);
    FIELD(0x06F0, float, defSpeed);
    FIELD(0x06F1, bool, input_run);
    FIELD(0x0719, bool, input_alt);
    FIELD(0x0720, float, armLength);
    FIELD(0x0739, bool, flashlight);
    FIELD(0x0749, bool, isLookAt);
    FIELD(0x0750, float, mouseSens);
    FIELD(0x0754, float, inpF_forward);
    FIELD(0x0758, float, inpF_back);
    FIELD(0x075C, float, inpF_rig);
    FIELD(0x0760, float, inpF_lef);
    FIELD(0x0761, bool, input_rotate);
    FIELD(0x0774, float, camSpeed);
    FIELD(0x0775, bool, grabsHeavy);
    FIELD(0x0776, bool, s_invertX);
    FIELD(0x0777, bool, s_invertY);
    FIELD(0x077C, float, kickTime);
    FIELD(0x077D, bool, isKicked);
    FIELD(0x077E, bool, canKick);
    FIELD(0x0780, bool, isRagdoll);
    FIELD(0x0821, bool, hulkMode);
    FIELD(0x0831, bool, arcade);
    FIELD(0x08A2, bool, input_jump);
    FIELD(0x08B9, bool, onWater);
    FIELD(0x091C, float, reload);
    FIELD(0x091D, bool, input_fire);
    FIELD(0x08BD, bool, input_crouch);
    FIELD(0x0938, RC::Unreal::FName, holdName);
    FIELD(0x0918, WeaponStruct, weapon);
    FIELD(0x09C8, float, burningTime);
    FIELD(0x09F9, bool, deactivateMouseInput);
    FIELD(0x0A21, bool, fallTp);
    FIELD(0x0A2C, float, underwaterDepth);
    FIELD(0x0A49, bool, isWakingUp);
    FIELD(0x0A4A, bool, skipTolerance);
    FIELD(0x0A4B, bool, isInFPBanimation);
    FIELD(0x0A50, float, headbobAnim);
    FIELD(0x0A54, float, headbobStrength);
    FIELD(0x0A58, float, headbobTiltStrength);
    FIELD(0x0A69, bool, noWakeup);
    FIELD(0x0A88, float, sleepDraining);
    FIELD(0x0A8C, float, agil);
    FIELD(0x0A91, bool, isMirror);
    FIELD(0x0A92, bool, dropped);
    FIELD(0x0AA1, bool, radialMenu);
    FIELD(0x0AC1, bool, scubaAir);
    FIELD(0x0B59, bool, lastDroppedItem_deleted);
    FIELD(0x0B5A, bool, canGetUp);
    FIELD(0x0B60, float, grab_speed);
    FIELD(0x0B6C, bool, AutoRagdollGetup);
    FIELD(0x0B79, bool, calClimb);
    FIELD(0x0BA1, bool, deactivateHeightRagdoll);
    FIELD(0x0BC4, bool, velmaMode);
    FIELD(0x0BC5, bool, crankFlashlight);


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
};

class Hook : public RC::Unreal::AActor
{
    FIELD(0x0290, float, dist);
    FIELD(0x0294, bool, attached_a);
    FIELD(0x0295, bool, attached_b);
    FIELD(0x0298, UObject*, component_A);
    FIELD(0x02A0, UObject*, component_B);
    FIELD(0x02A8, bool, isThrown);
    FIELD(0x02B8, UObject*, phys);
    FIELD(0x02C0, float, maxDist);
    FIELD(0x02D0, bool, dontDrop);
    FIELD(0x02D4, float, tensionLinear);
};

// Car (ATV) class
class Car : public RC::Unreal::AActor {
public:
    FIELD(0x0468, bool, isDriving);
    FIELD(0x046C, float, fuel);
    FIELD(0x0481, bool, broken);
    FIELD(0x047C, float, health);
    FIELD(0x04C8, bool, underwater);
    FIELD(0x0441, bool, input_forward);
    FIELD(0x0442, bool, input_back);
    FIELD(0x0443, bool, input_right);
    FIELD(0x0444, bool, input_left);
    FIELD(0x0450, MainPlayer*, pl);
    FIELD(0x0454, float, rotAlpha);
    FIELD(0x0458, float, torqAlpha);
    FIELD(0x045C, float, Speed);
    FIELD(0x0469, bool, IsDrive);
    FIELD(0x046A, bool, isDrive_sound);
    FIELD(0x046B, bool, turbo);
    FIELD(0x0471, bool, Empty);
    FIELD(0x0472, bool, Brake);
    FIELD(0x0478, float, diff_fuel);
    FIELD(0x0479, bool, lights);
    FIELD(0x0482, bool, brokenn);
    FIELD(0x0483, bool, input_alt);
    FIELD(0x0488, float, speed_turbo);
    FIELD(0x048C, float, speed_default);
    FIELD(0x048D, bool, nitro);
    FIELD(0x048E, bool, invX);
    FIELD(0x048F, bool, invY);
    FIELD(0x0490, bool, isDriven);
    FIELD(0x04E1, bool, inWater);
    FIELD(0x04E2, bool, floater);
    FIELD(0x04E3, bool, trap);
    FIELD(0x0509, bool, zapped);
    FIELD(0x0510, float, airtime);
    FIELD(0x0511, bool, IsInAir);
    FIELD(0x0512, bool, landed);
    FIELD(0x0520, GameMode*, gameMode);
    
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