#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CParkourComponent.generated.h"

UENUM(BlueprintType)
enum class EMantleType : uint8
{
	None, Mantle1M, Mantle15M, Mantle2M, Max,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mantling")
		float TraceDistance = 150.0f; // 최대 추적 높이

public:	
	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	EMantleType Mantling();
	void BeginMantle();
	void EndMantle();

private:
	FVector MantleHeight; // 맨틀의 최종적 위치
	ACharacter* OwnerCharacter;
};
