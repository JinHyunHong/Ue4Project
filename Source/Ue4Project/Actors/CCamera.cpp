#include "Actors/CCamera.h"
#include "Global.h"

void ACCamera::BeginPlay()
{
	PrimaryActorTick.bCanEverTick = true;
	StartPos = GetActorLocation();
	StartHeightPos = StartPos + GetActorUpVector() * Height;
	EndPos = StartPos + GetActorForwardVector() * Width;
	EndHeightPos = EndPos + GetActorUpVector() * Height;

	Super::BeginPlay();
}

void ACCamera::Tick(float DeltaTime)
{
	CheckFalse(bEnable);
	CheckTrue(Time > 1.0f);
	Super::Tick(DeltaTime);
	
	FVector result;
	Time += (DeltaTime / Duration);
	CMath::GetBezierCurve(StartPos, StartHeightPos, EndHeightPos, EndPos, Time, result);
	SetActorLocation(result);
}
