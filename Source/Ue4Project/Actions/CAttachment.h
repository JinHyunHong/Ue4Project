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
		TArray<class UShapeComponent*> ActionComponents; // �Ϲ� ������ Collision

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
		TArray<class UShapeComponent*> SkillComponents; // ��ų�� Collision

protected:
	UFUNCTION(BlueprintCallable)
		void AttachTo(FName InSoketName);

	UFUNCTION(BlueprintCallable)
		void AttachToCollision(class UShapeComponent* InComponent, FName InSoketName); // �� Collision�� ���� �� ����(Collision ������ �϶� ����)

	UFUNCTION(BlueprintCallable)
		void AttachToMesh(class USkeletalMeshComponent* InComponent, FName InSoketName); // �� Mesh�� ���� �� ����(���Ⱑ 2�� �϶� ����)

	
public:	
	ACAttachment();

	void OnActionCollision();
	void OnSkillCollision(const int32& InIndex); // Ư�� Index�� Collision�� ����

	// ��Ƽ���̶� �浹ü�� ������, ������.
	void OnCollision();
	void OffCollision();


private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// �浹ó�� ���� ��������Ʈ
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
	TArray<class UShapeComponent*> ShapeComponents; // ��ü Collision
};
