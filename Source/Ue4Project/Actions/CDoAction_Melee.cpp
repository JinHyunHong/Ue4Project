#include "Actions/CDoAction_Melee.h"
#include "Global.h"
#include "Characters/CActionCharacter.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

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


	CheckNull(Datas[0].AnimMontage);
	// IdleMode���� ��
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	const FDoActionData& data = Datas[0];

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio * OffsetRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();		
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	CheckNull(Datas[Index].AnimMontage);

	bExist = false;

	// ���� �÷��� ���� ��� ��Ÿ�ָ� ������Ŵ
	// ���Ŀ� Fist�� ������ ������ ��� ����Ϸ��� ���� �ڵ�
	// ���Ŀ� �ٽ� ����
	OwnerCharacter->StopAnimMontage();

	Index++;

	const FDoActionData& data = Datas[Index];
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio * OffsetRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	// ���� ��Ÿ�� �ߴ� ��Ŵ
	OwnerCharacter->StopAnimMontage();

	State->SetIdleMode();
	Status->SetMove();

	if (bSkill && SkillDatas[Index].OffsetRatio > 1.0f)
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreRatio", SkillDatas[Index].RatioDuration, false);

	// �ٽ� ó������ ������ �� �ְ� ó��
	Index = 0;
	bSkill = false;

	// ���� Effect ����
	Niagara ? Niagara->DestroyComponent() : Niagara = NULL;
}

void ACDoAction_Melee::DoSkill(const FString& InSkillName)
{
	Super::DoSkill(InSkillName);

	CheckFalse(State->IsIdleMode());

	for (int32 i = 0; i < SkillDatas.Num(); i++)
	{
		if (SkillDatas[i].SkillName.ToString() == InSkillName && SkillDatas[i].OffsetRatio > 1.0f)
		{
			CheckNull(SkillDatas[i].AnimMontage);

			State->SetActionMode();
			OwnerCharacter->PlayAnimMontage(SkillDatas[i].AnimMontage, SkillDatas[i].PlayRatio * OffsetRatio, SkillDatas[i].StartSection);
			OffsetRatio = SkillDatas[i].OffsetRatio;
			
			SkillDatas[i].bCanMove ? Status->SetMove() : Status->SetStop();

			Index = i;
			bSkill = true;

			return;
		}
	}
}

void ACDoAction_Melee::DoSkill(const int32& InIndex)
{
	Super::DoSkill(InIndex);

	CheckNull(SkillDatas[InIndex].AnimMontage);
	CheckFalse(State->IsIdleMode());

	const FDoSkillData& data = SkillDatas[InIndex];

	State->SetActionMode();
	
	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio * OffsetRatio, data.StartSection);
	if (data.OffsetRatio > 1.0f) OffsetRatio = data.OffsetRatio;

	data.bCanMove ? Status->SetMove() : Status->SetStop();

	Index = InIndex;
	bSkill = true;
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);
	CheckNull(InOtherCharacter);

	ACActionCharacter* owner = Cast<ACActionCharacter>(OwnerCharacter);
	CheckNull(owner);

	for (const ACharacter* other : HittedCharacters)
	{
		if (InOtherCharacter == other)
			return;
	}
	HittedCharacters.Add(InOtherCharacter);

	FDoActionData data;
	bSkill ? data = SkillDatas[Index] : data = Datas[Index];

	UParticleSystem* hitEffect = data.Effect;
	if (!!hitEffect)
	{
		FTransform transform = data.EffectTransform;
		// EffectTransform ���� ��ŭ �̵�
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	UNiagaraSystem* niagara = data.NiagaraEffect;
	if (!!niagara)
	{
		// ���� Effect ����
		Niagara ? Niagara->DestroyComponent() : Niagara = NULL;

		FTransform transform = data.NiagaraTransform;
		transform.AddToTranslation(owner->GetActorLocation());
		transform.SetRotation(owner->GetActorRotation().Quaternion());
		
		Niagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D());
	}

	float hitStop = data.HitStop;
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

	TSubclassOf<UMatineeCameraShake> shake = data.ShakeClass;
	if (!!shake)
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->StartCameraShake(shake);


	if (!!data.Sound)
	{
		UAudioComponent* audio = CHelpers::GetComponent<UAudioComponent>(owner);
		data.Sound->VolumeMultiplier = data.Volume;
		audio->Stop();
		audio->SetSound(data.Sound);
		audio->Play();
	}


	// ������ �߰� ������ �ѱ涧
	FDamageEvent e;

	// 1. ������, 2. ������ �̺�Ʈ, 3 : Instigator(��Ʈ�ѷ�), 4 : DamageCursor(������ �߱��ϴ� ģ��)
	InOtherCharacter->TakeDamage(data.Power, e, OwnerCharacter->GetController(), this);
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
