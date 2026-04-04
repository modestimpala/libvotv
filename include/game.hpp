#pragma once
#include "AGameMode.hpp"
#include "StructUtil.hpp"

namespace votv::game {
    using RC::Unreal::UFunction;
    using RC::Unreal::FMemory;
    using RC::Unreal::uint8;
    using RC::Unreal::FName;

    // Forward declarations
    class MainPlayer;
    class SaveSlot;
    class DayNightCycle;
    class GameInstance;
    class GameMode;
    class ATV;
    class Prop_Hook;
    class Bed;
    class Grime;
    class GrowingPlant;
    class ServerBox;
    class SitBox;
    class Padlock;
    class ToolGun;
    class VidCam;
    class Workbench;
    class ATM;

    // Game Modes enum (0.9.0)
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

    // Seasons enum (new in 0.9.0)
    namespace Seasons {
        enum Type {
            Summer = 0,
            Autumn = 1,
            Winter = 2,
            Spring = 3
        };
    }

    // Tool categories (new in 0.9.0)
    namespace ToolCategories {
        enum Type {
            Misc = 0,
            Build = 1,
            Physics = 2,
            Rendering = 3,
            Fun = 4,
            Environment = 5,
            Debug = 6
        };
    }

    // Tool parameter types (new in 0.9.0)
    namespace ToolParameterTypes {
        enum Type {
            Bools = 0,
            Bytes = 1,
            Integers = 2,
            Floats = 3,
            Strings = 4,
            Vectors = 5,
            Rotators = 6,
            Classes = 7
        };
    }

    // Farm plant types (new in 0.9.0)
    namespace FarmPlantTypes {
        enum Type {
            Grass = 0,
            Bush = 1,
            Tree = 2
        };
    }

    // Photographic entity types (new in 0.9.0)
    namespace PhotographicTypes {
        enum Type {
            None = 0,
            Common = 1,
            Rare = 2,
            Spooky = 3
        };
    }

    // Hotkey actions (new in 0.9.0)
    namespace HotkeyActions {
        enum Type {
            None = 0,
            Grab = 1,
            DropPlace = 2,
            HoldInHand = 3,
            PutInInventory = 4,
            Throw = 5,
            PrecisionPlacement = 6,
            Use = 7,
            UseSelectedAction = 8,
            ActionList = 9
        };
    }

    // Trash types (new in 0.9.0)
    namespace TrashTypes {
        enum Type {
            Empty = 0,
            Plastic = 1,
            Paper = 2,
            Metal = 3,
            Rubber = 4,
            Elec = 5,
            Glass = 6,
            Wood = 7,
            Organic = 8
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
        char padding[0x1];  //Enum<enum_spawnmenuTabs::Type> category;                    // 0x0038 (size: 0x1)
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
        RC::Unreal::UObject spawnAsObject;      // 0x0058 (size: 0x8)
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
        UE4SS_FIELD(GameInstance*, gameInstance);

        // Player state fields
        UE4SS_FIELD(float, sleepTime);
        UE4SS_FIELD(bool, hasVirus);
        UE4SS_FIELD(bool, isSleep);
        UE4SS_FIELD(bool, isDreaming);
        UE4SS_FIELD(bool, IsFlying);
        UE4SS_FIELD(bool, isInMainMenu);
        UE4SS_FIELD(bool, backroomsEnabled);
        UE4SS_FIELD(bool, canDamage);
        UE4SS_FIELD(bool, isImmortal);

        // Renamed fields (0.9.0)
        UE4SS_FIELD(bool, isWinter);              // was: snow
        UE4SS_ENUM_FIELD(Seasons::Type, currentSeason);
        UE4SS_FIELD(bool, isSummer);
        UE4SS_FIELD(bool, isAutumn);
        UE4SS_FIELD(bool, isSpring);

        // Difficulty (renamed from diff)
        UE4SS_FIELD(RC::Unreal::uint8, difficulty);

        // Dishes (renamed)
        // dishes -> dishes_names (TArray<FText>)
        // dishes_str -> dishes_techNames (TArray<FString>)

