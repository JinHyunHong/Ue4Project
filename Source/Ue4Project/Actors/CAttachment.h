#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

UCLASS()
class UE4PROJECT_API ACAttachment : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(VisibleDefaultsOnly)
		class USkeletalMeshComponent* Mesh;

protected:
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSoketName);

	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class UShapeComponent* InComponent, FName InSoketName);
	
public:	
	ACAttachment();

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

private:
	TArray<class UShapeComponent*> ShapeComponents;
};
