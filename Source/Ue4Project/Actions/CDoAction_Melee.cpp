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
	// 데이터 개수가 0보다 커야함
	CheckFalse(Datas.Num() > 0);

	// 다음콤보가 허용되었다면
	if (bEnable == true)
	{
		// 다음콤보가 있다고 알려줌
		bExist = true;
		// 콤보 입력 더이상 하지 말라(이미 들어왔으니까)
		bEnable = false;

		return;
	}


	CheckNull(Datas[0].AnimMontage);
	// IdleMode여야 함
	CheckFalse(State->IsIdleMode());
	State->SetActionMode();

	const FDoActionData& data = Datas[0];

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio * OffsetRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();		
}

void ACDoAction_Melee::DoAction(const int32& InIndex)
{
	Super::DoAction();

	CheckFalse(Datas.Num() > 0);
	CheckNull(Datas[InIndex].AnimMontage);
	CheckFalse(State->IsIdleMode());
	
	State->SetActionMode();

	const FDoActionData& data = Datas[InIndex];

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio * OffsetRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}


void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bExist);
	CheckNull(Datas[Index].AnimMontage);

	bExist = false;

	// 현재 플레이 중인 모든 몽타주를 중지시킴
	// 이후에 Fist의 마지막 동작을 끊어서 사용하려고 넣은 코드
	// 이후에 다시 설명
	OwnerCharacter->StopAnimMontage();

	Index++;

	CheckFalse(Datas.Num() > Index);

	const FDoActionData& data = Datas[Index];

	OwnerCharacter->PlayAnimMontage(data.AnimMontage, data.PlayRatio * OffsetRatio, data.StartSection);

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_Action()
{
	Super::End_Action();

	// 현재 몽타주 중단 시킴
	OwnerCharacter->StopAnimMontage();

	State->SetIdleMode();
	Status->SetMove();

	if (bSkill && SkillDatas[Index].OffsetRatio > 1.0f)
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreRatio", SkillDatas[Index].RatioDuration, false);

	// 다시 처음부터 공격할 수 있게 처리
	Index = 0;
	bSkill = false;

	// 기존 Effect 제거
	if (!!Niagara)
	{
		Niagara->DestroyComponent();
		Niagara = NULL;
	}
}

void ACDoAction_Melee::DoSkill(const FString& InSkillName)
{
	Super::DoSkill(InSkillName);

	CheckFalse(SkillDatas.Num() > 0);
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

	CheckFalse(SkillDatas.Num() > 0);
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
		// EffectTransform 보정 만큼 이동
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	UNiagaraSystem* niagara = data.NiagaraEffect;
	if (!!niagara)
	{
		FTransform transform = data.NiagaraTransform;
		transform.AddToTranslation(owner->GetActorLocation());
		transform.SetRotation(owner->GetActorRotation().Quaternion());
		
		Niagara = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D());
	}

	float hitStop = data.HitStop;
	// 0 이랑 가깝지 않으면 함
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		// 1e-3f -> 10^-3 -> 1 / 1000초 -> 0.001
		// 1000배 정도 느리게 만듬
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-3f);
		// 월드가 느려지만 타이머도 느려진다. 그래서 다시 그 시간만큼 곱해서 회복시킴
		// 즉, hitStop만큼 딜레이 시킴
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


	// 데미지 추가 정보를 넘길때
	FDamageEvent e;

	// 1. 데미지, 2. 데미지 이벤트, 3 : Instigator(컨트롤러), 4 : DamageCursor(데미지 야기하는 친구)
	InOtherCharacter->TakeDamage(data.Power, e, OwnerCharacter->GetController(), this);
}


void ACDoAction_Melee::OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter)
{
	if (!!Niagara)
	{
		Niagara->DestroyComponent();
		Niagara = NULL;
	}
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

void ACDoAction_Melee::DoAction_RandomIndex()
{
	Super::DoAction_RandomIndex();

	CheckFalse(Datas.Num() > 0);

	Index = UKismetMathLibrary::RandomIntegerInRange(0, Datas.Num() - 1);
}

void ACDoAction_Melee::DoSkill_RandomIndex()
{
	Super::DoSkill_RandomIndex();

	CheckFalse(SkillDatas.Num() > 0);

	Index = UKismetMathLibrary::RandomIntegerInRange(0, SkillDatas.Num() - 1);
}