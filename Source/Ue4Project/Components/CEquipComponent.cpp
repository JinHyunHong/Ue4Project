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
	CheckEmpty(InItemName);

	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwner());
	CheckNull(owner);

	FName characterType = FName(CHelpers::GetEnumToString(*owner->GetCharacterType(), "ECharacter"));
	FName itemName = FName(InItemName);
	FItem itemData;

	FEquipmentDataTables* equipDatas = DataTable->FindRow<FEquipmentDataTables>(characterType, FString(""));
	CheckNull(equipDatas);

	UDataTable* equipData = NULL;

	switch (InEquipType)
	{
		case EEquipmentType::Chest:
		{
			equipData = equipDatas->ChestTable;
			FEquipment_Chest* chestData = equipData->FindRow<FEquipment_Chest>(itemName, FString(""));
			itemData = chestData->Item;
			break;
		}
		case EEquipmentType::Pants:
		{
			equipData = equipDatas->PatnsTable;
			FEquipment_Pants* pantsData = equipData->FindRow<FEquipment_Pants>(itemName, FString(""));
			itemData = pantsData->Item;
			break;
		}
		case EEquipmentType::Boots:
		{
			equipData = equipDatas->BootsTable;
			FEquipment_Boots* bootsData = equipData->FindRow<FEquipment_Boots>(itemName, FString(""));
			itemData = bootsData->Item;
			break;
		}
		case EEquipmentType::Helmet:
		{
			equipData = equipDatas->HelmetTable;
			FEquipment_Helmet* helmetData = equipData->FindRow<FEquipment_Helmet>(itemName, FString(""));
			itemData = helmetData->Item;
			break;
		}
		case EEquipmentType::LeftBracer:
		{			
			equipData = equipDatas->BracersTable;
			FBracersPair* bracersData = equipData->FindRow<FBracersPair>(itemName, FString(""));
			itemData = bracersData->Left.Item;
			break;
		}
		case EEquipmentType::RightBracer:
		{
			equipData = equipDatas->BracersTable;
			FBracersPair* bracersData = equipData->FindRow<FBracersPair>(itemName, FString(""));
			itemData = bracersData->Right.Item;
			break;
		}
		case EEquipmentType::Cloak:
		{
			equipData = equipDatas->CloakTable;	
			FEquipment_Cloak* cloakData = equipData->FindRow<FEquipment_Cloak>(itemName, FString(""));
			itemData = cloakData->Item;
			break;
		}
		case EEquipmentType::Gloves:
		{
			equipData = equipDatas->GlovesTable;	
			FEquipment_Gloves* glovesData = equipData->FindRow<FEquipment_Gloves>(itemName, FString(""));
			itemData = glovesData->Item;
			break;
		}
		case EEquipmentType::Robes:
		{
			equipData = equipDatas->RobesTable;
			FEquipment_Robe* robeData = equipData->FindRow<FEquipment_Robe>(itemName, FString(""));
			itemData = robeData->Item;
			break;
		}
		case EEquipmentType::Shoulders:
		{
			equipData = equipDatas->ShouldersTable;
			FEquipment_Shoulders* shouldersData = equipData->FindRow<FEquipment_Shoulders>(itemName, FString(""));
			itemData = shouldersData->Item;
			break;
		}
		case EEquipmentType::Weapon:
		{
			FTransform transform;
			
			equipData = equipDatas->WeaponTable;
			FEquipment_Weapon* weaponData = equipData->FindRow<FEquipment_Weapon>(itemName, FString(""));
			itemData = weaponData->Item;

			break;
		}
	}


	Equip(InEquipType, InItemName, itemData);

	return;
}

bool UCEquipComponent::Unequip(const EEquipmentType& InEquipType)
{
	FString itemName = FString();
	FItem item = FItem();
	Unequip(InEquipType, itemName, item);

	return true;
}

void UCEquipComponent::Equip(const EEquipmentType& InEquipType, const FString& InItemName, FItem& InItemData)
{
	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwner());
	CheckNull(owner);

	if (InEquipType != EEquipmentType::Weapon)
	{
		CheckNull(InItemData.Mesh);
		USkeletalMeshComponent* component = owner->GetEquipMesh(InEquipType);
		component->SetSkeletalMesh(InItemData.Mesh);
	}

	if (OnEquipChanged.IsBound())
		OnEquipChanged.Broadcast(InEquipType, InItemName, InItemData);
}

void UCEquipComponent::Unequip(const EEquipmentType& InEquipType, const FString& InItemName, FItem& InItemData)
{
	ACEquipCharacter* owner = Cast<ACEquipCharacter>(GetOwner());
	CheckNull(owner);

	if (InEquipType != EEquipmentType::Weapon)
	{
		USkeletalMeshComponent* component = owner->GetEquipMesh(InEquipType);
		component->SetSkeletalMesh(NULL);
	}

	if (OnUnequipChanged.IsBound())
		OnUnequipChanged.Broadcast(InEquipType, InItemName, InItemData);
}
