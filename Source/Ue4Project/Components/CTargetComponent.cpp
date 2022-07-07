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

	// �⺻ �� ����
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

	// Ÿ���� ����ߴ����� üũ�ؾ� �ϴϱ�
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(Target);

	// Ÿ������ ������ �� �ִ� ���� 2��
	// 1. �����Ÿ��� �������
	// 2. DeadMode 
	// GetDistanceTo() : �Ÿ��� ����

	bool b = false;
	// b�� false�ϱ� �ڿ��� true�� true�� ��
	b |= state->IsDeadMode();
	b |= Target->GetDistanceTo(OwnerCharacter) >= TraceRadius;
	// &�� �ϰ� �ȴٸ� b�� true�� �ְ� �ϸ� �ȴ�.

	// ������ ��� �þ����� �� ���� ����
	if (b == true)
	{
		EndTargeting();

		return;
	}


	// ��, �ٶ󺸴� ������ ȸ�������� ���õǵ��� ����(left, right target ���� �ذ�)

	FVector start = OwnerCharacter->GetActorLocation();
	FVector target = Target->GetActorLocation();

	// start���� target�� �ٶ󺸴� ȸ������ �������
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	FRotator current = OwnerCharacter->GetControlRotation();

	// RInterpTo : ���� ���⿡�� ���� ������� �ð��� ����� �ӵ��� �ڿ������� �������ִ� �Լ�
	rotator = UKismetMathLibrary::RInterpTo(current, rotator, DeltaTime, interopSpeed);

	// SetControlRotation()�� private�̾ Contoller�� ���� �����;���
	OwnerCharacter->GetController()->SetControlRotation(rotator);
}


void UCTargetComponent::ToggleTarget()
{
	if (!!Target)
	{
		EndTargeting();

		return;
	}

	// ���� Ÿ������ ���ٸ� ������
	StartTargeting();
}

void UCTargetComponent::StartTargeting()
{
	SetTraceTargets();
	SetTarget();
}

void UCTargetComponent::EndTargeting()
{
	// EndTargeting������ �ʱ�ȭ ����
	Target = NULL;
	TraceTargets.Empty();

	if (!!Attached)
		Attached->DestroyComponent();
}

void UCTargetComponent::SetTraceTargets()
{
	FVector start = OwnerCharacter->GetActorLocation();
	// Sphere Multi ����� �ǵ�, �̰� ��ġ�� ������ �ȳ���(�׷��� ��ġ�� ��¦ ����)
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

		// casting �Ǿ��ٸ� AddUnique()�� �ߺ� �ȵǰ� �־���
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

	// �Ÿ������� ���Ⱚ float
	TMap<float, ACharacter*> map;

	for (ACharacter* character : TraceTargets)
	{
		if (Target == character)
			continue;

		FVector targetLocation = character->GetActorLocation();
		FVector ownerLocation = OwnerCharacter->GetActorLocation();
		// owner�� target�� �ٶ���� �ϴϱ�
		FVector ownerToTarget = targetLocation - ownerLocation;

		// ī�޶��� ���� ��
		FQuat quat = FQuat(OwnerCharacter->GetControlRotation());
		// forwar, up ������
		FVector forward = quat.GetForwardVector();
		FVector up = quat.GetUpVector();


		// ������ ���� ������ ���� �Ÿ����� �����̴�.
		FVector cross = forward ^ ownerToTarget;
		// cross�� up�� �����ؼ� z���� ������ ��
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
			// Ű�� 0���� �۰ų� ���� Ű���̴�.
			if (key < 0.0f)
				continue;
		}

		// �̹����� left�ϱ� 0���� ū �ֵ��� �߶�
		else
		{
			if (key > 0.0f)
				continue;
		}

		// ũ�ٸ� �ѱ�(�ּҰ��� ���ҰŶ�)
		if (FMath::Abs(key) > min)
			continue;

		// ���� ����� = �ּҰ��� target�� ����
		min = FMath::Abs(key);
		target = *map.Find(key);
	}

	ChangeCursor(target);
}

void UCTargetComponent::ChangeCursor(class ACharacter* InTarget)
{
	if (!!InTarget)
	{
		// �̹� ������Ʈ�� �����Ѵٸ� �����ض�
		if (!!Attached)
			Attached->DestroyComponent();

		// ���Ͽ��� ��ƼŬ�� ����
		FTransform transform;
		Attached = UNiagaraFunctionLibrary::SpawnSystemAttached(Niagara, InTarget->GetMesh(), "", transform.GetLocation(), transform.GetRotation().Rotator(), EAttachLocation::KeepRelativeOffset, false);

		// Ÿ�� ����
		Target = InTarget;

		return;
	}

	// Null�̶�� EndTargeting���� ����.
	EndTargeting();
}