        // Dream system
        UE4SS_FIELD(float, dreamProbability);

        // New fields (0.9.0)
        UE4SS_FIELD(bool, fridgeIsOpen);
        UE4SS_FIELD(bool, craftRecipeUnrolled);
        UE4SS_FIELD(bool, april1st);
        UE4SS_FIELD(bool, isInGayBabyJail);
        UE4SS_FIELD(bool, unlockFungunsInStory);
        UE4SS_FIELD(int32_t, tps);

        // Events & state
        UE4SS_FIELD(bool, apoc);
        UE4SS_FIELD(bool, arirFeeding);
        UE4SS_FIELD(bool, isChristmas);
        UE4SS_FIELD(bool, editor);
        UE4SS_FIELD(int32_t, activeEvents);

        // Entity pointers (non-null = event active)
        UE4SS_FIELD(AActor*, evil);
        UE4SS_FIELD(AActor*, blackFog);
        UE4SS_FIELD(AActor*, redSky);
        UE4SS_FIELD(AActor*, badSun);

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
        UE4SS_FIELD(bool, underwaterPlayer);      // was: underwater
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
        UE4SS_FIELD(AActor*, grabbing_actor);     // was: pickup_actor
        UE4SS_FIELD(AActor*, holding_actor);      // was: holdObject
        UE4SS_FIELD(RC::Unreal::FName, holding_name); // was: holdName
        UE4SS_FIELD(Prop_Hook*, activeHook);
        UE4SS_FIELD(AActor*, droppedItem);
        UE4SS_FIELD(AActor*, sittingOn);
        UE4SS_FIELD(UObject*, sitOnComponent);
        UE4SS_FIELD(UObject*, sitPlace);
        UE4SS_FIELD(AActor*, lookAtActor);
        UE4SS_FIELD(ATV*, atv);  // non-null when driving ATV

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
        UE4SS_FIELD(WeaponStruct, weapon);
        UE4SS_FIELD(float, burningTime);
        UE4SS_FIELD(bool, deactivateMouseInput);
        UE4SS_FIELD(bool, fallTp);
        UE4SS_FIELD(float, playerDepth);          // was: underwaterDepth
        UE4SS_FIELD(bool, isWakingUp);
        UE4SS_FIELD(bool, skipFatigue);            // was: skipTolerance
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

        // Equipment & status
        UE4SS_FIELD(float, irradiation);
        UE4SS_FIELD(int32_t, scuba);
        UE4SS_FIELD(AActor*, cig);
        UE4SS_FIELD(bool, equipped_emf);
        UE4SS_FIELD(bool, equipped_geiger);
        UE4SS_FIELD(bool, equipped_krampushat);

        // New fields (0.9.0)
        UE4SS_FIELD(float, sleepComfort);
        UE4SS_FIELD(float, holdDelayInit);
        UE4SS_FIELD(float, holdDelay);
        UE4SS_FIELD(float, defaultEyeHeight);
        UE4SS_FIELD(bool, isFallDamageActive);
        UE4SS_FIELD(float, batteryUsage);
        UE4SS_FIELD(bool, input_drop);
        UE4SS_FIELD(bool, hasDropped);
        UE4SS_FIELD(bool, drop_place);
        UE4SS_FIELD(bool, drop_dontCollect);      // was: dontcollect
        UE4SS_FIELD(bool, grabHeavy_pull);
        UE4SS_FIELD(float, worldDeltaSeconds);
        UE4SS_FIELD(bool, isBurning);
        UE4SS_FIELD(bool, isDrawThrowPath);
        UE4SS_FIELD(bool, Heavy);
        UE4SS_FIELD(bool, lookAtCenter);
        UE4SS_FIELD(float, pooped);
        UE4SS_FIELD(bool, tutorialItems);
        UE4SS_FIELD(bool, isLookingAt);
        UE4SS_FIELD(bool, eatSounds);
        UE4SS_FIELD(bool, recentlyJumped);
        UE4SS_FIELD(bool, isExhausted_bool);
        UE4SS_FIELD(bool, isActiveINterface3D);
        UE4SS_FIELD(float, viewmodelBob);
        UE4SS_FIELD(float, waterDepth);
        UE4SS_FIELD(bool, underwaterListener);
        UE4SS_FIELD(bool, enableWaterFallDamage);

