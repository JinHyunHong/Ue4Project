#include "CEquipComponent.h"
#include "Global.h"
#include "Engine/SkeletalMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Actions/CAttachment.h"

UCEquipComponent::UCEquipComponent()
{
}


void UCEquipComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCEquipComponent::Equip(const EEquipmentType& InEquipType, const FString& InItemName)
{
	CheckNull(DataTable);
	CheckEmpty(InItemName);

	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwner());
	CheckNull(owner);

	FName characterType = FName(CHelpers::GetEnumToString(*owner->GetCharacterType(), "ECharacter"));
	FName itemName = FName(InItemName);
	FItem* itemData = NULL;

	FEquipmentDataTables* equipDatas = DataTable->FindRow<FEquipmentDataTables>(characterType, FString(""));
	CheckNull(equipDatas);

	UDataTable* equipData = NULL;

	switch (InEquipType)
	{
		case EEquipmentType::Chest: equipData = equipDatas->ChestTable; break;
		case EEquipmentType::Pants: equipData = equipDatas->PatnsTable; break;
		case EEquipmentType::Boots: equipData = equipDatas->BootsTable; break;
		case EEquipmentType::Helmet: equipData = equipDatas->HelmetTable; break;
		case EEquipmentType::LeftBracer: equipData = equipDatas->LeftBracerTable; break;
		case EEquipmentType::RightBracer: equipData = equipDatas->RightBracerTable; break;
		case EEquipmentType::Cloak:	equipData = equipDatas->CloakTable; break;
		case EEquipmentType::Gloves: equipData = equipDatas->GlovesTable; break;
		case EEquipmentType::Robes: equipData = equipDatas->RobesTable; break;
		case EEquipmentType::Shoulders: equipData = equipDatas->ShouldersTable; break;
		case EEquipmentType::Weapon: equipData = equipDatas->WeaponTable; break;
	}

	CheckNull(equipData);

	itemData = equipData->FindRow<FItem>(itemName, FString(""));

	Equip(InEquipType, InItemName, *itemData);

	return;
}


void UCEquipComponent::Unequip(const EEquipmentType& InEquipType)
{
	FString itemName = FString();
	FItem itemData = FItem();
	Unequip(InEquipType, itemName, itemData);
}

void UCEquipComponent::Equip(const EEquipmentType& InEquipType, const FString& InItemName, FItem& InItemData)
{
	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwner());
	CheckNull(owner);

	USkeletalMeshComponent* component = NULL;

	if (InEquipType != EEquipmentType::Weapon)
	{
		CheckNull(InItemData.Mesh);
		component = owner->GetEquipMesh(InEquipType);
		component->SetSkeletalMesh(InItemData.Mesh);
	}

	if (OnEquipChanged.IsBound())
		OnEquipChanged.Broadcast(InEquipType, InItemName, InItemData);
}

void UCEquipComponent::Unequip(const EEquipmentType& InEquipType, const FString& InItemName, FItem& InItemData)
{
	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwner());
	CheckNull(owner);

	USkeletalMeshComponent* component = NULL;
	
	if (InEquipType != EEquipmentType::Weapon)
	{
		component = owner->GetEquipMesh(InEquipType);
		component->SetSkeletalMesh(NULL);
	}

	if (OnUnequipChanged.IsBound())
		OnUnequipChanged.Broadcast(InEquipType, InItemName, InItemData);
}
