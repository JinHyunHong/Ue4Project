#include "CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Scene, "Scene");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Scene);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Scene);

	// ���� ������ �ʵ��� ���� �� �ø�
	Spline->SetRelativeLocation(FVector(0, 0, 30));
	
	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	// ��ġ �ɶ� �̸� �Ҵ��ϱ� ����
	Text->Text = FText::FromString(GetActorLabel());

	// ���ö����� ���� �������� ���θ� �����մϴ�. ���� ��ġ�� ������ ����Ʈ�� �Է� Ű ���� 1.0�� �˴ϴ�.
	Spline->SetClosedLoop(bLoop);
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
	Text->SetVisibility(false);
}