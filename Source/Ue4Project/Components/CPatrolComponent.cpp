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

	// GetLocationAtSplinePoint() : �ش� ����Ʈ�� ��ġ�� ��ȯ
	// 1. ����Ʈ �ε���, 2 : � ��ǥ ������� ��ġ�� ������(�츮�� ����)
	OutLocation = Path->GetSpline()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);

	return true;
}

void UCPatrolComponent::UpdateNextIndex()
{
	CheckFalse(bActive);
	CheckNull(Path);

	// ����Ʈ�� ����� ������
	int32 count = GetNumberOfSplinePoints();
	
	// ������ ���� �� ���
	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;

			return;
		}

		// ���� �������
		if (Path->GetSpline()->IsClosedLoop())
		{
			Index = count - 1;

			return;
		}

		// �� ���� �ߴٸ�
		Index = 1;
		bReverse = false;
		
		return;
	}

	// �������̶��
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

	// 0, 1, 2, 3, 4 ���ٰ� �ٽ� 3���� ���ƿ;� �ϴϱ�
	Index = count - 2;
	// �ٽ� ��������
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