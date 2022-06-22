#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Characters/CEquipCharacter.h"
#include "CEquipComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCEquipComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		UDataTable* DataTable;

public:	
	UCEquipComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		FItem Equip(const EEquipmentType& InEquipType, const FString& InItemName);
	
	UFUNCTION(BlueprintCallable)
		bool UnEquip(const EEquipmentType& InEquipType);

private:
	ACEquipCharacter* OwnerCharacter;
	ACAttachment* Weapon;
};
