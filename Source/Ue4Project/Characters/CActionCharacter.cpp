#include "Characters/CActionCharacter.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

ACActionCharacter::ACActionCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateActorComponent<UCActionComponent>(this, &Action, "Action");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCMontagesComponent>(this, &Montages, "Montages");
	
	CHelpers::CreateComponent<UAudioComponent>(this, &Audio, "Audio", GetMesh());

	// 자동 재생 방지
	Audio->bAutoActivate = false;
}

void ACActionCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACActionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACActionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}