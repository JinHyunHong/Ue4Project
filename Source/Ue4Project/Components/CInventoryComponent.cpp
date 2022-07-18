#include "CInventoryComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEquipCharacter.h"
#include "Characters/CPlayer.h"
#include "Actions/CAttachment.h"
#include "Widgets/CUserWidget_EquipInventory.h"
#include "Widgets/CUserWidget_Equipment.h"
#include "Widgets/CUserWidget_Inventory.h"

UCInventoryComponent::UCInventoryComponent()
{
	CHelpers::GetClass<UCUserWidget_EquipInventory>(&EquipIventoryClass, "WidgetBlueprint'/Game/Widgets/WB_EquipInventory.WB_EquipInventory_C'");
	CHelpers::GetClass<ACEquipCharacter>(&EquipStateClass, "Blueprint'/Game/Player/BP_CEquipState.BP_CEquipState_C'");
}


void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	ACPlayer* owner = Cast<ACPlayer>(GetOwner());
	
	EquipIventory = CreateWidget<UCUserWidget_EquipInventory, APlayerController>(owner->GetController<APlayerController>(), EquipIventoryClass);
	EquipIventory->AddToViewport();
	
	Equipment = Cast<UCUserWidget_Equipment>(EquipIventory->GetEquipment());
	Inventory = Cast<UCUserWidget_Inventory>(EquipIventory->GetInventory());

	FTransform transform = GetOwner()->GetActorTransform();
	transform.SetLocation(FVector(transform.GetLocation().X, transform.GetLocation().Y, 5069518));
	transform.SetRotation(FQuat(0.0f, 0.0f,-180.0f, 0.0f));
	EquipState = GetWorld()->SpawnActorDeferred<ACEquipCharacter>(EquipStateClass, transform);
	UGameplayStatics::FinishSpawningActor(EquipState, transform);

	Equipment->SetVisibility(ESlateVisibility::Hidden);
	Inventory->SetVisibility(ESlateVisibility::Hidden);
}

void UCInventoryComponent::ViewAllHidden()
{
	CheckNull(Equipment);
	CheckNull(Inventory);
	SetViewList(EViewListType::AllHidden);
}

void UCInventoryComponent::ViewEquipment()
{
	CheckNull(Equipment);
	SetViewList(EViewListType::Equipment);
}

void UCInventoryComponent::ViewInventory()
{
	CheckNull(Inventory);
	SetViewList(EViewListType::Inventory);
}

void UCInventoryComponent::ViewAll()
{
	CheckNull(Equipment);
	CheckNull(Inventory);
	SetViewList(EViewListType::AllView);
}

void UCInventoryComponent::AddItem(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData, bool bPickUp)
{
	CheckNull(Inventory);
	Inventory->AddItem(InCharacterType, InEquipType, InItemName, InItemData);

	if (bPickUp == false) OnUnequipState(InEquipType);
}

void UCInventoryComponent::AddEquip(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	CheckNull(Equipment);
	Equipment->AddEquip(InCharacterType, InEquipType, InItemName, InItemData);

	OnEquipState(InEquipType, InItemName, InItemData);
}

void UCInventoryComponent::OnEquipState(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	CheckNull(EquipState);
	CheckNull(InItemData.Mesh);
	USkeletalMeshComponent* component = EquipState->GetEquipMesh(InEquipType);
	CheckNull(component);
	component->SetSkeletalMesh(InItemData.Mesh);

	// 왼손에도 무기 해제 (Weapon + 1 -> LeftWeapon)
	component = EquipState->GetEquipMesh((EEquipmentType)((int32)EEquipmentType::Weapon + 1));
	component->SetSkeletalMesh(NULL);

	if (InItemData.bTwoHand)
	{
		// 왼손에도 무기 장착 (Weapon + 1 -> LeftWeapon)
		component = EquipState->GetEquipMesh((EEquipmentType)((int32)InEquipType + 1));
		component->SetSkeletalMesh(InItemData.Mesh);
	}
}

void UCInventoryComponent::OnUnequipState(const EEquipmentType& InEquipType)
{
	CheckNull(EquipState);
	USkeletalMeshComponent* component = EquipState->GetEquipMesh(InEquipType);
	CheckNull(component);
	component->SetSkeletalMesh(NULL);

	// 왼손에도 무기 해제 (Weapon + 1 -> LeftWeapon)
	component = EquipState->GetEquipMesh((EEquipmentType)((int32)EEquipmentType::Weapon + 1));
	component->SetSkeletalMesh(NULL);
}

void UCInventoryComponent::SetViewList(const EViewListType& InType)
{
	bool bDilation = false;
	ESlateVisibility type;
	ACharacter* owner = Cast<ACharacter>(GetOwner());

	switch (InType)
	{
		case EViewListType::AllHidden:
		{
			Equipment->SetVisibility(ESlateVisibility::Hidden);
			Inventory->SetVisibility(ESlateVisibility::Hidden);
			owner->GetController<APlayerController>()->bShowMouseCursor = false;
			owner->GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());

			break;
		}
		case EViewListType::Equipment:
		{
			type = Equipment->GetVisibility() != ESlateVisibility::Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
			Equipment->SetVisibility(type);

			bDilation |= type == ESlateVisibility::Visible;
			bDilation |= (!!Inventory) && Inventory->GetVisibility() == ESlateVisibility::Visible;

			if (bDilation)
			{
				owner->GetController<APlayerController>()->bShowMouseCursor = true;
				owner->GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());
				break;
			}
			
			owner->GetController<APlayerController>()->bShowMouseCursor = false;
			owner->GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());
			break;
		}	
		case EViewListType::Inventory:
		{
			type = Inventory->GetVisibility() != ESlateVisibility::Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
			Inventory->SetVisibility(type);
			
			bDilation |= type == ESlateVisibility::Visible;
			bDilation |= (!!Equipment) && Equipment->GetVisibility() == ESlateVisibility::Visible;

			if (bDilation)
			{
				owner->GetController<APlayerController>()->bShowMouseCursor = true;
				owner->GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());
				break;
			}

			owner->GetController<APlayerController>()->bShowMouseCursor = false;
			owner->GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());
			break;
		}
		case EViewListType::AllView:
		{
			Equipment->SetVisibility(ESlateVisibility::Visible);
			Inventory->SetVisibility(ESlateVisibility::Visible);
			owner->GetController<APlayerController>()->bShowMouseCursor = true;
			owner->GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());

			break;
		}
	}
}