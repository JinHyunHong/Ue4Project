#include "Actors/CItem.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "Widgets/CUserWidget_PickMessage.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"


ACItem::ACItem()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh", Scene);
	CHelpers::CreateComponent<USphereComponent>(this, &Sphere, "Sphere", Scene);
	CHelpers::CreateComponent<UWidgetComponent>(this, &Widget, "Widget", Scene);

	CHelpers::GetClass<UCUserWidget_PickMessage>(&PickMessageClass, "WidgetBlueprint'/Game/Widgets/WB_PickMessage.WB_PickMessage_C'");
	
	Widget->SetWidgetClass(PickMessageClass);
	Widget->SetDrawSize(FVector2D(50, 200));
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetRelativeLocation(FVector(0, 0, 40));
}

void ACItem::BeginPlay()
{
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACItem::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACItem::OnSphereEndOverlap);

	Widget->InitWidget();
	UCUserWidget_PickMessage* widget = Cast<UCUserWidget_PickMessage>(Widget->GetUserWidgetObject());
	CheckNull(widget);

	widget->SetVisibility(ESlateVisibility::Hidden);

	FindItemData(ItemData);

	Super::BeginPlay();
}

void ACItem::OnConstruction(const FTransform& Transform)
{
	Mesh->SetStaticMesh(NULL);

	UStaticMesh* mesh = NULL;

	FItem itemData;
	CheckFalse(FindItemData(itemData));

	mesh = itemData.Item;

	FVector translation = itemData.Translation;
	FRotator rotation = itemData.Rotation;
	
	CheckNull(mesh);
	Mesh->SetStaticMesh(mesh);
	Mesh->SetRelativeLocation(translation);
	Mesh->SetRelativeRotation(rotation);

	Super::OnConstruction(Transform);
}

void ACItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OnCustomDepth
	Mesh->SetRenderCustomDepth(true);
	Mesh->SetCustomDepthStencilValue(2);

	// View PickMassage Widget
	Widget->InitWidget();
	UCUserWidget_PickMessage* widget = Cast<UCUserWidget_PickMessage>(Widget->GetUserWidgetObject());
	CheckNull(widget);

	widget->SetVisibility(ESlateVisibility::Visible);

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);
	player->SetInteractItem(this);
}


void ACItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// OffCustomDepth
	Mesh->SetRenderCustomDepth(false);
	Mesh->SetCustomDepthStencilValue(0);

	// Hidden PickMassage Widget
	Widget->InitWidget();
	UCUserWidget_PickMessage* widget = Cast<UCUserWidget_PickMessage>(Widget->GetUserWidgetObject());
	CheckNull(widget);

	widget->StopAllAnimations();
	widget->SetForward(true);
	widget->SetVisibility(ESlateVisibility::Hidden);

	ACPlayer* player = Cast<ACPlayer>(OtherActor);
	CheckNull(player);
	player->SetInteractItem(NULL);
}

bool ACItem::FindItemData(FItem& OutItemData)
{
	FName characterType = FName(CHelpers::GetEnumToString(CharacterType, "ECharacter"));
	FName equipName = FName(CHelpers::GetEnumToString(EquipType, "EquipmentType"));
	FName itemName = FName(ItemName);

	FEquipmentDataTables* equipDatas = DataTable->FindRow<FEquipmentDataTables>(characterType, FString(""));
	CheckNullResult(equipDatas, false);

	UDataTable* equipData = NULL;

	switch (EquipType)
	{
		case EEquipmentType::Chest:
		{
			equipData = equipDatas->ChestTable;
			FEquipment_Chest* chest = equipData->FindRow<FEquipment_Chest>(itemName, FString(""));
			CheckNullResult(chest, false);
			OutItemData = chest->Item;
			return true;
		}
		case EEquipmentType::Pants:
		{
			equipData = equipDatas->PatnsTable;
			FEquipment_Pants* pants = equipData->FindRow<FEquipment_Pants>(itemName, FString(""));
			CheckNullResult(pants, false);
			OutItemData = pants->Item;
			return true;
		}
		case EEquipmentType::Boots:
		{
			equipData = equipDatas->BootsTable;
			FEquipment_Boots* boots = equipData->FindRow<FEquipment_Boots>(itemName, FString(""));
			CheckNullResult(boots, false);
			OutItemData = boots->Item;
			return true;
		}
		case EEquipmentType::Helmet:
		{
			equipData = equipDatas->HelmetTable;
			FEquipment_Helmet* helmet = equipData->FindRow<FEquipment_Helmet>(itemName, FString(""));
			CheckNullResult(helmet, false);
			OutItemData = helmet->Item;
			return true;
		}
		case EEquipmentType::LeftBracer:
		{
			equipData = equipDatas->BracersTable;
			FBracersPair* bracers = equipData->FindRow<FBracersPair>(itemName, FString(""));
			CheckNullResult(bracers, false);
			OutItemData = bracers->Left.Item;
			return true;
		}
		case EEquipmentType::RightBracer:
		{
			equipData = equipDatas->BracersTable;
			FBracersPair* bracers = equipData->FindRow<FBracersPair>(itemName, FString(""));
			CheckNullResult(bracers, false);
			OutItemData = bracers->Right.Item;
			return true;
		}
		case EEquipmentType::Cloak:
		{
			equipData = equipDatas->CloakTable;
			FEquipment_Cloak* cloak = equipData->FindRow<FEquipment_Cloak>(itemName, FString(""));
			CheckNullResult(cloak, false);
			OutItemData = cloak->Item;
			return true;
		}
		case EEquipmentType::Gloves:
		{
			equipData = equipDatas->GlovesTable;
			FEquipment_Gloves* gloves = equipData->FindRow<FEquipment_Gloves>(itemName, FString(""));
			CheckNullResult(gloves, false);
			OutItemData = gloves->Item;
			return true;
		}
		case EEquipmentType::Robes:
		{
			equipData = equipDatas->RobesTable;
			FEquipment_Robe* robe = equipData->FindRow<FEquipment_Robe>(itemName, FString(""));
			CheckNullResult(robe, false);
			OutItemData = robe->Item;
			return true;
		}
		case EEquipmentType::Shoulders:
		{
			equipData = equipDatas->ShouldersTable;
			FEquipment_Shoulders* shoulders = equipData->FindRow<FEquipment_Shoulders>(itemName, FString(""));
			CheckNullResult(shoulders, false);
			OutItemData = shoulders->Item;
			return true;
		}
		case EEquipmentType::Weapon:
		{
			equipData = equipDatas->WeaponTable;
			FEquipment_Weapon* weapon = equipData->FindRow<FEquipment_Weapon>(itemName, FString(""));
			CheckNullResult(weapon, false);
			OutItemData = weapon->Item;
			return true;
		}
	}
	
	return false;
}
