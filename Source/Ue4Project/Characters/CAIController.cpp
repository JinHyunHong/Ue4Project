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
	PrimaryActorTick.bCanEverTick = true; // 틱 동작시켜줌
	
	// AI 컨트롤러에는 이미 BlackBoard라는 변수가 정의되어 있다.
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	CHelpers::CreateActorComponent<UCBehaviorComponent>(this, &Behavior, "Behavior");

	// CreateDefaultSubobject : 객체를 생성자에서 동적으로 할당하기 위해 사용
	// NewObject : BeginPlay 이후의 게임모드에서 동적할당 할때 사용
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600; // 반경
	Sight->LoseSightRadius = 800; // 벗어났을때 잃게 되는 범위
	Sight->PeripheralVisionAngleDegrees = 90; // 시야각
	Sight->SetMaxAge(2); // 잃었을떄 얼마만큼 유지시킬 건지
	
	// DetectionByAffiliation : 감지를 할 관계를 설정
	Sight->DetectionByAffiliation.bDetectEnemies = true; // 적만 감지
	Sight->DetectionByAffiliation.bDetectNeutrals = false; // 중립은 감지 안함
	Sight->DetectionByAffiliation.bDetectFriendlies = false; // 아군은 감지 안함

	// 나중에 TeamID로 적인지 아군인지 판단

	Perception->ConfigureSense(*Sight);
	// SetDominantSense() : 만약 여러개의 감지를 넣는 다면, 어느 감지가 우선되는지
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

	// 빙의되었을때

	// InPawn은 빙의된 애여서 저장함
	OwnerEnemy = Cast<ACEnemy_AI>(InPawn);

	// SetGenericTeamId() : TeamID가 같으면 아군, 다르면 적군, 255이면 중립으로 간주하고 감지를 수행함
	SetGenericTeamId(OwnerEnemy->GetTeamID());

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);

	// UseBlackboard() : BlackboardAsset을 BlackboardComponent에 등록해서 사용하겠단 의미
	// 비헤이비어트리안에 블랙보드를 등록하기 때문에
	// 어차피 비헤이비어 트리 안에 블랙보드가 존재한다.
	// AIController에 있는 Blackboard 변수에 할당함
	UseBlackboard(OwnerEnemy->GetBehaviorTree()->BlackboardAsset, Blackboard);
	
	// 우리가 정의한 UCBehaviorComponent에 Blackboard를 세팅해서 운용하도록 해줌
	Behavior->SetBlackboard(Blackboard);

	// BehaviorTree 실행시킴
	RunBehaviorTree(OwnerEnemy->GetBehaviorTree());
}


void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

	// 빙의 해제되었을때

	// 감지된거 다 날려줌(이벤트 클리어)
	Perception->OnPerceptionUpdated.Clear();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 디버깅 모드 일떄만 실행함
	CheckFalse(bDrawDebug);

	// 공격 범위나 범위를 그려줌
	FVector center = OwnerEnemy->GetActorLocation();
	center.Z -= AdjustCircleHeight; // 높이를 조정(정확히 일치하면 잘안보이는 경우가 있어서)

	// Segments : 정밀도
	// YAxis : FVector::RightVector, ZAxis : FVector::ForwardVector -> 원을 그릴 회전 방향
	// -> 즉, ZAxis 부터 YAxis까지 방향으로 그림(언리얼은 항상 시계방향으로 그려서)
	// 전방부터 오른쪽으로 그려지게 됨
	// center부터 감지 시야까지 그려줌
	DrawDebugCircle(GetWorld(), center, Sight->SightRadius, 300, FColor::Green, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);

	// center 부터 BehaviorRange(공격범위) 까지 그려줌
	DrawDebugCircle(GetWorld(), center, BehaviorRange, 300, FColor::Red, false, -1, 0, 0, FVector::RightVector, FVector::ForwardVector);
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdateActors)
{
	TArray<AActor*> actors;
	// Perception에 감지된 액터들을 받음(SenseToUse가 none이면 현재 어떤 식으로든 인식되는 모든 액터가 가져옴)
	Perception->GetCurrentlyPerceivedActors(NULL, actors);

	ACPlayer* player = NULL;
	for (AActor* actor : actors)
	{
		player = Cast<ACPlayer>(actor);

		// 플레이어가 있다면 감지되었다는 뜻
		if (!!player)
			break;
	}
	
	// blackboard에 Object로 Player 세팅해줌
	// 감지가 되었다면 player가 들어가고 안되었다면 NULL이 들어갈 것이다.
	Blackboard->SetValueAsObject("Player", player);
}
