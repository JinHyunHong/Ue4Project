#include "Components/CIKComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCIKComponent::UCIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCIKComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// ������ ������
	float leftDistance; // �޹߰� ���� ���� ���� ����
	FRotator leftRotation;
	Trace(LeftSocket, leftDistance, leftRotation, Data.LeftFootDistance.X);

	float rightDistance; // �����߰� ���� ���� ���� ����
	FRotator rightRotation;
	Trace(RightSocket, rightDistance, rightRotation, Data.RightFootDistance.X);

	// �޹߰� ������ �� ���̰� ���� ���� ���� ����
	float offset = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	// right�� left�� ��Ī�Ǽ� -���̴�.
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - offset), DeltaTime, InterpSpeed);

	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, leftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, rightRotation, DeltaTime, InterpSpeed);
}




void UCIKComponent::Trace(FName InSocket, float& OutDistance, FRotator& OutRotation, float& OutFootDistance)
{
	OutDistance = 0.0f;

	FVector location = OwnerCharacter->GetMesh()->GetSocketLocation(InSocket);

	FVector start = FVector(location.X, location.Y, OwnerCharacter->GetActorLocation().Z);

	float traceZ = start.Z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;

	FVector end = FVector(location.X, location.Y, traceZ);

	TArray<AActor*> ignoreActors;
	ignoreActors.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignoreActors, DrawDebugType, hitResult, true, FLinearColor::Green, FLinearColor::Red);

	CheckFalse(hitResult.IsValidBlockingHit());

	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = OffsetDistance + length - TraceDistance;

	// hit�� �鿡 ���� ����
	FVector normal = hitResult.ImpactNormal;

	// ��ũ ź��Ʈ�� ȸ���� ����
	// ��ũ ź��Ʈ�� ���� �ش� �������� ȸ���ϴ� ������ ���� �� ����
	// ������ Y(����)���� Z(��)�� �������� ��������(Roll)ó�� ������
	// �� ź��Ʈ���� Z(��)���� Y(����)���� ���� ���� ��(roll�� ��)
	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);

	OutFootDistance = location.Z - hitResult.ImpactPoint.Z;
}



