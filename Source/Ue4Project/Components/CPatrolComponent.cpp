#include "CPatrolComponent.h"
#include "Global.h"
#include "Actors/CPatrolPath.h"
#include "Components/SplineComponent.h"

UCPatrolComponent::UCPatrolComponent()
{
}


void UCPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UCPatrolComponent::GetMoveTo(FVector& OutLocation, float& OutAcceptanceRadius)
{
	CheckFalseResult(bActive, false);

	OutLocation = FVector::ZeroVector;
	OutAcceptanceRadius = AcceptanceRadius;
	CheckNullResult(Path, false);

	// GetLocationAtSplinePoint() : 해당 포인트의 위치를 반환
	// 1. 포인트 인덱스, 2 : 어떤 좌표 방식으로 위치를 받을지(우리는 월드)
	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);

	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckFalse(bActive);
	CheckNull(Path);

	// 포인트가 몇개인지 가져옴
	int32 count = GetNumberOfSplinePoints();
	
	// 역으로 가는 것 계산
	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;

			return;
		}

		// 닫힌 루프라면
		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;

			return;
		}

		// 다 도달 했다면
		Index = 1;
		bReverse = false;
		
		return;
	}

	// 정방향이라면
	if (Index < count - 1)
	{
		Index++;

		return;
	}

	if (Path->GetSpline()->IsClosedLoop())
	{
		Index = 0;

		return;
	}

	// 0, 1, 2, 3, 4 갔다가 다시 3으로 돌아와야 하니까
	Index = count - 2;
	// 다시 뒤집어줌
	bReverse = true;
}

void UCPatrolComponent::RandomIndex()
{
	CheckNull(Path);

	Index = UKismetMathLibrary::RandomIntegerInRange(0, GetNumberOfSplinePoints() - 1);
}

int32 UCPatrolComponent::GetNumberOfSplinePoints()
{
	CheckNullResult(Path, 0);

	return Path->GetSpline()->GetNumberOfSplinePoints();
}