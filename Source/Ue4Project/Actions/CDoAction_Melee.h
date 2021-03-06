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
	virtual void DoAction(const int32& InIndex) override;
	virtual void Begin_DoAction() override;
	virtual void End_Action() override;

	virtual void DoSkill(const FString& InSkillName) override;
	virtual void DoSkill(const int32& InIndex) override;

public:
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOtherCharacter) override;

	virtual void OnAttahmentCollision() override;
	virtual void OffAttahmentCollision() override;

	virtual void DoAction_RandomIndex() override;
	virtual void DoSkill_RandomIndex() override;

private:
	UFUNCTION()
		void RestoreDilation();

private:
	bool bExist; // 다음 콤보가 존재하는지
	bool bEnable; // 다음 콤보가 허용되었는지
	bool bLast; // 콤보의 마지막인지

	TArray<ACharacter*> HittedCharacters;
	class UNiagaraComponent* Niagara;
};
