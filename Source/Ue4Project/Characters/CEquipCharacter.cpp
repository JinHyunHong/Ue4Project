#include "CEquipCharacter.h"
#include "Global.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Components/CEquipComponent.h"

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


	CHelpers::CreateActorComponent<UCEquipComponent>(this, &Equip, "Equip");

	// Bodies
	BodyChest->ComponentTags.Add("Body");
	BodyArms->ComponentTags.Add("Body");
	BodyHands->ComponentTags.Add("Body");
	BodyLegs->ComponentTags.Add("Body");
	BodyFeet->ComponentTags.Add("Body");

	// Equip
	Equips.Add(EEquipmentType::Chest, EqChest);
	Equips.Add(EEquipmentType::Pants, EqPants);
	Equips.Add(EEquipmentType::Boots, EqBoots);
	Equips.Add(EEquipmentType::Helmet, EqHelmet);
	Equips.Add(EEquipmentType::LeftBracer, EqLeftBracer);
	Equips.Add(EEquipmentType::RightBracer, EqRightBracer);
	Equips.Add(EEquipmentType::Cloak, EqCloak);
	Equips.Add(EEquipmentType::Gloves, EqGloves);
	Equips.Add(EEquipmentType::Robes, EqRobe);
	Equips.Add(EEquipmentType::Shoulders, EqShoulders);

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

	for (TPair<EEquipmentType, USkeletalMeshComponent*> equip : Equips)
	{
		CHelpers::GetClass<UClothingSimulationFactory>(&equip.Value->ClothingSimulationFactory, path);
	}

	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}

void ACEquipCharacter::BeginPlay()
{
	SetCharacter(CharacterType);

	// Mesh기준으로 Pose 변경
	for (USkeletalMeshComponent* body : Bodies)
		body->SetMasterPoseComponent(GetMesh(), true);

	for (TPair<EEquipmentType, USkeletalMeshComponent*> equip : Equips)
	{
		equip.Value->SetMasterPoseComponent(GetMesh(), true);
	}

	// Head
	Bodies.Add(GetMesh());

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

void ACEquipCharacter::SetMasterPose()
{
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
	USkeletalMeshComponent* mesh = Equips[InEquipType];
	CheckNullResult(mesh, NULL);

	return mesh;
}