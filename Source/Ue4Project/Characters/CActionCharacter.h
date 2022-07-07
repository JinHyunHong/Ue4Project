#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "CActionCharacter.generated.h"

UCLASS()
class UE4PROJECT_API ACActionCharacter : public ACharacter, public IICharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCActionComponent* Action;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStateComponent* State;

	UPROPERTY(VisibleDefaultsOnly)
		class UCStatusComponent* Status;

	UPROPERTY(VisibleDefaultsOnly)
		class UCMontagesComponent* Montages;

	UPROPERTY(VisibleDefaultsOnly)
		class UAudioComponent* Audio;

public:
	ACActionCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void ChangeColor_Implementation(FLinearColor InColor) override {};
	virtual void Begin_Dead() override {};
	virtual void End_Dead() override {};

public:
	void PlayNiagara();
	void StopNiagara();

protected:
	class UNiagaraComponent* Niagara;
};
