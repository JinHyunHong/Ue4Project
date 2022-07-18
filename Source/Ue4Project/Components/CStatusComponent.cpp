#include "CStatusComponent.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

UCStatusComponent::UCStatusComponent()
{
}

void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}

void UCStatusComponent::SetMove()
{
	bCanMove = true;
}

void UCStatusComponent::SetStop()
{
	bCanMove = false;
}

void UCStatusComponent::AddHealth(float InAmount)
{
	Health += InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SubHealth(float InAmount)
{
	Health -= InAmount;

	Health = FMath::Clamp(Health, 0.0f, MaxHealth);
}

void UCStatusComponent::SetSpeed(ECharacterSpeed InType)
{
	UCharacterMovementComponent* movement = CHelpers::GetComponent<UCharacterMovementComponent>(GetOwner());

	movement->MaxWalkSpeed = Speed[(int32)InType];
}