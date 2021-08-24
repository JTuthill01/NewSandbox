#pragma once

UENUM(BlueprintType)
enum class EHasWeapon : uint8 { EHW_NoWeapon UMETA(DisplayName = "No Weapon"), EHW_HasWeapon UMETA(DisplayName = "Has Weapon") };

UENUM(BlueprintType)
enum class EWeaponSlot : uint8 { EWS_First_Slot UMETA(DisplayName = "First Slot"), EWS_Second_Slot UMETA(DisplayName = "Second Slot"), EWS_Third_Slot UMETA(DisplayName = "Third Slot") };

UENUM(BlueprintType)
enum class EWeaponName : uint8 { EWN_TT33 UMETA(DisplayName = "TT33"), EWN_AK47 UMETA(DisplayName = "AK47"), EWN_AmericanShotgun UMETA(DisplayName = "American Shotgun"), EWN_BelgianAR UMETA(DisplayName = "Belgian AR"),
	EWN_GermanSMG UMETA(DisplayName = "German SMG"), EWN_HandCannon UMETA(DisplayName = "Hand Cannon"), EWN_SKS UMETA(DisplayName = "SKS"), EWN_XM82 UMETA(DisplayName = "XM82"), EWN_Bulldog UMETA(DisplayName = "Bulldog"),
	EWN_L86 UMETA(DisplayName = "L86"), EWN_AK74 UMETA(DisplayName = "AK74"), EWN_M4A1 UMETA(DisplayName = "M4A1"), EWN_NavySMG UMETA(DisplayName = "Navy SMG"), EWN_ItalianShotgun UMETA(DisplayName = "Italian"),
	EWN_SVD UMETA(DisplayName = "SVD"), EWN_ShortStrokeAR UMETA(DisplayName = "ShortStrokeAR") };

UENUM(BlueprintType)
enum class EWeaponClass : uint8 { EWC_Pistol UMETA(DisplayName = "Pistol"), EWC_SemiAutoRifle UMETA(DisplayName = "Semi Auto Rifle"), EWC_FullAutoRifle UMETA(DisplayName = "Full Auto Rifle"), 
	EWC_Shotgun UMETA(DisplayName = "Shotgun") };

UENUM(BlueprintType)
enum class EFireType : uint8 { EFT_Hitscan UMETA(DisplayName = "Hitscan"), EFT_Projectile UMETA(DisplayName = "Projectile") };

UENUM(BlueprintType)
enum class EShotgunShellType : uint8 { ESST_None UMETA(DisplayName = "None"), ESST_OOOBuckShot UMETA(DisplayName = "OOOBuckShot"), ESST_OOBuckShot UMETA(DisplayName = "OOBuckShot"), 
	ESST_OBuckShot UMETA(DisplayName = "OBuckShot"), ESST_NumberOneBuckShot UMETA(DisplayName = "NumberOneBuckShot") };

enum EShoutgunReload : uint8 { ESR_ReloadStart, ESR_ReloadLoop, ESR_ReloadEnd };