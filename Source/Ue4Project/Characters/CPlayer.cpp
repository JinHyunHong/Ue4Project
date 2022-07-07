#include "CPlayer.h"
#include "Global.h"
#include "Actions/CItem.h"
#include "Widgets/CUserWidget_SkillList.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CIKComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CEquipComponent.h"
#include "Components/CInventoryComponent.h"	
#include "Components/CapsuleComponent.h"
#include "Sound/SoundCue.h"
#include "Actions/CAction.h"
#include "Actions/CDoAction.h"
#include "Widgets/CUserWidget_SkillSlot.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateComponent<USceneComponent>(this, &TargetPoint, "TargetPoint", GetMesh());

	CHelpers::CreateActorComponent<UCOptionComponent>(this, &Option, "Option");
	CHelpers::CreateActorComponent<UCIKComponent>(this, &IK, "IK");
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target");
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

	CHelpers::GetClass<UCUserWidget_SkillList>(&SkillListClass, "WidgetBlueprint'/Game/Widgets/WB_SkillList.WB_SkillList_C'");

	TargetPoint->SetRelativeLocation(FVector(0, 90.0f, 110.0f));
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

	Action->SetAction("Unarmed");

	SkillList = CreateWidget<UCUserWidget_SkillList, APlayerController>(GetController<APlayerController>(), SkillListClass);
	SkillList->AddToViewport();

	FTransform transform;
	AirDashEffect ? AirDash = UNiagaraFunctionLibrary::SpawnSystemAttached(AirDashEffect, GetMesh(), "", transform.GetLocation(), transform.GetRotation().Rotator(), EAttachLocation::KeepRelativeOffset, false, false) : AirDash = NULL;

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
	PlayerInputComponent->BindAction("Air", EInputEvent::IE_Pressed, this, &ACPlayer::OnAir);

	PlayerInputComponent->BindAction("Target", EInputEvent::IE_Pressed, this, &ACPlayer::OnTarget);

	PlayerInputComponent->BindAction("TargetLeft", EInputEvent::IE_Pressed, this, &ACPlayer::OnTargetLeft);
	PlayerInputComponent->BindAction("TargetRight", EInputEvent::IE_Pressed, this, &ACPlayer::OnTargetRight);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACPlayer::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACPlayer::OffRun);
	PlayerInputComponent->BindAction("Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid);
	PlayerInputComponent->BindAction("ViewEquipment", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewEquipmnent);
	PlayerInputComponent->BindAction("ViewInventory", EInputEvent::IE_Pressed, this, &ACPlayer::OnViewInventory);
	PlayerInputComponent->BindAction("PickUp", EInputEvent::IE_Pressed, this, &ACPlayer::OnPickUp);

	PlayerInputComponent->BindAction("Skill1", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoSkill1);
	PlayerInputComponent->BindAction("Skill2", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoSkill2);
	PlayerInputComponent->BindAction("Skill3", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoSkill3);
	PlayerInputComponent->BindAction("Skill4", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoSkill4);
}

FGenericTeamId ACPlayer::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
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
		case EStateType::Hitted: Hitted(); break;
		case EStateType::Dead: Dead(); break;
	}
}

void ACPlayer::OnEquipChanged(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	if (InEquipType == EEquipmentType::Weapon) 
		Action->SetAction(InItemName);

	// Add Equipment Data
	Inventory->AddEquip(CharacterType, InEquipType, InItemName, InItemData);
	
	UCAction* action = Action->GetData();
	CheckNull(action);
	ACDoAction* doAction = action->GetDoAction();
	CheckNull(doAction);

	for (FDoSkillData& data : doAction->GetSkillDatas())
		SkillList->AddSkill(data.SkillName.ToString(), data.SumNail, data.MaxCoolTime);
}

void ACPlayer::OnUnequipChanged(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData)
{
	if (InEquipType == EEquipmentType::Weapon)
		Action->SetAction("Unarmed");

	// Add Inventory Data
	Inventory->AddItem(CharacterType, InEquipType, InItemName, InItemData);
	
	SkillList->ClearList();
}

void ACPlayer::OnRun()
{
	if (GetCharacterMovement()->IsFalling())
	{
		AirDash ? AirDash->Activate() : AirDash = NULL;

		ACharacter* enemy = Target->GetTarget();
		CheckNull(enemy);

		// 공중 중에 적 주위에서 랜덤으로 이동함
		FVector location = enemy->GetActorLocation();
		location.X = UKismetMathLibrary::RandomFloatInRange(location.X - RandomAirX, location.X + RandomAirX);
		location.Y = UKismetMathLibrary::RandomFloatInRange(location.Y - RandomAirY, location.Y + RandomAirY);
		SetActorLocation(location);

		return;

	}

	Status->SetSpeed(ECharacterSpeed::Run);
}

