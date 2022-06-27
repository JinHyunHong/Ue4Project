#include "CPlayer.h"
#include "Global.h"
#include "Actions/CItem.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CEquipComponent.h"
#include "Components/CInventoryComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UCInventoryComponent>(this, &Inventory, "Inventory");

	// 사방을 뛰어다닐 수 있게(카메라의 회전에 따라 회전하지 않음)
	bUseControllerRotationYaw = false;

	SpringArm->SetRelativeLocation(FVector(9, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	// 캐릭터의 회전값을 높여서 회전가능하도록 처리
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACPlayer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}


void ACPlayer::BeginPlay()
{
	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	Equip->OnEquipChanged.AddDynamic(this, &ACPlayer::OnEquipChanged);
	Equip->OnUnequipChanged.AddDynamic(this, &ACPlayer::OnUnequipChanged);
	
	Status->SetSpeed(ECharacterSpeed::Walk);

	Super::BeginPlay();

}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveFoward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACPlayer::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACPlayer::OffRun);
	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);
	PlayerInputComponent->BindAction("ViewEquipment", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewEquipmnent);
	PlayerInputComponent->BindAction("ViewInventory", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewInventory);
	PlayerInputComponent->BindAction("PickUp", EInputEvent::IE_Pressed, this, &ACPlayer::OnPickUp);
}

void ACPlayer::OnMoveFoward(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(Status->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnHorizontalLook(float InAxis)
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}

void ACPlayer::OnVerticalLook(float InAxis)
{
	float rate = Option->GetVerticalLookRate();
	AddControllerPitchInput(InAxis * rate * GetWorld()->GetDeltaSeconds());
}


void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Roll: Begin_Roll(); break;
		case EStateType::Backstep: Begin_Backstep(); break;
	}
}

void ACPlayer::OnEquipChanged(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	if (InEquipType == EEquipmentType::Weapon) 
		Action->SetAction(InItemName);

	// Add Equipment Data
	Inventory->AddEquip(CharacterType, InEquipType, InItemName, InItemData);
}

void ACPlayer::OnUnequipChanged(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	if (InEquipType == EEquipmentType::Weapon)
		Action->SetAction("Unarmed");

	// Add Inventory Data
	Inventory->AddItem(CharacterType, InEquipType, InItemName, InItemData);
}

void ACPlayer::OnRun()
{
	Status->SetSpeed(ECharacterSpeed::Run);
}

void ACPlayer::OffRun()
{
	Status->SetSpeed(ECharacterSpeed::Walk);
}

void ACPlayer::OnAvoid()
{
	CheckFalse(Status->CanMove());
	CheckFalse(State->IsIdleMode());

	if (InputComponent->GetAxisValue("MoveForward") < 0.0f)
	{
		State->SetBackstepMode();
		
		return;
	}

	State->SetRollMode();
}


void ACPlayer::OnViewEquipmnent()
{
	Inventory->ViewEquipment();
}

void ACPlayer::OnViewInventory()
{
	Inventory->ViewInventory();
}

void ACPlayer::OnPickUp()
{
	CheckNull(InteractItem);

	ACItem* item = Cast<ACItem>(InteractItem);
	CheckNull(item);

	Inventory->AddItem(item->GetCharacterType(), item->GetEquipmentType(), item->GetItemName(), item->GetItemData(), true);

	InteractItem->Destroy();
	InteractItem = NULL;
}

void ACPlayer::OnDoAction()
{
	Action->DoAction();
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector from = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, from));

	Montages->PlayRoll();
}

void ACPlayer::End_Roll()
{
	if (Action->IsUnarmedMode() == false)
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	State->SetIdleMode();
}

void ACPlayer::Begin_Backstep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackStep();
}

void ACPlayer::End_Backstep()
{
	if (Action->IsUnarmedMode())
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;

	}

	State->SetIdleMode();
}

void ACPlayer::Dead()
{
	CheckFalse(State->IsDeadMode());

	// Montages->PlayDead();
}

void ACPlayer::Begin_Dead()
{
	Action->OffAllCollision();
	Action->DestroyAllActions();
}

void ACPlayer::End_Dead()
{
}