#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CAction.h"
#include "CDoAction.generated.h"

UCLASS()
class UE4PROJECT_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetDatas(TArray<FDoActionData> InDatas) { Datas = InDatas; }
	FORCEINLINE void SetSkillDatas(TArray<FDoSkillData> InDatas) { SkillDatas = InDatas; }
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }

	FORCEINLINE TArray<FDoActionData>& GetDatas() { return Datas; }
	FORCEINLINE TArray<FDoSkillData>& GetSkillDatas() { return SkillDatas; }

public:
	ACDoAction();

public:
	// 액션을 수행(액션에 해당하는 몽타주 플레이)
	virtual void DoAction() {};
	// Index로 액션을 수행
	virtual void DoAction(const int32& InIndex) {};
	// 몽타주에 의해 시작
	virtual void Begin_DoAction() {};
	// DoSkill, DoAction 모두 이 노티파이로 종료
	virtual void End_Action() {};

	virtual void DoSkill(const FString& InSkillName) {};
	virtual void DoSkill(const int32& InIndex) {};

	// Aiming이 필요한 애들은 이것을 재정의
	virtual void OnAim() {}
	virtual void OffAim() {}

public:
	UFUNCTION()
		void RestoreRatio(); // 타이머로 OffsetRatio값 초기화

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	// CAttachment 충돌때 콜 될 함수
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {};
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) {};
	UFUNCTION()
		virtual void OnAttahmentCollision() {};
	UFUNCTION()
		virtual void OffAttahmentCollision() {};


protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

	UPROPERTY(BlueprintReadOnly)
		class UCStateComponent* State;

	UPROPERTY(BlueprintReadOnly)
		class UCStatusComponent* Status;

public:
	virtual void DoAction_RandomIndex() {}; // DoAction 사이즈내로 Random한 Index 생성
	virtual void DoSkill_RandomIndex() {}; // DoSkill 사이즈내로 Random한 Index 생성

public:
	FORCEINLINE int32 GetIndex() { return Index; }

protected:
	const bool* bEquipped;
	bool bSkill; // 스킬 상태중인지

	TArray<FDoActionData> Datas;
	TArray<FDoSkillData> SkillDatas;
	float OffsetRatio = 1.0f; // 공격속도 올리기 위해

	int32 Index; // 현재 몇번째 콤보중인지
};
