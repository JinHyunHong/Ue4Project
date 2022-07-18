#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Characters/CEquipCharacter.h"
#include "CItem.generated.h"

UCLASS()
class UE4PROJECT_API ACItem : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "ItemList")
		UDataTable* DataTable;

	UPROPERTY(EditAnywhere, Category = "Item")
		ECharacter CharacterType;

	UPROPERTY(EditAnywhere, Category = "Item")
		EEquipmentType EquipType;

	UPROPERTY(EditAnywhere, Category = "Item")
		FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<class UCUserWidget_PickMessage> PickMessageClass;

	UPROPERTY(EditAnywhere, Category = "Item")
		bool bRandom; // ∑£¥˝ æ∆¿Ã≈€

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* Sphere;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* Widget;

public:	
	ACItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UFUNCTION()
		void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	void SetItem();

private:
	void FindItemData();

public:
	FORCEINLINE const ECharacter& GetCharacterType() { return CharacterType; }
	FORCEINLINE const EEquipmentType& GetEquipmentType() { return EquipType; }
	FORCEINLINE const FString& GetItemName() { return ItemName; }
	FORCEINLINE const FItem& GetItemData() { return ItemData; }

public:
	FORCEINLINE void SetRandomItem(bool InRandom) { bRandom = InRandom; }

private:
	FItem ItemData;
};
