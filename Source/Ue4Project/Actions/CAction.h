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
	// ����� ���� Ÿ���� �Ƚᵵ ��
	UPROPERTY(EditAnywhere)
		class UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere)
		float PlayRatio = 1.0f;

	UPROPERTY(EditAnywhere)
		FName StartSection;

	UPROPERTY(EditAnywhere)
		bool bCanMove = true;

	// �þ߸� �ٲ� ������
	UPROPERTY(EditAnywhere)
		bool bPawnControl = true;

};

// �׼� ���� ��� ��Ÿ�� �����Ͱ� �� �ʿ���
USTRUCT(BlueprintType)
struct FDoActionData : public FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float Power = 5.0f;

	// HitStop : Ÿ�ݵǾ��� �� �Ͻ�����(����)�� ������ ����(Ÿ�ݰ�)
	UPROPERTY(EditAnywhere)
		float HitStop;

	// Ÿ�ݽ� �÷��� ��ų ȿ��
	UPROPERTY(EditAnywhere)
		class UParticleSystem* Effect;

	// ȿ�� ��ġ, ũ�⸦ �߰������� �����ϱ� ���� ��
	UPROPERTY(EditAnywhere)
		FTransform EffectTransform;

	// Ÿ�ݽ� ī�޶��� ��鸲�� ������ Ŭ����
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UMatineeCameraShake> ShakeClass;
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
		TSubclassOf<class ACDoAction> DoActionClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<FDoActionData> DoActionDatas;
};


UCLASS()
class UE4PROJECT_API UCAction : public UObject
{
	GENERATED_BODY()

public:
	friend class UCActionComponent;

public:
	FORCEINLINE FName GetItemName() { return ItemName; }
	FORCEINLINE EActionType GetActionType() { return Type; }
	FORCEINLINE class ACEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class ACDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }

private:
	FName ItemName;
	EActionType Type;
	class ACEquipment* Equipment;
	class ACAttachment* Attachment;
	class ACDoAction* DoAction;
};