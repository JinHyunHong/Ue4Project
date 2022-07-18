#include "CEquipment.h"
#include "Global.h"
#include "Characters/ICharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CStatusComponent.h"

ACEquipment::ACEquipment()
{	
}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);

	Super::BeginPlay();
}

void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (EquipData.AnimMontage != NULL)
		OwnerCharacter->PlayAnimMontage(EquipData.AnimMontage, EquipData.PlayRatio, EquipData.StartSection);
	else
		End_Equip();

	if (EquipData.bPawnControl == true)
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}
}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
		OnEquipmentDelegate.Broadcast();
}

void ACEquipment::End_Equip_Implementation()
{
	bEquipped = true;

	State->SetIdleMode();
}

void ACEquipment::Unequip_Implementation()
{
	if (UnequipData.AnimMontage != NULL)
		OwnerCharacter->PlayAnimMontage(UnequipData.AnimMontage, UnequipData.PlayRatio, UnequipData.StartSection);
	else
		End_Unequip();
	
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ACEquipment::Begin_Unequip_Implementation()
{
	if (OnUnequipmentDelegate.IsBound())
		OnUnequipmentDelegate.Broadcast();
}

void ACEquipment::End_Unequip_Implementation()
{
	bEquipped = false;
}

