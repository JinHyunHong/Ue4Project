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
	FORCEINLINE void SetEquipped(const bool* InEquipped) { bEquipped = InEquipped; }

public:
	ACDoAction();

public:
	// 액션을 수행(액션에 해당하는 몽타주 플레이)
	virtual void DoAction() {};
	// 몽타주에 의해 시작
	virtual void Begin_DoAction() {};
	// 몽타주에 의해 종료
	virtual void End_DoAction() {};

	// Aiming이 필요한 애들은 이것을 재정의
	virtual void OnAim() {}
	virtual void OffAim() {}

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

protected:
	const bool* bEquipped;

	TArray<FDoActionData> Datas;

};
