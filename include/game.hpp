#pragma once
#include <Unreal/AActor.hpp>
#include <Unreal/UObject.hpp>
#include <Unreal/FString.hpp>
#include <Unreal/AActor.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UObjectArray.hpp>
#include <Unreal/UFunction.hpp>
#include <Unreal/World.hpp>
#include <Unreal/UScriptStruct.hpp>

#include "AGameMode.hpp"
#include "BPMacros.hpp"
#include "StructUtil.hpp"

namespace votv::game {
    using RC::Unreal::UFunction;
    using RC::Unreal::FMemory;
    using RC::Unreal::uint8;
    using RC::Unreal::FName;
    using RC::to_string;
    namespace UObjectGlobals = RC::Unreal::UObjectGlobals; 
// Forward declarations
class MainPlayer;
class SaveSlot;
class DayNightCycle;
class GameInst;
class GameMode;
class Hook;

// Game Modes enum (pa0081_0008)
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
    char padding_0[0x8];    //    class UAnimMontage* montage_9_4B16A04E4D5F5DF99428C190FE6D1295;                   // 0x0000 (size: 0x8)
    float damage;                               // 0x0008 (size: 0x4)
    float reload;                              // 0x000C (size: 0x4)
    float length;                               // 0x0010 (size: 0x4)
    float force;                            // 0x0014 (size: 0x4)
    bool attack;                               // 0x0018 (size: 0x1)
    char padding_1[0x7];                                                              // 0x0019 (size: 0x7)
    char padding_2[0x10];      // TArray<class UPhysicalMaterial*> matEff_21_184D7DC9414F64A7AE914FBDB6ED92DC;      // 0x0020 (size: 0x10)
    //TArray<float> matEffDmg;                      // 0x0030 (size: 0x10)
};

struct PropStruct {
     class UStaticMesh* mesh;                     // 0x0000 (size: 0x8)
     RC::Unreal::FText displayName;                         // 0x0008 (size: 0x18)
    RC::Unreal::FText description;                     // 0x0020 (size: 0x18)
    char padding[0x1];  //Enum<enum_spawnmenuCats::Type> category_42_B50473484322DA921629D9BE91DB63EC; // 0x0038 (size: 0x1)
    char padding_0[0x3];                                                              // 0x0039 (size: 0x3)
    float massMultiply;                      // 0x003C (size: 0x4)
    bool canPickup;                         // 0x0040 (size: 0x1)
    bool heavy;                         // 0x0041 (size: 0x1)
    bool ignoreInteractions;                  // 0x0042 (size: 0x1)
    bool staticInteract;                    // 0x0043 (size: 0x1)
    float dragForce;                          // 0x0044 (size: 0x4)
     RC::Unreal::int32 price;                                 // 0x0048 (size: 0x4)
    RC::Unreal::FName achievement_unlock;              // 0x004C (size: 0x8)
    bool hidden;                                 // 0x0054 (size: 0x1)
    char padding_1[0x3];                                                              // 0x0055 (size: 0x3)
     RC::Unreal::TSubclassOf<class AActor> spawnAsObject;      // 0x0058 (size: 0x8)
    bool spoiler;                             // 0x0060 (size: 0x1)
    char padding_2[0x7];                                                              // 0x0061 (size: 0x7)
    RC::Unreal::FString craftTag;                       // 0x0068 (size: 0x10)
    float volumeMultiply;                   // 0x0078 (size: 0x4)
    bool parseNameToObject;               // 0x007C (size: 0x1)
    bool canCollect;                          // 0x007D (size: 0x1)
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