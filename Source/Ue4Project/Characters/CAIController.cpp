#include "CAIController.h"
#include "Global.h"
#include "CEnemy_AI.h"
#include "CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CBehaviorComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true; // ƽ ���۽�����
	
	// AI ��Ʈ�ѷ����� �̹� BlackBoard��� ������ ���ǵǾ� �ִ�.
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");

	// CreateDefaultSubobject : ��ü�� �����ڿ��� �������� �Ҵ��ϱ� ���� ���
	// NewObject : BeginPlay ������ ���Ӹ�忡�� �����Ҵ� �Ҷ� ���
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600; // �ݰ�
	Sight->LoseSightRadius = 800; // ������� �Ұ� �Ǵ� ����
	Sight->PeripheralVisionAngleDegrees = 90; // �þ߰�
	Sight->SetMaxAge(2); // �Ҿ����� �󸶸�ŭ ������ų ����
	
	// DetectionByAffiliation : ������ �� ���踦 ����
	Sight->DetectionByAffiliation.bDetectEnemies = true; // ���� ����
	Sight->DetectionByAffiliation.bDetectNeutrals = false; // �߸��� ���� ����
	Sight->DetectionByAffiliation.bDetectFriendlies = false; // �Ʊ��� ���� ����

	// ���߿� TeamID�� ������ �Ʊ����� �Ǵ�

	Perception->ConfigureSense(*Sight);
	// SetDominantSense() : ���� �������� ������ �ִ� �ٸ�, ��� ������ �켱�Ǵ���
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

float ACAIController::GetSightRadius()
{
	return Sight->SightRadius;
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// ���ǵǾ�����

	// InPawn�� ���ǵ� �ֿ��� ������
	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);

	// SetGenericTeamId() : TeamID�� ������ �Ʊ�, �ٸ��� ����, 255�̸� �߸����� �����ϰ� ������ ������
	SetGenericTeamId(OwnerEnemy->GetTeamID());

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	// UseBlackboard() : BlackboardAsset�� BlackboardComponent�� ����ؼ� ����ϰڴ� �ǹ�
	// �����̺��Ʈ���ȿ� �����带 ����ϱ� ������
	// ������ �����̺�� Ʈ�� �ȿ� �����尡 �����Ѵ�.
	// AIController�� �ִ� Blackboard ������ �Ҵ���
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
	
	// �츮�� ������ UCBehaviorComponent�� Blackboard�� �����ؼ� ����ϵ��� ����
	Behavior->SetBlackboard(Blackboard);

	// BehaviorTree �����Ŵ
	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
}


void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	// ���� �����Ǿ�����

	// �����Ȱ� �� ������(�̺�Ʈ Ŭ����)
	Perception->OnPerceptionUpdated.Clear();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ����� ��� �ϋ��� ������
	CheckFalse(bDrawDebug);

	// ���� ������ ������ �׷���
	FVector center = OwnerEnemy->GetActorLocation();
	center.Z -= AdjustCircleHeight; // ���̸� ����(��Ȯ�� ��ġ�ϸ� �߾Ⱥ��̴� ��찡 �־)

	// Segments : ���е�
	// YAxis : FVector::RightVector, ZAxis : FVector::ForwardVector -> ���� �׸� ȸ�� ����
	// -> ��, ZAxis ���� YAxis���� �������� �׸�(�𸮾��� �׻� �ð�������� �׷���)
	// ������� ���������� �׷����� ��
	// center���� ���� �þ߱��� �׷���
	DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);

	// center ���� BehaviorRange(���ݹ���) ���� �׷���
	DrawDebugCircle(GetWorld(), center, BehaviorRange, 300, FColor::Red, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdateActors)
{
	TArray<AActor*> actors;
	// Perception�� ������ ���͵��� ����(SenseToUse�� none�̸� ���� � �����ε� �νĵǴ� ��� ���Ͱ� ������)
	Perception->GetCurrentlyPerceivedActors(NULL, actors);

	ACPlayer* player = NULL;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);

		// �÷��̾ �ִٸ� �����Ǿ��ٴ� ��
		if (!!player)
			break;
	}
	
	// blackboard�� Object�� Player ��������
	// ������ �Ǿ��ٸ� player�� ���� �ȵǾ��ٸ� NULL�� �� ���̴�.
	Blackboard->SetValueAsObject("Player", player);
}
