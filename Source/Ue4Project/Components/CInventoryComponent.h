#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/CEquipCharacter.h"
#include "CInventoryComponent.generated.h"


// 현재 위젯 Visibillity 상황
UENUM(BlueprintType)
enum class EViewListType : uint8
{
	AllHidden, Equipment, Inventory, AllView, Max,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE4PROJECT_API UCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	// Equipment, Iventory 위젯 세팅가능하도록 설정
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		TSubclassOf<class UCUserWidget_EquipInventory> EquipIventoryClass;

	// Equipment Widget에서 캐릭터를 볼 수 있도록
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
		TSubclassOf<class ACEquipCharacter> EquipStateClass;

public:	
	UCInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
		void ViewAllHidden();
	
	UFUNCTION(BlueprintCallable)
		void ViewEquipment();
	
	UFUNCTION(BlueprintCallable)
		void ViewInventory();
	
	UFUNCTION(BlueprintCallable)
		void ViewAll();

public:
	UFUNCTION(BlueprintCallable)
		void AddItem(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData, bool bPickUp = false);
	
	UFUNCTION(BlueprintCallable)
		void AddEquip(const ECharacter& InCharacterType, const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData);

private:
	void OnEquipState(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData); // EquipState에 장비 장착
	void OnUnequipState(const EEquipmentType& InEquipType); // EquipState에 장비 해제

private:
	void SetViewList(const EViewListType& InType);


public:
	FORCEINLINE class UCUserWidget_Equipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCUserWidget_Inventory* GetInventory() { return Inventory; }

private:
	// 한개의 위젯으로 통합되어 있음
	class UCUserWidget_EquipInventory* EquipIventory;
	class UCUserWidget_Equipment* Equipment;
	class UCUserWidget_Inventory* Inventory;

private:
	class ACEquipCharacter* EquipState;
};
