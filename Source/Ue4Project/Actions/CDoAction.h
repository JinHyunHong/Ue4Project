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
	// �׼��� ����(�׼ǿ� �ش��ϴ� ��Ÿ�� �÷���)
	virtual void DoAction() {};
	// ��Ÿ�ֿ� ���� ����
	virtual void Begin_DoAction() {};
	// ��Ÿ�ֿ� ���� ����
	virtual void End_DoAction() {};

	virtual void DoSkill(const FString& InSkillName) {};
	virtual void DoSkill(const int32& InIndex) {};

	// Aiming�� �ʿ��� �ֵ��� �̰��� ������
	virtual void OnAim() {}
	virtual void OffAim() {}

public:
	UFUNCTION()
		void RestoreRatio(); // Ÿ�̸ӷ� OffsetRatio�� �ʱ�ȭ

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	// CAttachment �浹�� �� �� �Լ�
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

protected:
	const bool* bEquipped;
	bool bSkill; // ��ų ����������

	TArray<FDoActionData> Datas;
	TArray<FDoSkillData> SkillDatas;
	float OffsetRatio = 1.0f; // ���ݼӵ� �ø��� ����
};
