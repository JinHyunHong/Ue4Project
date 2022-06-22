#include "CInventoryComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEquipCharacter.h"
#include "Characters/CPlayer.h"
#include "Actors/CAttachment.h"
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
	transform.SetRotation(FQuat(0, 0, -180, 0));
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

void UCInventoryComponent::AddItem(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	CheckNull(Inventory);
	Inventory->AddItem(InCharacterType, InEquipType, InItemName, InItemData);

	OnUnequipState(InEquipType);
}

void UCInventoryComponent::AddEquip(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData, bool bInitEquip)
{
	CheckNull(Equipment);
	Equipment->AddEquip(InCharacterType, InEquipType, InItemName, InItemData, bInitEquip);

	OnEquipState(InEquipType, InItemData);
}

void UCInventoryComponent::OnEquipState(const EEquipmentType& InEquipType, const FItem& InItemData)
{
	CheckNull(EquipState);
	
	if (InEquipType == EEquipmentType::Weapon)
	{
		FTransform transform;
		TSubclassOf<ACAttachment> attachmentClass = InItemData.AttachmentClass;
		CheckNull(attachmentClass);
	
		StateWeapon = Cast<AActor>(EquipState->GetWorld()->SpawnActorDeferred<ACAttachment>(attachmentClass, transform, EquipState));
		CheckNull(StateWeapon);
	
		StateWeapon->SetActorLabel(EquipState->GetActorLabel() + "_" + StateWeapon->GetActorLabel());
	
		UGameplayStatics::FinishSpawningActor(StateWeapon, transform);
	
		return;
	}

	CheckNull(InItemData.Mesh);
	USkeletalMeshComponent* mesh = EquipState->GetEquipMesh(InEquipType);
	CheckNull(mesh);
	mesh->SetSkeletalMesh(InItemData.Mesh);
}

void UCInventoryComponent::OnUnequipState(const EEquipmentType& InEquipType)
{
	CheckNull(EquipState);

	if (InEquipType == EEquipmentType::Weapon)
	{
		StateWeapon->Destroy();

		return;
	}

	USkeletalMeshComponent* mesh = EquipState->GetEquipMesh(InEquipType);
	CheckNull(mesh);
	mesh->SetSkeletalMesh(NULL);
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
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
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
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.05f);
				owner->GetController<APlayerController>()->bShowMouseCursor = true;
				owner->GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());
				break;
			}
			
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
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
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.05f);
				owner->GetController<APlayerController>()->bShowMouseCursor = true;
				owner->GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());
				break;
			}

			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
			owner->GetController<APlayerController>()->bShowMouseCursor = false;
			owner->GetController<APlayerController>()->SetInputMode(FInputModeGameOnly());
			break;
		}
		case EViewListType::AllView:
		{
			Equipment->SetVisibility(ESlateVisibility::Visible);
			Inventory->SetVisibility(ESlateVisibility::Visible);
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.05f);				
			owner->GetController<APlayerController>()->bShowMouseCursor = true;
			owner->GetController<APlayerController>()->SetInputMode(FInputModeGameAndUI());

			break;
		}
	}
}