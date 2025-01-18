#pragma once
#include <Unreal/AActor.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/FString.hpp>

#include "AGameMode.hpp"
#include "StructUtil.hpp"

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

    
};

struct PropStruct {
 
}; // Size: 0x7E

// Main Game Mode class
class GameMode : public RC::Unreal::AGameMode {
public:
    // Direct field access
    UE4SS_FIELD(MainPlayer*, mainPlayer);
    UE4SS_FIELD(SaveSlot*, saveSlot);
    UE4SS_FIELD(bool, isHalloween);
    UE4SS_FIELD(DayNightCycle*, daynightCycle);
    UE4SS_FIELD(GameInst*, gameInstance);
    
    // Player state fields
    UE4SS_FIELD(float, sleepTime);
    UE4SS_FIELD(bool, hasVirus);
    UE4SS_FIELD(bool, isSleeping);
    UE4SS_FIELD(bool, isDreaming);
    UE4SS_FIELD(bool, IsFlying);
    UE4SS_FIELD(bool, isInMainMenu);
    UE4SS_FIELD(bool, backroomsEnabled);
    UE4SS_FIELD(bool, canDamage);
    UE4SS_FIELD(bool, isImmortal);

    // Power system fields
    UE4SS_FIELD(float, powerCalculating);
    UE4SS_FIELD(float, powerDownloading);
    UE4SS_FIELD(float, powerCoordinates);
    UE4SS_FIELD(float, powerPlaying);
    UE4SS_FIELD(float, powerLight);
    UE4SS_FIELD(float, totalPower);
    UE4SS_FIELD(float, usedPower);
    UE4SS_FIELD(float, powerRatio);

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
    UE4SS_FIELD(bool, underwater);
    UE4SS_FIELD(bool, inWater);
    UE4SS_FIELD(bool, noclip);
    UE4SS_FIELD(bool, crouching);
    UE4SS_FIELD(bool, dead);
    UE4SS_FIELD(float, air);
    UE4SS_FIELD(float, foodDrain);
    UE4SS_FIELD(float, sleepDrain);
    UE4SS_FIELD(bool, mirror);
    UE4SS_FIELD(bool, combat);
    UE4SS_FIELD(bool, hasGlasses);
    UE4SS_FIELD(bool, hasFlashlight);
    UE4SS_FIELD(GameMode*, gameMode);
    UE4SS_FIELD(AActor*, pickup_actor);
    UE4SS_FIELD(AActor*, holdObject);
    UE4SS_FIELD(Hook*, activeHook);
    UE4SS_FIELD(AActor*, droppedItem);
    UE4SS_FIELD(AActor*, sittingOn);
    UE4SS_FIELD(UObject*, sitOnComponent);
    UE4SS_FIELD(UObject*, sitPlace);
    UE4SS_FIELD(AActor*, lookAtActor);

