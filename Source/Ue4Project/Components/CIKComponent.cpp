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


	// 간격을 구해줌
	float leftDistance; // 왼발과 지면 붙은 곳의 간격
	FRotator leftRotation;
	Trace(LeftSocket, leftDistance, leftRotation, Data.LeftFootDistance.X);

	float rightDistance; // 오른발과 지면 붙은 곳의 간격
	FRotator rightRotation;
	Trace(RightSocket, rightDistance, rightRotation, Data.RightFootDistance.X);

	// 왼발과 오른발 중 높이가 가장 낮은 값을 선택
	float offset = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, offset, DeltaTime, InterpSpeed);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - offset), DeltaTime, InterpSpeed);
	// right는 left에 대칭되서 -값이다.
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

	// hit된 면에 수직 벡터
	FVector normal = hitResult.ImpactNormal;

	// 아크 탄젠트로 회전값 구함
	// 아크 탄젠트를 쓰면 해당 방향으로 회전하는 방향을 구할 수 있음
	// 원래는 Y(우측)에서 Z(위)로 방향으로 옆구르기(Roll)처럼 되지만
	// 역 탄젠트여서 Z(위)에서 Y(우측)으로 가는 것이 됨(roll이 됨)
	float roll = UKismetMathLibrary::DegAtan2(normal.Y, normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(normal.X, normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);

	OutFootDistance = location.Z - hitResult.ImpactPoint.Z;
}



