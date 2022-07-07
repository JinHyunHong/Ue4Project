#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SkillSlot.generated.h"

UCLASS()
class UE4PROJECT_API UCUserWidget_SkillSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Slot")
		int32 Index = 0;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* SkillImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* SkillBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* IndexText;


protected:
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;

public:
	void Update(const FString& InSkillName, class UTexture2D* InSkillImage, const float& InCoolTime);
	void Clear();


public:
	UFUNCTION(BlueprintCallable)
		void RestartCoolTime();

public:
	FORCEINLINE bool GetEnable() { return bEnable; }
	FORCEINLINE float& GetMaxCoolTime() { return MaxCoolTime; }

private:
	UFUNCTION()
		void IncreaseCoolTime();

public:
	FORCEINLINE const FString& GetSkillName() { return SkillName; }

private:
	FString SkillName;
	bool bEnable;
	float CoolTime;
	float MaxCoolTime;
};