    UE4SS_FIELD(float, animTimer);
    UE4SS_FIELD(float, defSpeed);
    UE4SS_FIELD(bool, input_run);
    UE4SS_FIELD(bool, input_alt);
    UE4SS_FIELD(float, armLength);
    UE4SS_FIELD(bool, flashlight);
    UE4SS_FIELD(bool, isLookAt);
    UE4SS_FIELD(float, mouseSens);
    UE4SS_FIELD(float, inpF_forward);
    UE4SS_FIELD(float, inpF_back);
    UE4SS_FIELD(float, inpF_rig);
    UE4SS_FIELD(float, inpF_lef);
    UE4SS_FIELD(bool, input_rotate);
    UE4SS_FIELD(float, camSpeed);
    UE4SS_FIELD(bool, grabsHeavy);
    UE4SS_FIELD(bool, s_invertX);
    UE4SS_FIELD(bool, s_invertY);
    UE4SS_FIELD(float, kickTime);
    UE4SS_FIELD(bool, isKicked);
    UE4SS_FIELD(bool, canKick);
    UE4SS_FIELD(bool, isRagdoll);
    UE4SS_FIELD(bool, hulkMode);
    UE4SS_FIELD(bool, arcade);
    UE4SS_FIELD(bool, input_jump);
    UE4SS_FIELD(bool, onWater);
    UE4SS_FIELD(float, reload);
    UE4SS_FIELD(bool, input_fire);
    UE4SS_FIELD(bool, input_crouch);
    UE4SS_FIELD(RC::Unreal::FName, holdName);
    UE4SS_FIELD(WeaponStruct, weapon);
    UE4SS_FIELD(float, burningTime);
    UE4SS_FIELD(bool, deactivateMouseInput);
    UE4SS_FIELD(bool, fallTp);
    UE4SS_FIELD(float, underwaterDepth);
    UE4SS_FIELD(bool, isWakingUp);
    UE4SS_FIELD(bool, skipTolerance);
    UE4SS_FIELD(bool, isInFPBanimation);
    UE4SS_FIELD(float, headbobAnim);
    UE4SS_FIELD(float, headbobStrength);
    UE4SS_FIELD(float, headbobTiltStrength);
    UE4SS_FIELD(bool, noWakeup);
    UE4SS_FIELD(float, sleepDraining);
    UE4SS_FIELD(float, agil);
    UE4SS_FIELD(bool, isMirror);
    UE4SS_FIELD(bool, dropped);
    UE4SS_FIELD(bool, radialMenu);
    UE4SS_FIELD(bool, scubaAir);
    UE4SS_FIELD(bool, lastDroppedItem_deleted);
    UE4SS_FIELD(bool, canGetUp);
    UE4SS_FIELD(float, grab_speed);
    UE4SS_FIELD(bool, AutoRagdollGetup);
    UE4SS_FIELD(bool, calClimb);
    UE4SS_FIELD(bool, deactivateHeightRagdoll);
    UE4SS_FIELD(bool, velmaMode);
    UE4SS_FIELD(bool, crankFlashlight);


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
    UE4SS_FIELD(float, dist);
    UE4SS_FIELD(bool, attached_a);
    UE4SS_FIELD(bool, attached_b);
    UE4SS_FIELD(UObject*, component_A);
    UE4SS_FIELD(UObject*, component_B);
    UE4SS_FIELD(bool, isThrown);
    UE4SS_FIELD(UObject*, phys);
    UE4SS_FIELD(float, maxDist);
    UE4SS_FIELD(bool, dontDrop);
    UE4SS_FIELD(float, tensionLinear);
};

// Car (ATV); class
class Car : public RC::Unreal::AActor {
public:
    UE4SS_FIELD(bool, isDriving);
    UE4SS_FIELD(float, fuel);
    UE4SS_FIELD(bool, broken);
    UE4SS_FIELD(float, health);
    UE4SS_FIELD(bool, underwater);
    UE4SS_FIELD(bool, input_forward);
    UE4SS_FIELD(bool, input_back);
    UE4SS_FIELD(bool, input_right);
    UE4SS_FIELD(bool, input_left);
    UE4SS_FIELD(MainPlayer*, pl);
    UE4SS_FIELD(float, rotAlpha);
    UE4SS_FIELD(float, torqAlpha);
    UE4SS_FIELD(float, Speed);
    UE4SS_FIELD(bool, IsDrive);
    UE4SS_FIELD(bool, isDrive_sound);
    UE4SS_FIELD(bool, turbo);
    UE4SS_FIELD(bool, Empty);
    UE4SS_FIELD(bool, Brake);
    UE4SS_FIELD(float, diff_fuel);
    UE4SS_FIELD(bool, lights);
    UE4SS_FIELD(bool, brokenn);
    UE4SS_FIELD(bool, input_alt);
    UE4SS_FIELD(float, speed_turbo);
    UE4SS_FIELD(float, speed_default);
    UE4SS_FIELD(bool, nitro);
    UE4SS_FIELD(bool, invX);
    UE4SS_FIELD(bool, invY);
    UE4SS_FIELD(bool, isDriven);
    UE4SS_FIELD(bool, inWater);
    UE4SS_FIELD(bool, floater);
    UE4SS_FIELD(bool, trap);
    UE4SS_FIELD(bool, zapped);
    UE4SS_FIELD(float, airtime);
    UE4SS_FIELD(bool, IsInAir);
    UE4SS_FIELD(bool, landed);
    UE4SS_FIELD(GameMode*, gameMode);
    
};

