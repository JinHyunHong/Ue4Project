#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CActionCharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

//=============================================
//== BP implementation Function(Body Color Change)
//== ChangeColor(), SetBodyMaterial(), RestoreColor()
//=============================================

UCLASS()
class UE4PROJECT_API ACEnemy : public ACActionCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Enemy")
		bool bBoss = false;

	UPROPERTY(EditAnywhere, Category = "Enemy")
		TSubclassOf<class UCUserWidget_Health> BossClass;

	UPROPERTY(EditAnywhere, Category = "Hitted")
		FLinearColor HittedColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Hitted")
		TSubclassOf<AActor> BloodClass;

	UPROPERTY(EditAnywhere, Category = "Hitted")
		float BloodDuration = 3.0f;

	// Hitted �Ǿ��� �� �и��� ����
	UPROPERTY(EditAnywhere, Category = "Hitted")
		float LaunchAmount = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Hitted")
		float AirLaunchAmount = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Angry")
		FLinearColor AngryColor = FLinearColor(0.5f, 0.0f, 0.0f, 1.0f); // ȭ�� ������ ����


protected:
	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* NameWidget;

	UPROPERTY(VisibleDefaultsOnly)
		class UWidgetComponent* HealthWidget;

public:
	ACEnemy();

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SetBodyMaterial(); // ���ʹ� ������ Material�� ������ ����

	// �� �����ϱ�(Ÿ�̸� �������̼�����)
	UFUNCTION(BlueprintImplementableEvent)
		void RestoreColor();

public:
	virtual void Begin_Dead() override;
	virtual void End_Dead() override;

private:
	UFUNCTION()
		void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	void Hitted();
	void Dead();
	
public:
	UFUNCTION()
		void FinishDead();

public:
	FORCEINLINE bool IsBoss() { return bBoss; }
	FORCEINLINE FLinearColor& GetAngryColor() { return AngryColor; }


private:
	class UMaterialInstanceDynamic* BodyMaterial;

private:
	class AController* DamageInstigator; // ������ ��Ʈ�ѷ�
	float DamageValue; // ������ ��
	class UUserWidget* BossWidget;
	TArray<AActor*> Bloods;
};
