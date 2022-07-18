#include "Components/CParkourComponent.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCParkourComponent::UCParkourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


EMantleType UCParkourComponent::Mantling()
{
	FVector start = OwnerCharacter->GetActorLocation();
	start.Z += TraceDistance;
	start += OwnerCharacter->GetActorForwardVector() * 50.0f;
	FVector end = start;
	end.Z -= 200.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<AActor*> ignoreActors; 
	ignoreActors.Add(OwnerCharacter);

	FHitResult hitResult;

	CheckFalseResult(UKismetSystemLibrary::SphereTraceSingleForObjects(OwnerCharacter->GetWorld(), start, end, 10.0f, objectTypes, false, ignoreActors, DrawDebugType, hitResult, true), EMantleType::None);

	MantleHeight = hitResult.Location;

	FVector location = OwnerCharacter->GetActorLocation() - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	if (MantleHeight.Z - location.Z > 200.0f)
		return EMantleType::Mantle2M;

	if (MantleHeight.Z - location.Z > 150.0f)
		return EMantleType::Mantle15M;

	if (MantleHeight.Z > 100.0f)
		return EMantleType::Mantle1M;

	return EMantleType::None;
}

void UCParkourComponent::BeginMantle()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void UCParkourComponent::EndMantle()
{
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}


