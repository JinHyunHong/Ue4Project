#include "CEquipCharacter.h"
#include "Global.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/CEquipComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACEquipCharacter::ACEquipCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &BodyChest, "BodyChest", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &BodyArms, "BodyArms", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &BodyHands, "BodyHands", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &BodyLegs, "BodyLegs", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &BodyFeet, "BodyFeet", GetMesh());

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqCloak, "EqCloak", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqShoulders, "EqShoulders", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqRobe, "EqRobe", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqGloves, "EqGloves", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqHelmet, "EqHelmet", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqChest, "EqChest", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqPants, "EqPants", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqBoots, "EqBoots", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqLeftBracer, "EqLeftBracer", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqRightBracer, "EqRightBracer", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqRightWeapon, "EqRightWeapon", GetMesh());
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &EqLeftWeapon, "EqLeftWeapon", GetMesh());

	CHelpers::CreateActorComponent<UCEquipComponent>(this, &Equip, "Equip");

	// Bodies
	BodyChest->ComponentTags.Add("Body");
	BodyArms->ComponentTags.Add("Body");
	BodyHands->ComponentTags.Add("Body");
	BodyLegs->ComponentTags.Add("Body");
	BodyFeet->ComponentTags.Add("Body");

	// Equip
	Equips[(int32)EEquipmentType::Chest] = EqChest;
	Equips[(int32)EEquipmentType::Pants] = EqPants;
	Equips[(int32)EEquipmentType::Boots] = EqBoots;
	Equips[(int32)EEquipmentType::Helmet] = EqHelmet;
	Equips[(int32)EEquipmentType::LeftBracer] = EqLeftBracer;
	Equips[(int32)EEquipmentType::RightBracer] = EqRightBracer;
	Equips[(int32)EEquipmentType::Cloak] = EqCloak;
	Equips[(int32)EEquipmentType::Gloves] = EqGloves;
	Equips[(int32)EEquipmentType::Robes] = EqRobe;
	Equips[(int32)EEquipmentType::Shoulders] = EqShoulders;
	Equips[(int32)EEquipmentType::Weapon] = EqRightWeapon;
	Equips[(int32)EEquipmentType::Weapon + 1] = EqLeftWeapon;

	// SetClothingSimulationFactory
	FString path = "Class'/Script/ClothingSystemRuntimeNv.ClothingSimulationFactoryNv'";


	TArray<UActorComponent*> components;
	components = GetComponentsByTag(USkeletalMeshComponent::StaticClass(), "Body");

	for (UActorComponent* component : components)
	{
		USkeletalMeshComponent* body = Cast<USkeletalMeshComponent>(component);

		if (!!body)
		{
			CHelpers::GetClass<UClothingSimulationFactory>(&body->ClothingSimulationFactory, path);

			Bodies.Add(body);
		}
	}

	for (int i = 0; i < (int32)EEquipmentType::Max + 1; i++)
	{
		if(!!Equips[i]) CHelpers::GetClass<UClothingSimulationFactory>(&Equips[i]->ClothingSimulationFactory, path);
	}

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// EqWeapon Set Socket
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Characters/Meshes/UCCS_Male/SK_UCCS_Male.SK_UCCS_Male'");
	GetMesh()->SetSkeletalMesh(mesh);
	EqLeftWeapon->SetupAttachment(GetMesh(), "Left_Hand");
	EqRightWeapon->SetupAttachment(GetMesh(), "Right_Hand");
}

void ACEquipCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ACEquipCharacter::BeginPlay()
{
	SetCharacter(CharacterType);

	UMaterialInstanceConstant* bodyInst;

	CHelpers::GetAssetDynamic(&bodyInst, "MaterialInstanceConstant'/Game/Characters/Materials/UCCS_Male/MI_Diffuse.MI_Diffuse'");

	BodyMaterial = UMaterialInstanceDynamic::Create(bodyInst, this);

	// Mesh기준으로 Pose 변경, SetMaterial
	for (USkeletalMeshComponent* body : Bodies)
	{
		body->SetMasterPoseComponent(GetMesh(), true);
		body->SetMaterial(0, BodyMaterial);
	}

	for (int i = 0; i < (int32)EEquipmentType::Max + 1; i++)
	{
		if (!!Equips[i]) Equips[i]->SetMasterPoseComponent(GetMesh(), true);
	}

	// Head
	Bodies.Add(GetMesh());

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(2, BodyMaterial);
	GetMesh()->SetMaterial(3, BodyMaterial);
	GetMesh()->SetMaterial(4, BodyMaterial);

	Super::BeginPlay();
}

void ACEquipCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEquipCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACEquipCharacter::ChangeColor_Implementation(FLinearColor InColor)
{
	Super::ChangeColor_Implementation(InColor);

	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACEquipCharacter::SetCharacter(const ECharacter& InType)
{
	CharacterType = InType;

	FCharacterMesh* result;
	result = DataTable->FindRow<FCharacterMesh>(FName(CHelpers::GetEnumToString(InType, "ECharacter")), FString(""));

	CheckNull(result);
	CheckFalse(result->Character == CharacterType);
	
	CharacterInfo = *result;
	
	UpdateMesh();
}

void ACEquipCharacter::UpdateMesh()
{
	if (bFullMesh)
	{
		SetFullMesh();

		return;
	}

	SetPartMesh();
}

void ACEquipCharacter::SetFullMesh()
{
	for (USkeletalMeshComponent* body : Bodies)
		body->SetSkeletalMesh(NULL);

	GetMesh()->SetSkeletalMesh(CharacterInfo.FullMesh);
	GetMesh()->SetAnimInstanceClass(CharacterInfo.AnimBP);
}

void ACEquipCharacter::SetPartMesh()
{
	CheckFalse(CharacterInfo.Character == CharacterType);

	// Part SetSkeletalMesh
	GetMesh()->SetSkeletalMesh(CharacterInfo.HeadMesh);
	BodyChest->SetSkeletalMesh(CharacterInfo.ChestMesh);
	BodyArms->SetSkeletalMesh(CharacterInfo.ArmsMesh);
	BodyHands->SetSkeletalMesh(CharacterInfo.HandsMesh);
	BodyLegs->SetSkeletalMesh(CharacterInfo.LegsMesh);
	BodyFeet->SetSkeletalMesh(CharacterInfo.FeetMesh);

	GetMesh()->SetAnimInstanceClass(CharacterInfo.AnimBP);
}

USkeletalMeshComponent* ACEquipCharacter::GetEquipMesh(const EEquipmentType& InEquipType)
{
	USkeletalMeshComponent* mesh = Equips[(int32)InEquipType];
	CheckNullResult(mesh, NULL);

	return mesh;
}