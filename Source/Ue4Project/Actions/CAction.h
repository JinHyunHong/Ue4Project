#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Components/CActionComponent.h"
#include "CAction.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentData
{
	GENERATED_BODY()

public:
	// 얘들은 따로 타입을 안써도 됨
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	// Equip정보에서는 Equip 사운드, Action에서는 HitSound
	UPROPERTY(EditAnywhere)
		class USoundCue* Sound;

	UPROPERTY(EditAnywhere)
		float Volume = 1.0f;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	// 시야를 바꿀 것인지
	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;

};

// 액션 같은 경우 몽타주 데이터가 더 필요함
USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	// HitStop : 타격되었을 때 일시정지(경직)을 구현할 변수(타격감)
	UPROPERTY(EditAnywhere)
		float HitStop;

	// 타격시 플레이 시킬 효과
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	// 효과 위치, 크기를 추가적으로 보정하기 위한 값
	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	// 나이아가라 이펙트도 사용할 수 있도록 함
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere)
		FTransform NiagaraTransform;

	// 타격시 카메라의 흔들림을 구현할 클래스
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UMatineeCameraShake> ShakeClass;
};

USTRUCT(BlueprintType)
struct FDoSkillData : public FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		FName SkillName;
	
	UPROPERTY(EditAnywhere)
		class UTexture2D* SumNail;

	UPROPERTY(EditAnywhere)
		float MaxCoolTime = 5.0f;

	UPROPERTY(EditAnywhere)
		float OffsetRatio = 1.0f; // 공격 속도 Offset

	UPROPERTY(EditAnywhere)
		float RatioDuration = 10.0f; // 공격 속도 Offset 지속 시간
};

USTRUCT(BlueprintType)
struct FActionData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		EActionType ActionType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACEquipment> EquipmentClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData EquipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FEquipmentData UnequipmentData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<class ACDoAction> DoActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoSkillData> DoSkillDatas;
};


UCLASS()
class UE4PROJECT_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	friend class UCActionComponent;

public:
	UCAction();

	void DestoryAction();

public:
	FORCEINLINE FName GetActionName() { return ActionName; }
	FORCEINLINE EActionType GetActionType() { return Type; }
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }

private:
	FName ActionName;
	EActionType Type;
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACDoAction* DoAction;
};