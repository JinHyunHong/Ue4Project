#include "CBTTaskNode_SetMaterialVector.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

UCBTTaskNode_SetMaterialVector::UCBTTaskNode_SetMaterialVector()
{
	NodeName = "SetMaterialVector";
}

EBTNodeResult::Type UCBTTaskNode_SetMaterialVector::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	ai->Execute_ChangeColor(ai, ai->GetAngryColor());

	return EBTNodeResult::Succeeded;
}


