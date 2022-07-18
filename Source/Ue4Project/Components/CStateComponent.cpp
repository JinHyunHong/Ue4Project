#include "CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{
}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChangeType(EStateType::Roll);
}

void UCStateComponent::SetBackstepMode()
{
	ChangeType(EStateType::Backstep);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetMantle1MMode()
{
	ChangeType(EStateType::Mantle1M);
}

void UCStateComponent::SetMantle15MMode()
{
	ChangeType(EStateType::Mantle15M);
}

void UCStateComponent::SetMantle2MMode()
{
	ChangeType(EStateType::Mantle2M);
}

void UCStateComponent::ChangeType(EStateType InType)
{
	// 같은 타입의 경우 여러번 바뀌지 않도록
	CheckTrue(Type == InType)

	EStateType type = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(type, InType);
}