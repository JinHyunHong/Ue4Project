#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPatrolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCPatrolComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
		class ACPatrolPath* Path;

	UPROPERTY(EditAnywhere)
		int32 Index; // 현재 몇번을 타고 있는지

	UPROPERTY(EditAnywhere)
		bool bReverse; // 뒤집어 졌는지(역으로 진행할건지)

	UPROPERTY(EditAnywhere)
		float AcceptanceRadius = 50; // 도달반경(얼마만큼 근방으로 갔을때 도달로 간주할지)
	
public:
	// 있는지
	FORCEINLINE bool IsValid() { return Path != NULL; }

	// 어디로 이동할지(어느위치로 이동할지, 거리가 얼마나 되는지) 
	bool GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius);

	// 도달했으면 다음으로 넘겨줘라
	void UpdateNextIndex();

public:	
	UCPatrolComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void SetPath(class ACPatrolPath* InPath) { Path = InPath; }

	UFUNCTION(BlueprintCallable)
		void SetIndex(const int32& InIndex) { Index = InIndex; }

	UFUNCTION(BlueprintCallable)
		void SetReverse(bool InReverse) { bReverse = InReverse; }

	UFUNCTION(BlueprintCallable)
		void RandomIndex();

	UFUNCTION(BlueprintPure)
		int32 GetNumberOfSplinePoints();

	UFUNCTION(BlueprintPure)
		int32& GetIndex() { return Index; }

	UFUNCTION(BlueprintPure)
		bool GetReverse() { return bReverse; }

public:
	FORCEINLINE void Active() { bActive = true; }
	FORCEINLINE void Inactive() { bActive = false; }

private:
	bool bActive = false;
};
