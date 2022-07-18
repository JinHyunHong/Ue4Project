#include "Actions/CItem.h"
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
	CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/DataTables/Equipment/DT_Equipment.DT_Equipment'");

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
	
	SetItem();

	Super::BeginPlay();
}

void ACItem::OnConstruction(const FTransform& Transform)
{
	SetItem();

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

void ACItem::SetItem()
{
	Mesh->SetStaticMesh(NULL);

	UStaticMesh* mesh = NULL;

	FindItemData();

	mesh = ItemData.Item;
	CheckNull(mesh);

	FVector translation = ItemData.Translation;
	FRotator rotation = ItemData.Rotation;

	Mesh->SetStaticMesh(mesh);
	Mesh->SetRelativeLocation(translation);
	Mesh->SetRelativeRotation(rotation);
}

void ACItem::FindItemData()
{
	CheckNull(DataTable);

	// 랜덤 아이템의 경우
	if (bRandom)
	{
		CharacterType = (ECharacter)UKismetMathLibrary::RandomIntegerInRange(0, (int32)ECharacter::Max - 1);
		EquipType = (EEquipmentType)UKismetMathLibrary::RandomIntegerInRange((int32)EEquipmentType::Chest, (int32)EEquipmentType::Max - 1);

		// TODO : Item Type Add (None -> Item)
		// EquipType = (EEquipmentType)UKismetMathLibrary::RandomIntegerInRange((int32)EEquipmentType::None, (int32)EEquipmentType::Max);
	}

	FName characterType = FName(CHelpers::GetEnumToString(CharacterType, "ECharacter"));
	FName equipName = FName(CHelpers::GetEnumToString(EquipType, "EEquipmentType"));
	FName itemName = FName(ItemName);

	FEquipmentDataTables* equipDatas = DataTable->FindRow<FEquipmentDataTables>(characterType, FString(""));
	CheckNull(equipDatas);

	UDataTable* equipData = NULL;

	switch (EquipType)
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

	if (bRandom)
	{
		TArray<FName> rowNames = equipData->GetRowNames();
		CheckFalse(rowNames.Num() > 0);
		
		itemName = rowNames[UKismetMathLibrary::RandomIntegerInRange(0, rowNames.Num() - 1)];
		ItemName = itemName.ToString();

		bRandom = false;
	}

	FItem* itemData = equipData->FindRow<FItem>(itemName, "");
	CheckNull(itemData);

	ItemData = *itemData;
}