        // Convenient struct wrapper
        struct PlayerState {
            bool inWater;
            bool underwaterPlayer;
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
                this->underwaterPlayer,
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

    class Prop_Hook : public RC::Unreal::AActor
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

    // ATV class (renamed from Car/car1 in 0.9.0)
    class ATV : public RC::Unreal::AActor {
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
        UE4SS_FIELD(MainPlayer*, player);         // was: pl
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

        // New fields (0.9.0)
        UE4SS_FIELD(float, battery);
        UE4SS_FIELD(float, dirt);
        UE4SS_FIELD(float, deltaSeconds);
        UE4SS_FIELD(float, energyWaste);
        UE4SS_FIELD(float, exhaustForce);
        UE4SS_FIELD(bool, hasSolar);
        UE4SS_FIELD(bool, hasBelt);
        UE4SS_FIELD(bool, hasContainer);
        UE4SS_FIELD(bool, hasBigLights);
        UE4SS_FIELD(bool, hasGuns);
        UE4SS_FIELD(bool, hasFloaties);
        UE4SS_FIELD(bool, hasMap);
        UE4SS_FIELD(bool, hasRadio);
        UE4SS_FIELD(bool, hasAircontrol);
        UE4SS_FIELD(bool, hasFly);
        UE4SS_FIELD(bool, hasChargedEngine);
        UE4SS_FIELD(bool, has_alternator);
        UE4SS_FIELD(bool, hasSpareTire);
        UE4SS_FIELD(bool, mouseSteering);
        UE4SS_FIELD(bool, bodyIsOnTheGround);
        UE4SS_FIELD(bool, april1st);
        UE4SS_FIELD(RC::Unreal::FName, key);
    };

    // Backwards compatibility alias
    using Car = ATV;

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

        // New fields (0.9.0)
        UE4SS_FIELD(RC::Unreal::FString, keycardName);
        UE4SS_FIELD(bool, cantClose);
        UE4SS_FIELD(bool, ignoreInteractions);
    };

    // Game Instance class (renamed from GameInst/gameInst in 0.9.0)
    class GameInstance : public RC::Unreal::UObject {
    public:
        UE4SS_FIELD(SaveSlot*, save_gameInst);    // was: save
        UE4SS_FIELD(RC::Unreal::FString, SlotName);
        UE4SS_FIELD(bool, opened);
        UE4SS_ENUM_FIELD_NAME(GameModes::Type, GameMode, gameMode);
        UE4SS_FIELD(int32_t, startDay);
        UE4SS_FIELD(bool, isSaveReset);
        UE4SS_FIELD(bool, loadObjects);
        UE4SS_FIELD(RC::Unreal::FVector, locationDistance);
        UE4SS_FIELD(RC::Unreal::FName, subArea);
        UE4SS_FIELD(bool, antibreatherKickout);
        UE4SS_FIELD(RC::Unreal::FString, dwString);
        UE4SS_FIELD(RC::Unreal::FString, dwString_2);
        UE4SS_FIELD(GameMode*, gamemode_);
        UE4SS_FIELD(bool, NewVar_0);

        // New fields (0.9.0)
        // gameRules (struct Fstruct_gameRules) - access individual rules via property name
        UE4SS_FIELD(bool, RHIget);

        GameInstance() = default;
    };

    // Backwards compatibility alias
    using GameInst = GameInstance;

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
        UE4SS_FIELD(int32_t, totalCompletedTaskParts); // was: completes

        // Additional stats
        UE4SS_FIELD(float, arirReputation);
        UE4SS_FIELD(float, coffeePower);
        UE4SS_FIELD(bool, hasFailedWeek);
        UE4SS_FIELD(float, moonPhase);
        UE4SS_FIELD(float, battery);
    };


