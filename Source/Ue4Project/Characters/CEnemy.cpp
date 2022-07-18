#include "Characters/CEnemy.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CItem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
#include "Actions/CAttachment.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<UWidgetComponent>(this, &NameWidget, "NameWidget", GetMesh());
	CHelpers::CreateComponent<UWidgetComponent>(this, &HealthWidget, "HealthWidget", GetMesh());

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "SkeletalMesh'/Game/Characters/Meshes/Minion/Buff_Red.Buff_Red'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Enemy/ABP_Buff_Red.ABP_Buff_Red_C'");
	GetMesh()->SetAnimInstanceClass(animInstance);

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	TSubclassOf<UCUserWidget_Name> nameClass;
	CHelpers::GetClass<UCUserWidget_Name>(&nameClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'");
	NameWidget->SetWidgetClass(nameClass);
	NameWidget->SetRelativeLocation(FVector(0, 0, 240));
	NameWidget->SetDrawSize(FVector2D(240, 30));
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCUserWidget_Health> healthClass;
	CHelpers::GetClass<UCUserWidget_Health>(&healthClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'");
	HealthWidget->SetWidgetClass(healthClass);
	HealthWidget->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidget->SetDrawSize(FVector2D(150, 10));
	HealthWidget->SetWidgetSpace(EWidgetSpace::Screen);

	CHelpers::GetClass<UCUserWidget_Health>(&BossClass, "WidgetBlueprint'/Game/Widgets/WB_Health_Boss.WB_Health_Boss_C'");
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	SetBodyMaterial();

	State->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	if (bBoss)
	{
		BossWidget = CreateWidget<UCUserWidget_Health, APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0), BossClass);
		BossWidget->AddToViewport();
		
		Cast<UCUserWidget_Health>(BossWidget)->Update(Status->GetHealth(), Status->GetMaxHealth());
		
		NameWidget->SetVisibility(false);
		HealthWidget->SetVisibility(false);

		return;
	}

	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>(NameWidget->GetUserWidgetObject())->SetNameText(GetActorLabel());

	HealthWidget->InitWidget();
	Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject())->Update(Status->GetHealth(), Status->GetMaxHealth());
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted: Hitted();	break;
	case EStateType::Dead: Dead();	break;
	}
}


float ACEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CheckTrueResult(State->IsDeadMode(), 0.0f);

	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	State->SetHittedMode();

	return Status->GetHealth();
}

void ACEnemy::Hitted()
{
	Status->SubHealth(DamageValue);

	UCUserWidget_Health* widget;
	bBoss ? widget = Cast<UCUserWidget_Health>(BossWidget) : widget = Cast<UCUserWidget_Health>(HealthWidget->GetUserWidgetObject());
	widget->Update(Status->GetHealth(), Status->GetMaxHealth());

	DamageValue = 0.0f;

	Status->SetStop();

	FTransform transform;
	FVector start;
	FVector target;

	ACActionCharacter* enemy = Cast<ACActionCharacter>(DamageInstigator->GetPawn());
	CheckNull(enemy);

	UCActionComponent* action = CHelpers::GetComponent<UCActionComponent>(enemy);
	CheckNull(action);

	ACAttachment* attachment = Cast<ACAttachment>(action->GetData()->GetAttachment());

	if (Status->GetHealth() <= 0.0f)
	{
		State->SetDeadMode();

		// Dead일 경우 EndTargeting
		UCTargetComponent* targeting = CHelpers::GetComponent<UCTargetComponent>(enemy);
		CheckNull(targeting);

		targeting->EndTargeting();

		return;
	}

	if (attachment)
	{
		start = GetActorLocation();
		target = attachment->GetActorLocation();
		transform.SetLocation(enemy->GetActorLocation());
		transform.SetRotation(UKismetMathLibrary::FindLookAtRotation(start, target).Quaternion());

		AActor* blood = GetWorld()->SpawnActorDeferred<AActor>(BloodClass, transform, this);
		// Blood는 BloodDuration 간격으로 Destory됨
		blood->InitialLifeSpan = BloodDuration;
		UGameplayStatics::FinishSpawningActor(blood, transform);
	}

	Montages->PlayHitted();

	start = GetActorLocation();
	target = enemy->GetActorLocation();
	FRotator rotation = GetActorRotation();
	SetActorRotation(FRotator(rotation.Pitch, UKismetMathLibrary::FindLookAtRotation(start, target).Yaw, rotation.Roll));
	DamageInstigator = NULL;

	if (!GetCharacterMovement()->IsFalling())
	{
		FVector direction = target - start;
		direction.Normalize();
		LaunchCharacter(-direction * LaunchAmount, true, false);
	}
	
	// Interface 함수를 호출할 시에는 Excute_ 키워드를 붙여야 하고
	// 인자로는 호출할 클래스를 넘긴다.
	Execute_ChangeColor(this, HittedColor);


	UCBehaviorComponent* behavior = CHelpers::GetComponent<UCBehaviorComponent>(GetController());
	CheckNull(behavior);

	// AngryMode(폭주상태)는 AngryColor로 바꿔줌
	if (behavior->IsAngryMode())
	{
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreAngryColor", 0.3f, false);

		return;
	}
	
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.3f, false);
}

void ACEnemy::Dead()
{
	CheckFalse(State->IsDeadMode());

	Action->OffAllCollision();
	Montages->PlayDead();
}

void ACEnemy::Begin_Dead()
{
	Action->DestroyAllActions();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (bBoss) BossWidget->SetVisibility(ESlateVisibility::Hidden);

	NameWidget->SetVisibility(false);
	HealthWidget->SetVisibility(false);
}


void ACEnemy::End_Dead()
{
	StopAnimMontage();
	UKismetSystemLibrary::K2_SetTimer(this, "FinishDead", 0.05f, true);
}

void ACEnemy::FinishDead()
{
	if (GetCurrentMontage() == NULL)
	{
		// 아이템을 Drop시킴
		FTransform transform;
		FVector location = GetActorLocation();
		transform.SetLocation(FVector(location.X, location.Y, location.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 20.0f));

		ACItem* item = GetWorld()->SpawnActorDeferred<ACItem>(ACItem::StaticClass(), transform);

		FString str = "Drop";
		str.Append("_");
		str.Append(item->GetActorLabel());
		item->SetActorLabel(str);

		// RandomItem 생성
		item->SetRandomItem(true);
		item->SetItem();

		UKismetSystemLibrary::K2_ClearTimer(this, "FinishDead");
		
		Destroy();

		UGameplayStatics::FinishSpawningActor(item, transform);
	}
}

void ACEnemy::RestoreColor()
{
	Execute_ChangeColor(this, FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
}

void ACEnemy::RestoreAngryColor()
{
	Execute_ChangeColor(this, AngryColor);
}