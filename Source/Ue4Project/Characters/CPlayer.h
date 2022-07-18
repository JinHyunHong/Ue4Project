#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CEquipCharacter.h"
#include "Components/CStateComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"


UCLASS()
class UE4PROJECT_API ACPlayer : public ACEquipCharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCUserWidget_SkillList> SkillListClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Footstep")
		class USoundCue* Footstep;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Footstep")
		float DeduceDistance; // �߰� �浹�� �ٴ� ������ �Ÿ� ���� �� 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AirRun")
		class UNiagaraSystem* AirDashEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AirRun")
		FVector RandomAir = FVector(150.0f, 150.0f, 30.0f); // ���߿��� �̵� �� ���� ��

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AirRun")
		float MoveDilation = 0.5f; // Air �̵��� DilationAmount

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AirRun")
		float TargetDistance = 300.0f; // Air Target���� �Ÿ� ����(�Ÿ� �ȿ� �ִ� Ÿ�ٸ� Air ó������) 

private:
	// TeamID�� ���̶� �ٸ��� ������ �ȴ�.
	UPROPERTY(EditDefaultsOnly)
		uint8 TeamID = 0;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class UCOptionComponent* Option;

	UPROPERTY(VisibleDefaultsOnly)
		class UCIKComponent* IK;

	UPROPERTY(VisibleDefaultsOnly)
		class UCTargetComponent* Target;

	UPROPERTY(VisibleDefaultsOnly)
		class UCParkourComponent* Parkour;


protected:
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class UCInventoryComponent* Inventory;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* TargetPoint; // ���ݽ� Ÿ���� ��ġ�� ����(Air�� ���)


public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FGenericTeamId GetGenericTeamId() const override;

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	void OnMoveFoward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);


private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);
	
public:
	UFUNCTION()
		void OnEquipChanged(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData);

	UFUNCTION()
		void OnUnequipChanged(const EEquipmentType& InEquipType, const FString& InItemName, const FItem& InItemData);

private:
	void OnRun();
	void OffRun();
	void OnAvoid();

private:
	bool OnMantling();
	void Mantle1M();
	void Mantle15M();
	void Mantle2M();

private:
	void OnViewEquipmnent();
	void OnViewInventory();
	void OnInteract();

	void OnDoAction();
	void OnAir();

private:
	void AirFalling(); // Air���� �÷��̾� ���ݽ� ��ġ ����

private:
	void OnTarget();
	void OnTargetLeft();
	void OnTargetRight();

	void OnDoSkill1();
	void OnDoSkill2();
	void OnDoSkill3();
	void OnDoSkill4();

public:
	void Begin_Roll();
	void Begin_Backstep();
	
	void End_Roll();
	void End_Backstep();

public:
	FORCEINLINE void SetInteractItem(AActor* InItem) { InteractItem = InItem; }
	FORCEINLINE const float& GetDeduceDistance() { return DeduceDistance; }

private:
	void Hitted();
	void Dead();

public:
	virtual void Begin_Dead() override;
	virtual void End_Dead() override;

public:
	void PlayFootstep();


private:
	class UCUserWidget_SkillList* SkillList;
	class AActor* InteractItem;
	class AController* DamageInstigator;
	class UNiagaraComponent* AirDash;
	float DamageValue;
};
