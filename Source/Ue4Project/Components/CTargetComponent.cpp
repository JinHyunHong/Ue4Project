#include "CTargetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// 기본 값 설정
	CHelpers::GetAsset<UNiagaraSystem>(&Niagara, "NiagaraSystem'/Game/RPGEffects/ParticlesNiagara/TargetingDecals/NS_Targeting_Circle1.NS_Targeting_Circle1'");
}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckNull(Target);

	// 타겟이 사망했는지를 체크해야 하니까
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target);

	// 타겟팅이 해제될 수 있는 조건 2개
	// 1. 일정거리를 벗어났을때
	// 2. DeadMode 
	// GetDistanceTo() : 거리를 구함

	bool b = false;
	// b가 false니까 뒤에가 true면 true가 됨
	b |= state->IsDeadMode();
	b |= Target->GetDistanceTo(OwnerCharacter) >= TraceRadius;
	// &로 하게 된다면 b를 true로 주고 하면 된다.

	// 옆으로 계속 늘어지는 거 보다 좋음
	if (b == true)
	{
		EndTargeting();

		return;
	}


	// 즉, 바라보는 방향의 회전값으로 세팅되도록 만듬(left, right target 버그 해결)

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();

	// start에서 target을 바라보는 회전값이 만들어짐
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();

	// RInterpTo : 시작 방향에서 종료 방향까지 시간에 비례한 속도로 자연스럽게 보간해주는 함수
	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, interopSpeed);

	// SetControlRotation()은 private이어서 Contoller를 통해 가져와야함
	OwnerCharacter->GetController()->SetControlRotation(rotator);
}


void UCTargetComponent::ToggleTarget()
{
	if (!!Target)
	{
		EndTargeting();

		return;
	}

	// 기존 타켓팅이 없다면 수행함
	StartTargeting();
}

void UCTargetComponent::StartTargeting()
{
	SetTraceTargets();
	SetTarget();
}

void UCTargetComponent::EndTargeting()
{
	// EndTargeting에서는 초기화 해줌
	Target = NULL;
	TraceTargets.Empty();

	if (!!Attached)
		Attached->DestroyComponent();
}

void UCTargetComponent::SetTraceTargets()
{
	FVector start = OwnerCharacter->GetActorLocation();
	// Sphere Multi 사용할 건데, 이건 위치가 같으면 안나옴(그래서 위치만 살짝 높임)
	FVector end = FVector(start.X, start.Y, start.Z + 1);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, TraceRadius, "Pawn", false, ignoreActors, Debug, hitResults, true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	for (const FHitResult& result : hitResults)
	{
		if (result.GetActor()->GetClass() == OwnerCharacter->GetClass())
			continue;

		ACharacter* character = Cast<ACharacter>(result.GetActor());

		// casting 되었다면 AddUnique()로 중복 안되게 넣어줌
		if (!!character)
			TraceTargets.AddUnique(character);
	}
}

void UCTargetComponent::SetTarget()
{
	float angle = -2.0f;
	ACharacter* target = NULL;

	for (ACharacter* character : TraceTargets)
	{
		FVector direction = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		FVector offset = character->GetActorLocation() - OwnerCharacter->GetActorLocation();

		offset = offset.GetSafeNormal();

		float temp = direction | offset;

		if (temp < angle)
			continue;

		angle = temp;
		target = character;
	}
	ChangeCursor(target);
}

void UCTargetComponent::ChangeTargetLeft()
{
	ChangeTarget(false);
}

void UCTargetComponent::ChangeTargetRight()
{
	ChangeTarget(true);
}

void UCTargetComponent::ChangeTarget(bool InRight)
{
	CheckNull(Target);

	// 거리값이자 방향값 float
	TMap<float, ACharacter*> map;

	for (ACharacter* character : TraceTargets)
	{
		if (Target == character)
			continue;

		FVector targetLocation = character->GetActorLocation();
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		// owner가 target을 바라봐야 하니까
		FVector ownerToTarget = targetLocation - ownerLocation;

		// 카메라의 방향 값
		FQuat quat = FQuat(OwnerCharacter->GetControlRotation());
		// forwar, up 구해줌
		FVector forward = quat.GetForwardVector();
		FVector up = quat.GetUpVector();


		// 나오는 수직 백터의 값이 거리이자 방향이다.
		FVector cross = forward ^ ownerToTarget;
		// cross와 up을 내적해서 z값만 가지고 옴
		float dot = cross | up;

		map.Add(dot, character);
	}

	float min = FLT_MAX;
	ACharacter* target = NULL;

	TArray<float> keys;
	map.GetKeys(keys);
	for (float key : keys)
	{
		if (InRight == true)
		{
			// 키가 0보다 작거나 왼쪽 키값이다.
			if (key < 0.0f)
				continue;
		}

		// 이번에는 left니까 0보다 큰 애들을 잘라냄
		else
		{
			if (key > 0.0f)
				continue;
		}

		// 크다면 넘김(최소값을 구할거라)
		if (FMath::Abs(key) > min)
			continue;

		// 가장 가까운 = 최소값의 target을 넣음
		min = FMath::Abs(key);
		target = *map.Find(key);
	}

	ChangeCursor(target);
}

void UCTargetComponent::ChangeCursor(class ACharacter* InTarget)
{
	if (!!InTarget)
	{
		// 이미 컴포넌트가 존재한다면 제거해라
		if (!!Attached)
			Attached->DestroyComponent();

		// 소켓에다 파티클을 붙임
		FTransform transform;
		Attached = UNiagaraFunctionLibrary::SpawnSystemAttached(Niagara, InTarget->GetMesh(), "", transform.GetLocation(), transform.GetRotation().Rotator(), EAttachLocation::KeepRelativeOffset, false);

		// 타겟 지정
		Target = InTarget;

		return;
	}

	// Null이라면 EndTargeting으로 간다.
	EndTargeting();
}