    // Day/Night Cycle class
    class DayNightCycle : public RC::Unreal::AActor {
    public:
        // Time fields
        UE4SS_INT_VECTOR_FIELD(timeZ);
        // Create easy access to Z component which represents the day
        int32_t current_day() const { return get_timeZ_z(); }

        UE4SS_FIELD(int32_t, timeZ_Z);
        UE4SS_FIELD(float, day);
        UE4SS_FIELD(float, phase);
        UE4SS_FIELD(float, phaseSin);
        UE4SS_FIELD(float, phaseNormSin);
        UE4SS_FIELD(float, maxTime);
        UE4SS_FIELD(float, totalTime);
        UE4SS_FIELD(float, timeScale);
        UE4SS_FIELD(float, sleepingTimeDilation);

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

        // New weather fields (0.9.0)
        UE4SS_FIELD(float, rainLightningChance);
        UE4SS_FIELD(float, rainDeactivateChance);
        UE4SS_FIELD(float, rainProbability);
        UE4SS_FIELD(float, fogProbability);
        UE4SS_FIELD(float, seasonalExponent);
        UE4SS_FIELD(float, phase_nsin);
        UE4SS_FIELD(float, rainStrength);
        UE4SS_FIELD(float, finalFogDensity);
        UE4SS_FIELD(float, rainWindSpeed);
        UE4SS_FIELD(bool, rainToggle);
        UE4SS_FIELD(bool, permanentRain);
        UE4SS_FIELD(bool, permanentFog);
        UE4SS_FIELD(bool, isHalloween);
        UE4SS_FIELD(bool, enableSunlight);        // was: disableSunlight (inverted)

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
            float sleepingTimeDilation;
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
            float rainStrength;
            bool permanentRain;
            bool permanentFog;
        };


        TimeInfo GetTimeInfo() const {
            return {
                current_day(), day, phase, phaseSin, phaseNormSin,
                maxTime, totalTime, timeScale, sleepingTimeDilation
            };
        }

        WeatherInfo GetWeatherInfo() const {
            return {
                isActive, starActive, rain, isRaining, rainMuted,
                diffMult, sunHeight, rainSpeed, thickFog,
                rainStrength, permanentRain, permanentFog
            };
        }
    };

    // =========================================================================
    // New classes (0.9.0)
    // =========================================================================

