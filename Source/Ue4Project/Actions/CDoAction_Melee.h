#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_Melee.generated.h"

UCLASS()
class UE4PROJECT_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

	virtual void OnAttahmentCollision() override;
	virtual void OffAttahmentCollision() override;

private:
	UFUNCTION()
		void RestoreDilation();

private:
	bool bExist; // ���� �޺��� �����ϴ���
	bool bEnable; // ���� �޺��� ���Ǿ�����
	bool bLast; // �޺��� ����������

	int32 Index; // ���� ���° �޺�������

	TArray<ACharacter*> HittedCharacters;
	
};
