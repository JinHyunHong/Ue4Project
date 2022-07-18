#include "CBTTaskNode_SetSightRadius.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Perception/AISenseConfig_Sight.h"

UCBTTaskNode_SetSightRadius::UCBTTaskNode_SetSightRadius()
{
	NodeName = "SetSightRadius";
}

EBTNodeResult::Type UCBTTaskNode_SetSightRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());

	UAISenseConfig_Sight* sight = controller->GetSight();
	sight->SightRadius = SightRadius; // 감지 반경
	sight->LoseSightRadius = LossSightRadius;

	return EBTNodeResult::Succeeded;
}