// Kerfur Omega class
class KerfurOmega : public RC::Unreal::AActor {
public:
    UE4SS_FIELD(bool, sentient);
    UE4SS_FIELD(float, meow);
    UE4SS_FIELD(bool, skipFaceAssign);
};

// Base prop class
class Prop : public RC::Unreal::AActor {
public:
    UE4SS_FIELD(GameMode*, gameMode);
    UE4SS_FIELD(RC::Unreal::FName, Name);
    UE4SS_FIELD(PropStruct, propData);
    UE4SS_FIELD(bool, Static);
    UE4SS_FIELD(bool, removeWOrespawn);
    UE4SS_FIELD(bool, frozen);
    UE4SS_FIELD(bool, ignoreSav);
    UE4SS_FIELD(bool, ingoreFix);
    UE4SS_FIELD(bool, sleep);
    UE4SS_FIELD(bool, customShadows);
    UE4SS_FIELD(bool, returnLookAt);
    UE4SS_FIELD(bool, doCastShadow);
};

// Door class 
class Door : public RC::Unreal::AActor {
public:
    UE4SS_FIELD(bool, isOpened);
    UE4SS_FIELD(bool, isMoving);
    UE4SS_FIELD(bool, isActive);
    UE4SS_FIELD(bool, autoclose);
    UE4SS_FIELD(bool, jammed);
};

class GameInst : public RC::Unreal::UObject {
public:
    UE4SS_FIELD(SaveSlot*, save);
    UE4SS_FIELD(RC::Unreal::FString, SlotName);
    UE4SS_FIELD(bool, opened);
    UE4SS_ENUM_FIELD_NAME(GameModes::Type, GameMode, gameMode);
    // 0x01E2 - 0x01E3 is padding (2 bytes);
    UE4SS_FIELD(int32_t, startDay);
    UE4SS_FIELD(bool, isSaveReset);
    UE4SS_FIELD(bool, loadObjects);
    // 0x022A - 0x022F is padding (6 bytes);
    UE4SS_FIELD(RC::Unreal::FVector, locationDistance);
    UE4SS_FIELD(RC::Unreal::FName, subArea);
    UE4SS_FIELD(bool, antibreatherKickout);
    // 0x0265 - 0x0267 is padding (3 bytes);
    UE4SS_FIELD(RC::Unreal::FString, dwString);
    UE4SS_FIELD(RC::Unreal::FString, dwString_2);
    UE4SS_FIELD(GameMode*, gamemode_);
    UE4SS_FIELD(bool, NewVar_0);
    
    GameInst() = default;
};

class SaveSlot : public RC::Unreal::UObject {
public:
    UE4SS_FIELD(float, totalTime);
    UE4SS_FIELD(float, food);
    UE4SS_FIELD(float, sleep);
    UE4SS_FIELD(int32_t, Points);
    UE4SS_FIELD(float, health);
    UE4SS_FIELD(float, p_health);
    UE4SS_FIELD(float, Day);
    UE4SS_FIELD(float, maxHealth);
};


// Day/Night Cycle class
class DayNightCycle : public RC::Unreal::AActor {
public:
    // Time fields  
    UE4SS_INT_VECTOR_FIELD(timeZ);;
    // Create easy access to Z component which represents the day
    int32_t current_day() const { return get_timeZ_z();; }

 UE4SS_FIELD(int32_t, timeZ_Z);
    UE4SS_FIELD(float, day);
    UE4SS_FIELD(float, phase);
    UE4SS_FIELD(float, phaseSin);
    UE4SS_FIELD(float, phaseNormSin);
    UE4SS_FIELD(float, maxTime);
    UE4SS_FIELD(float, totalTime);
    UE4SS_FIELD(float, timeScale);

    // Weather fields
    UE4SS_FIELD(bool, isActive);
    UE4SS_FIELD(bool, starActive);
    UE4SS_FIELD(float, rain);
    UE4SS_FIELD(bool, isRaining);
    UE4SS_FIELD(bool, rainMuted);
    UE4SS_FIELD(float, diffMult);
    UE4SS_FIELD(float, sunHeight);
    UE4SS_FIELD(float, rainSpeed);
    UE4SS_FIELD(float, thickFog);

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