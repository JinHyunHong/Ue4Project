#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CActionComponent.h"
#include "Characters/CActionCharacter.h"
#include "Engine/DataTable.h"
#include "CEquipCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacter : uint8
{
	Man, Max
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	// None -> Item
	None, Chest, Pants, Boots, Helmet, LeftBracer, RightBracer, Cloak, Gloves, Robes, Shoulders, Weapon, Max
};

ENUM_RANGE_BY_COUNT(EEquipmentType, EEquipmentType::Max);

UENUM(BlueprintType)
enum class BootsType : uint8
{
	Open, Closed, 
};

UENUM(BlueprintType)
enum class BootsHeight : uint8
{
	Low, High, Slippers,
};

UENUM(BlueprintType)
enum class ChestType : uint8
{
	Open, Closed,
};

UENUM(BlueprintType)
enum class CloakType: uint8
{
	Narrow, Wide,
};

UENUM(BlueprintType)
enum class GlovesType : uint8
{
	Short, Long, Open,
};

UENUM(BlueprintType)
enum class RobeType : uint8
{
	ShortSleeve, LongSleeve,
};

UENUM(BlueprintType)
enum class SleeveType : uint8
{
	NoSleeve, LongSleeve,
};

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	Sword, Axe, DoubleAxe, Dagger, Rod,
};

USTRUCT(BlueprintType)
struct FCharacterMesh : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		ECharacter Character;

	UPROPERTY(EditAnywhere)
		class USkeletalMesh* FullMesh;

	UPROPERTY(EditAnywhere)
		class USkeletalMesh* HeadMesh;

	UPROPERTY(EditAnywhere)
		class USkeletalMesh* ChestMesh;

	UPROPERTY(EditAnywhere)
		class USkeletalMesh* ArmsMesh;

	UPROPERTY(EditAnywhere)
		class USkeletalMesh* HandsMesh;

	UPROPERTY(EditAnywhere)
		class USkeletalMesh* FeetMesh;

	UPROPERTY(EditAnywhere)
		class USkeletalMesh* LegsMesh;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UAnimInstance> AnimBP;
};

USTRUCT(BlueprintType)
struct FEquipmentDataTables : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		ECharacter Character;

	UPROPERTY(EditAnywhere)
		UDataTable* CloakTable;

	UPROPERTY(EditAnywhere)
		UDataTable* ShouldersTable;

	UPROPERTY(EditAnywhere)
		UDataTable* RobesTable;

	UPROPERTY(EditAnywhere)
		UDataTable* LeftBracerTable;

	UPROPERTY(EditAnywhere)
		UDataTable* RightBracerTable;

	UPROPERTY(EditAnywhere)
		UDataTable* GlovesTable;

	UPROPERTY(EditAnywhere)
		UDataTable* HelmetTable;

	UPROPERTY(EditAnywhere)
		UDataTable* ChestTable;

	UPROPERTY(EditAnywhere)
		UDataTable* PatnsTable;

	UPROPERTY(EditAnywhere)
		UDataTable* BootsTable;

	UPROPERTY(EditAnywhere)
		UDataTable* WeaponTable;
};

USTRUCT(BlueprintType)
struct FItem : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		class USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere)
		class UStaticMesh* Item;

	UPROPERTY(EditAnywhere)
		class UTexture2D* SumNail;

	UPROPERTY(EditAnywhere)
		FString Desc;

	UPROPERTY(EditAnywhere)
		bool bStack;

	UPROPERTY(EditAnywhere)
		FVector Translation = FVector(0, 50, 0);

	UPROPERTY(EditAnywhere)
		FRotator Rotation = FRotator(0, 0, -90);

public:
	UPROPERTY(EditAnywhere)
		EActionType ActionType;

	UPROPERTY(EditAnywhere)
		bool bTwoHand; // 양손 착용 무기인지

	UPROPERTY(EditAnywhere)
		float MinPower;

	UPROPERTY(EditAnywhere)
		float MaxPower;

	UPROPERTY(EditAnywhere)
		float Critical;

	UPROPERTY(EditAnywhere)
		float MinDeffense;

	UPROPERTY(EditAnywhere)
		float MaxDeffense;
};

USTRUCT(BlueprintType)
struct FEquipment_Bracer : public FItem
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FEquipment_Cloak : public FItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		CloakType Type;
};

USTRUCT(BlueprintType)
struct FEquipment_Gloves : public FItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		GlovesType Type;
};

USTRUCT(BlueprintType)
struct FEquipment_Robe : public FItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		RobeType Type;
};

USTRUCT(BlueprintType)
struct FEquipment_Shoulders : public FItem
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FEquipment_Boots : public FItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		BootsType Type;

	UPROPERTY(EditAnywhere)
		BootsHeight Height;
};

USTRUCT(BlueprintType)
struct FEquipment_Chest : public FItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		ChestType ChestType;

	UPROPERTY(EditAnywhere)
		SleeveType SleeveType;
};

USTRUCT(BlueprintType)
struct FEquipment_Helmet : public FItem
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FEquipment_Pants : public FItem
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FEquipment_Weapon : public FItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		WeaponType Type;
};

UCLASS()
class UE4PROJECT_API ACEquipCharacter : public ACActionCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCEquipComponent* Equip;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterMesh")
		UDataTable* DataTable;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "CharacterMesh")
		ECharacter CharacterType;

	UPROPERTY(EditDefaultsOnly, Category = "CharacterMesh")
		bool bFullMesh = false;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* BodyChest;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* BodyArms;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* BodyHands;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* BodyLegs;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* BodyFeet;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqCloak;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqShoulders;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqRobe;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqGloves;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqHelmet;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqChest;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqPants;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqBoots;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqLeftBracer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqRightBracer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqLeftWeapon; // Only Preview

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		class USkeletalMeshComponent* EqRightWeapon; // Only Preview

public:
	FORCEINLINE const ECharacter* GetCharacterType() { return &CharacterType; }

protected:
	UFUNCTION(BlueprintCallable)
		void SetCharacter(const ECharacter& InType);

protected:
	void UpdateMesh();

	void SetFullMesh();

	void SetPartMesh();

public:
	ACEquipCharacter();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void ChangeColor_Implementation(FLinearColor InColor) override;
	virtual void Begin_Dead() override {};
	virtual void End_Dead() override {};

public:
	USkeletalMeshComponent* GetEquipMesh(const EEquipmentType& InEquipType);


protected:
	TArray<USkeletalMeshComponent*> Bodies;
	USkeletalMeshComponent* Equips[(int32)EEquipmentType::Max + 1]; // Weapon -> RightWeapon, Weapon + 1 -> LeftWeapon

protected:
	FCharacterMesh CharacterInfo;

private:
	class UMaterialInstanceDynamic* BodyMaterial;

};
