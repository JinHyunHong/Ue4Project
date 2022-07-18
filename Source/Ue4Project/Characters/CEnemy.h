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

	// Hitted 되었을 때 밀리는 정도
	UPROPERTY(EditAnywhere, Category = "Hitted")
		float LaunchAmount = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Hitted")
		float AirLaunchAmount = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Angry")
		FLinearColor AngryColor = FLinearColor(0.5f, 0.0f, 0.0f, 1.0f); // 화난 상태의 색상


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
		void SetBodyMaterial(); // 몬스터는 제각각 Material을 가지고 있음

	// 색 복원하기(타이머 델리게이션으로)
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
	class AController* DamageInstigator; // 공격한 컨트롤러
	float DamageValue; // 데미지 양
	class UUserWidget* BossWidget;
	TArray<AActor*> Bloods;
};
