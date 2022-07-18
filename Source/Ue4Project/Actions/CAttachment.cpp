#include "Actions/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ShapeComponent.h"

ACAttachment::ACAttachment()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Scene);
}

void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	GetComponents<UShapeComponent>(ShapeComponents);

	for (UShapeComponent* component : ShapeComponents)
	{
		component->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
		component->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
	}
	
	// ±âº»°ø°Ý, ½ºÅ³ Collision ¼³Á¤
	OnAddActionCollision();
	OnAddSkillCollision();

	OffCollision();

	Super::BeginPlay();
}

void ACAttachment::AttachTo(FName InSoketName)
{
	// AttachToSocket
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSoketName);
}


void ACAttachment::AttachToCollision(UShapeComponent* InComponent, FName InSoketName)
{
	// AttachToCollision
	InComponent->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSoketName);
}

void ACAttachment::AttachToMesh(USkeletalMeshComponent* InComponent, FName InSoketName)
{
	// AttachToMesh
	InComponent->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSoketName);
}

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OtherActor->GetClass() == OwnerCharacter->GetClass());

	ACharacter* other = Cast<ACharacter>(OtherActor);
	CheckNull(other);

	if (OnAttachmentBeginOverlap.IsBound())
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, other);
}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OtherActor->GetClass() == OwnerCharacter->GetClass());

	ACharacter* other = Cast<ACharacter>(OtherActor);
	CheckNull(other);

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, this, other);
}

void ACAttachment::OnActionCollision()
{
	for (UShapeComponent* component : ActionComponents)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (OnAttachmentCollision.IsBound())
		OnAttachmentCollision.Broadcast();
}

void ACAttachment::OnSkillCollision(const int32& InIndex)
{
	CheckFalse(SkillComponents.Num() > InIndex);

	SkillComponents[InIndex]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (OnAttachmentCollision.IsBound())
		OnAttachmentCollision.Broadcast();
}

void ACAttachment::OnCollision()
{
	// ÄÑÁÜ
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	if (OnAttachmentCollision.IsBound())
		OnAttachmentCollision.Broadcast();
}

void ACAttachment::OffCollision()
{
	// ²¨ÁÜ
	for (UShapeComponent* component : ShapeComponents)
		component->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (OffAttachmentCollision.IsBound())
		OffAttachmentCollision.Broadcast();
}