void ACPlayer::OffRun()
{
	if (AirDash)
	{
		AirDash->Deactivate();
		return;
	}

	Status->SetSpeed(ECharacterSpeed::Walk);
}

void ACPlayer::OnAvoid()
{
	CheckTrue(GetCharacterMovement()->IsFalling());
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
	AirFalling();
	
	Action->DoAction();
}

void ACPlayer::OnAir()
{
	CheckTrue(GetCharacterMovement()->IsFalling());
	CheckFalse(Status->CanMove());
	CheckTrue(GetVelocity().Size2D() > 0); 
	CheckTrue(State->IsActionMode());

	// Targeting을 해서 Enemy를 구함
	Target->StartTargeting();

	ACharacter* enemy = Target->GetTarget();
	CheckNull(enemy);

	// Player를 공중에 띄움
	LaunchCharacter(GetActorUpVector() * 1000.0f, true, true);

	enemy->LaunchCharacter(enemy->GetActorUpVector() * 900.0f, true, true);

	// 캐릭터랑 같은 높이에 있게 함
	enemy->SetActorLocation(TargetPoint->GetComponentLocation());

	// Enemy가 Player를 바라봄
	FVector start = enemy->GetActorLocation();
	FVector target = GetActorLocation();
	FRotator rotation = GetActorRotation();
	enemy->SetActorRotation(FRotator(rotation.Pitch, UKismetMathLibrary::FindLookAtRotation(start, target).Yaw, rotation.Roll));
}

void ACPlayer::AirFalling()
{
	CheckFalse(GetCharacterMovement()->IsFalling());

	ACharacter* enemy = Target->GetTarget();
	CheckNull(enemy);

	LaunchCharacter(GetActorUpVector() * 200.0f, false, true);

	enemy->LaunchCharacter(enemy->GetActorUpVector() * 200.0f, false, true);
}

void ACPlayer::OnDoSkill1()
{
	CheckTrue(State->IsActionMode());

	UCUserWidget_SkillSlot* slot = SkillList->GetSlot(0);
	CheckNull(slot);

	CheckFalse(slot->GetEnable());
	AirFalling();
	Action->DoSkill(0);
	slot->RestartCoolTime();
}

void ACPlayer::OnDoSkill2()
{
	CheckTrue(State->IsActionMode());

	UCUserWidget_SkillSlot* slot = SkillList->GetSlot(1);
	CheckNull(slot);

	CheckFalse(slot->GetEnable());
	AirFalling();
	Action->DoSkill(1);
	slot->RestartCoolTime();
}

void ACPlayer::OnDoSkill3()
{
	CheckTrue(State->IsActionMode());

	UCUserWidget_SkillSlot* slot = SkillList->GetSlot(2);
	CheckNull(slot);

	CheckFalse(slot->GetEnable());
	AirFalling();
	Action->DoSkill(2);
	slot->RestartCoolTime();
}

void ACPlayer::OnDoSkill4()
{
	CheckTrue(State->IsActionMode());

	UCUserWidget_SkillSlot* slot = SkillList->GetSlot(3);
	CheckNull(slot);

	CheckFalse(slot->GetEnable());
	AirFalling();
	Action->DoSkill(3);
	slot->RestartCoolTime();
}

void ACPlayer::OnTarget()
{
	Target->ToggleTarget();
}

void ACPlayer::OnTargetLeft()
{
	Target->ChangeTargetLeft();
}

void ACPlayer::OnTargetRight()
{
	Target->ChangeTargetRight();
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

float ACPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACPlayer::Hitted()
{
	Status->SubHealth(DamageValue);
	DamageValue = 0.0f;

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();

		return;
	}

	Montages->PlayHitted();
}


void ACPlayer::Dead()
{
	CheckFalse(State->IsDeadMode());

	Montages->PlayDead();
}

void ACPlayer::Begin_Dead()
{
	Action->OffAllCollision();
	Action->DestroyAllActions();
}

void ACPlayer::End_Dead()
{
}

void ACPlayer::PlayFootstep()
{
	CheckTrue(GetVelocity().Size2D() <= 0.0f)
	CheckFalse(State->IsIdleMode());

	Audio->Stop();
	Audio->SetSound(Footstep);
	Audio->Play();
}