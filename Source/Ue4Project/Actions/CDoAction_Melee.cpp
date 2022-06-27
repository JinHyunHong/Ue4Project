#include "Actions/CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();
	// ������ ������ 0���� Ŀ����
	CheckFalse(Datas.Num() > 0);

	// �����޺��� ���Ǿ��ٸ�
	if (bEnable == true)
	{
		// �����޺��� �ִٰ� �˷���
		bExist = true;
		// �޺� �Է� ���̻� ���� ����(�̹� �������ϱ�)
		bEnable = false;

		return;
	}


	// IdleMode���� ��
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	const FDoActionData& data = Datas[0];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	bExist = false;

	// ���� �÷��� ���� ��� ��Ÿ�ָ� ������Ŵ
	// ���Ŀ� Fist�� ������ ������ ��� ����Ϸ��� ���� �ڵ�
	// ���Ŀ� �ٽ� ����
	OwnerCharacter->StopAnimMontage();

	Index++;

	const FDoActionData& data = Datas[Index];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	const FDoActionData& data = Datas[Index];
	// ���� ��Ÿ�� �ߴ� ��Ŵ
	OwnerCharacter->StopAnimMontage(data.AnimMontage);

	State->SetIdleMode();
	Status->SetMove();

	// �ٽ� ó������ ������ �� �ְ� ó��
	Index = 0;
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}
	HittedCharacters.Add(InOtherCharacter);


	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		// EffectTransform ���� ��ŭ �̵�
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	float hitStop = Datas[Index].HitStop;
	// 0 �̶� ������ ������ ��
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		// 1e-3f -> 10^-3 -> 1 / 1000�� -> 0.001
		// 1000�� ���� ������ ����
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
		// ���尡 �������� Ÿ�̸ӵ� ��������. �׷��� �ٽ� �� �ð���ŭ ���ؼ� ȸ����Ŵ
		// ��, hitStop��ŭ ������ ��Ŵ
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-3f, false);
	}

	TSubclassOf<UMatineeCameraShake> shake = Datas[Index].ShakeClass;
	if (!!shake)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);

	// ������ �߰� ������ �ѱ涧
	FDamageEvent e;

	// 1. ������, 2. ������ �̺�Ʈ, 3 : Instigator(��Ʈ�ѷ�), 4 : DamageCursor(������ �߱��ϴ� ģ��)
	InOtherCharacter->TakeDamage(Datas[Index].Power, e, OwnerCharacter->GetController(), this);
}


void ACDoAction_Melee::OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter)
{
}

void ACDoAction_Melee::OnAttahmentCollision()
{
}

void ACDoAction_Melee::OffAttahmentCollision()
{
	HittedCharacters.Empty();
}

void ACDoAction_Melee::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

