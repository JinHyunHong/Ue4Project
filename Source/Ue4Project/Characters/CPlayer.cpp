#include "CPlayer.h"
#include "Global.h"
#include "Actors/CItem.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CInventoryComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCInventoryComponent>(this, &Inventory, "Inventory");

	SpringArm->SetRelativeLocation(FVector(9, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 250.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
}

void ACPlayer::BeginPlay()
{
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
	
	PlayerInputComponent->BindAction("ViewEquipment", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewEquipmnent);
	PlayerInputComponent->BindAction("ViewInventory", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewInventory);

	PlayerInputComponent->BindAction("PickUp", EInputEvent::IE_Pressed, this, &ACPlayer::OnPickUp);
}

void ACPlayer::OnMoveFoward(float InAxis)
{
	CheckFalse(State->CanMove());

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{
	CheckFalse(State->CanMove());

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

	Inventory->AddItem(item->GetCharacterType(), item->GetEquipmentType(), item->GetItemName(), item->GetItemData());

	InteractItem->Destroy();
	InteractItem = NULL;
}