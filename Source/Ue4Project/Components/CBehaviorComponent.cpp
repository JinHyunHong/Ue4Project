#include "CBehaviorComponent.h"
#include "Global.h"
#include "Characters/CPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UCBehaviorComponent::IsWaitMode()
{
	return GetType() == EBehaviorType::Wait;
}

bool UCBehaviorComponent::IsApproachMode()
{
	return GetType() == EBehaviorType::Approach;
}

bool UCBehaviorComponent::IsActionMode()
{
	return GetType() == EBehaviorType::Action;
}

bool UCBehaviorComponent::IsPatrolMode()
{
	return GetType() == EBehaviorType::Patrol;
}

bool UCBehaviorComponent::IsHittedMode()
{
	return GetType() == EBehaviorType::Hitted;
}

bool UCBehaviorComponent::IsAvoidMode()
{
	return GetType() == EBehaviorType::Avoid;
}

bool UCBehaviorComponent::IsAngryMode()
{
	return GetType() == EBehaviorType::Angry;
}

UCBehaviorComponent::UCBehaviorComponent()
{

}

void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCBehaviorComponent::SetWaitMode()
{
	ChangeType(EBehaviorType::Wait);
}

void UCBehaviorComponent::SetApproachMode()
{
	ChangeType(EBehaviorType::Approach);
}

void UCBehaviorComponent::SetActionMode()
{
	ChangeType(EBehaviorType::Action);
}

void UCBehaviorComponent::SetPatrolMode()
{
	ChangeType(EBehaviorType::Patrol);
}

void UCBehaviorComponent::SetHittedMode()
{
	ChangeType(EBehaviorType::Hitted);
}

void UCBehaviorComponent::SetAvoidMode()
{
	ChangeType(EBehaviorType::Avoid);
}

void UCBehaviorComponent::SetAngryMode()
{
	ChangeType(EBehaviorType::Angry);
}

void UCBehaviorComponent::ChangeType(EBehaviorType InType)
{
	EBehaviorType type = GetType();
	// 블랙보드에 AI 상태 값 세팅
	// blackboard에서는 enum을 uint8로 받는다.
	Blackboard->SetValueAsEnum(BehaviorKey, (uint8)InType);

	if (OnBehaviorTypeChanged.IsBound())
		OnBehaviorTypeChanged.Broadcast(type, InType);
}

ACPlayer* UCBehaviorComponent::GetTargetPlayer()
{
	// 블랙보드에 캐릭터가 있었다면 그것을 리턴
	return Cast<ACPlayer>(Blackboard->GetValueAsObject(PlayerKey));
}

EBehaviorType UCBehaviorComponent::GetType()
{
	// 블랙보드에 세팅되어있는 키에 따른 enum 값(EBehaviorType)을 가져옴
	// uint8을 다시 EBehaviorType로 캐스팅함
	return (EBehaviorType)Blackboard->GetValueAsEnum(BehaviorKey);
}