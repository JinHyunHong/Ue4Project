#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "CEquipCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacter : uint8
{
	Sword, Max
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
	Sword, Axe, Dagger, Rod,
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
		UDataTable* BracersTable;

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
struct FItem
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
struct FEquipment_Bracer : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItem Item;
};

USTRUCT(BlueprintType)
struct FEquipment_Cloak : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		CloakType Type;

	UPROPERTY(EditAnywhere)
		FItem Item;
};

USTRUCT(BlueprintType)
struct FEquipment_Gloves : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		GlovesType Type;

	UPROPERTY(EditAnywhere)
		FItem Item;
};

USTRUCT(BlueprintType)
struct FEquipment_Robe : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		RobeType Type;

	UPROPERTY(EditAnywhere)
		FItem Item;
};

USTRUCT(BlueprintType)
struct FEquipment_Shoulders : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItem Item;
};

USTRUCT(BlueprintType)
struct FEquipment_Boots : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItem Item;

	UPROPERTY(EditAnywhere)
		BootsType Type;

	UPROPERTY(EditAnywhere)
		BootsHeight Height;
};

USTRUCT(BlueprintType)
struct FEquipment_Chest : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItem Item;

	UPROPERTY(EditAnywhere)
		ChestType ChestType;

	UPROPERTY(EditAnywhere)
		SleeveType SleeveType;
};

USTRUCT(BlueprintType)
struct FEquipment_Helmet : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItem Item;
};

USTRUCT(BlueprintType)
struct FEquipment_Pants : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItem Item;
};

USTRUCT(BlueprintType)
struct FBracersPair : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FEquipment_Bracer Left;

	UPROPERTY(EditAnywhere)
		FEquipment_Bracer Right;
};

USTRUCT(BlueprintType)
struct FEquipment_Weapon : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FItem Item;

	UPROPERTY(EditAnywhere)
		WeaponType Type;
};


UCLASS()
class UE4PROJECT_API ACEquipCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCEquipComponent* Equip;

	UPROPERTY(VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

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
		class USkeletalMeshComponent* EqWeapon; // Only Preview

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
	virtual void Begin_Dead() {};
	virtual void End_Dead() {};

public:
	USkeletalMeshComponent* GetEquipMesh(const EEquipmentType& InEquipType);


protected:
	TArray<USkeletalMeshComponent*> Bodies;
	TMap<EEquipmentType, USkeletalMeshComponent*> Equips;

protected:
	FCharacterMesh CharacterInfo;
};