    // Bed class (new typed class, was AActor* in 0.8.11)
    class Bed : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(bool, dropItem);
        UE4SS_FIELD(float, dreamProb);
        UE4SS_FIELD(float, comfort);
        UE4SS_FIELD(RC::Unreal::FName, Name);
    };

    // Grime class (replaces grunge system in 0.9.0)
    class Grime : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(int32_t, type);
        UE4SS_FIELD(float, process);
        UE4SS_FIELD(RC::Unreal::FName, cleanParameter);
        UE4SS_FIELD(float, maxProcess);
        UE4SS_FIELD(float, cleanStrength);
        UE4SS_FIELD(bool, randomOrientation);
        UE4SS_FIELD(bool, snapOrientation);
        UE4SS_FIELD(bool, isCleanable);
        UE4SS_FIELD(bool, snap);
        UE4SS_FIELD(bool, resistRain);
    };

    // Growing plant base class (new in 0.9.0)
    class GrowingPlant : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(RC::Unreal::FName, plant);
        UE4SS_FIELD(float, growRate);
        UE4SS_FIELD(float, grow);
        UE4SS_FIELD(int32_t, amount);
        UE4SS_FIELD(float, fertilizer);
        UE4SS_FIELD(float, water);
        UE4SS_FIELD(bool, discard);
        UE4SS_FIELD(float, fruitGrow);
        UE4SS_FIELD(bool, hideDirt);
        UE4SS_FIELD(bool, grew);
        UE4SS_FIELD(float, dryness);
        UE4SS_FIELD(float, sunAmount);
        UE4SS_FIELD(float, deltaSeconds);
        UE4SS_FIELD(RC::Unreal::FName, attachParent);
    };

    // Server box class (new in 0.9.0)
    class ServerBox : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(RC::Unreal::FString, name);
        UE4SS_FIELD(bool, active);
        UE4SS_FIELD(bool, calc);
        UE4SS_FIELD(int32_t, floppyType);
        UE4SS_FIELD(int32_t, floppyReadwrites);
        UE4SS_FIELD(bool, isBroken);
        UE4SS_FIELD(int32_t, type);
        UE4SS_FIELD(bool, resisnant);
        UE4SS_FIELD(float, resTime);
        UE4SS_FIELD(int32_t, dishIndex);
        UE4SS_FIELD(bool, isSuper);
        UE4SS_FIELD(bool, damaged);
        UE4SS_FIELD(RC::Unreal::FString, floppyObjectData);
        UE4SS_FIELD(bool, upgrade_1);
        UE4SS_FIELD(bool, upgrade_2);
        UE4SS_FIELD(bool, upgrade_3);
        UE4SS_FIELD(int32_t, minigame);
        UE4SS_FIELD(bool, staticMinigame);
        UE4SS_FIELD(int32_t, upgrades);
        UE4SS_FIELD(bool, lookatFloppyButton);
    };

    // Sit box class (new in 0.9.0)
    class SitBox : public RC::Unreal::AActor {
    public:
        UE4SS_VECTOR_FIELD(location);
        UE4SS_FIELD(float, distance);
        UE4SS_FIELD(bool, enableIcon);
        UE4SS_FIELD(float, getoffAngle);
    };

    // Padlock class (new in 0.9.0)
    class Padlock : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(float, radius);
        UE4SS_VECTOR_FIELD(grabOffset);
        UE4SS_FIELD(RC::Unreal::FName, Name);
        UE4SS_FIELD(AActor*, lock);
        UE4SS_FIELD(bool, grabNearest);
    };

    // Tool gun class (new in 0.9.0)
    class ToolGun : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(RC::Unreal::FName, Name);
        UE4SS_FIELD(RC::Unreal::FName, activeTool);
        UE4SS_FIELD(MainPlayer*, player);
        UE4SS_FIELD(AActor*, selectObject);
        UE4SS_FIELD(bool, isRMB);
    };

    // Video camera class (new in 0.9.0)
    class VidCam : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(float, battery);
        UE4SS_FIELD(int32_t, quality);
        UE4SS_FIELD(float, imageQuality);
        UE4SS_FIELD(int32_t, videoBitrate);
        UE4SS_FIELD(int32_t, audioBitrate);
        UE4SS_FIELD(int32_t, audioSampleRate);
        UE4SS_FIELD(int32_t, fps);
        UE4SS_FIELD(RC::Unreal::FName, Name);
        UE4SS_FIELD(float, microwaved);
    };

    // Workbench class (new in 0.9.0)
    class Workbench : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(RC::Unreal::FName, Name);
        UE4SS_FIELD(bool, isCrafting);
        UE4SS_FIELD(bool, canCraft);
        UE4SS_FIELD(RC::Unreal::FString, currentBlueprint);
        UE4SS_FIELD(bool, scrapCraft);
        UE4SS_FIELD(RC::Unreal::FName, bpName);
        UE4SS_FIELD(bool, isReverse);
    };

    // ATM class (new in 0.9.0)
    class ATM : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(bool, lookingAt);
        UE4SS_FIELD(bool, isBusy);
        UE4SS_FIELD(RC::Unreal::FName, billType);
        UE4SS_FIELD(int32_t, billAmount);
    };

    // Sleeping bag class (new in 0.9.0)
    class SleepingBag : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(RC::Unreal::FName, Name);
        UE4SS_FIELD(bool, unwrapOnBed);
    };

    // Whiteboard class (new in 0.9.0)
    class Whiteboard : public RC::Unreal::AActor {
    public:
        UE4SS_FIELD(RC::Unreal::FName, Name);
        UE4SS_FIELD(RC::Unreal::FString, str);
    };

} // namespace votv::game

// Credit: MrDrNose
// Credit: UE4SS
// Credit: ModestImpala