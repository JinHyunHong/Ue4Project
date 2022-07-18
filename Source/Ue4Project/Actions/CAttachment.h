#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class AActor*, InAttackCauser, class ACharacter*, InOtherCharacter);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentCollision);

UCLASS()
class UE4PROJECT_API ACAttachment : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Scene;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly)
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<class UShapeComponent*> ActionComponents; // 일반 공격의 Collision

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<class UShapeComponent*> SkillComponents; // 스킬의 Collision

protected:
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSoketName);

	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class UShapeComponent* InComponent, FName InSoketName); // 각 Collision을 붙일 수 있음(Collision 여러개 일때 유용)

	UFUNCTION(BlueprintCallable)
		void AttachToMesh(class USkeletalMeshComponent* InComponent, FName InSoketName); // 각 Mesh를 붙일 수 있음(무기가 2개 일때 유용)

	
public:	
	ACAttachment();

	void OnActionCollision();
	void OnSkillCollision(const int32& InIndex); // 특정 Index의 Collision을 켜줌

	// 노티파이때 충돌체가 켜지고, 꺼진다.
	void OnCollision();
	void OffCollision();


private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// 충돌처리 공격 델리게이트
	UPROPERTY(BlueprintAssignable)
		FAttachmentBeginOverlap OnAttachmentBeginOverlap;

	UPROPERTY(BlueprintAssignable)
		FAttachmentEndOverlap OnAttachmentEndOverlap;

	UPROPERTY(BlueprintAssignable)
		FAttachmentCollision OnAttachmentCollision;

	UPROPERTY(BlueprintAssignable)
		FAttachmentCollision OffAttachmentCollision;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void OnEquip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnUnequip();

	UFUNCTION(BlueprintImplementableEvent)
		void OnAddActionCollision();

	UFUNCTION(BlueprintImplementableEvent)
		void OnAddSkillCollision();

protected:
	UPROPERTY(BlueprintReadOnly)
		class ACharacter* OwnerCharacter;

private:
	TArray<class UShapeComponent*> ShapeComponents; // 전체 Collision
};
