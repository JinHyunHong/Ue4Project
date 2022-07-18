#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CAttachment.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "GameFramework/Character.h"

void UCActionComponent::SetData(const EActionType& InType, class UCAction** OutData)
{
	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);

	FName actionName = FName(CHelpers::GetEnumToString<EActionType>(InType, "EActionType"));
	FActionData* actionData = DataTable->FindRow<FActionData>(actionName, FString(""));
	CheckNull(actionData);

	FTransform transform;

	ACAttachment* attachment = NULL;

	if (!!actionData->AttachmentClass)
	{
		attachment = owner->GetWorld()->SpawnActorDeferred<ACAttachment>(actionData->AttachmentClass, transform, owner);
		attachment->SetActorLabel(GetLableName(owner, (actionName.ToString() + "_" + "Attachment")));
		UGameplayStatics::FinishSpawningActor(attachment, transform);
	}

	ACEquipment* equipment = NULL;

	if (!!actionData->EquipmentClass)
	{
		equipment = owner->GetWorld()->SpawnActorDeferred<ACEquipment>(actionData->EquipmentClass, transform, owner);
		equipment->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		equipment->SetActorLabel(GetLableName(owner, (actionName.ToString() + "_" + "Equipment")));
		equipment->SetEquipData(actionData->EquipmentData);
		equipment->SetUnequipData(actionData->UnequipmentData);

		UGameplayStatics::FinishSpawningActor(equipment, transform);

		if (!!attachment)
		{
			equipment->OnEquipmentDelegate.AddDynamic(attachment, &ACAttachment::OnEquip);
			equipment->OnUnequipmentDelegate.AddDynamic(attachment, &ACAttachment::OnUnequip);
			equipment->Begin_Unequip(); // 기본적으로 처음 Unequip()
		}
	}

	ACDoAction* doAction = NULL;

	if (!!actionData->DoActionClass)
	{
		doAction = owner->GetWorld()->SpawnActorDeferred<ACDoAction>(actionData->DoActionClass, transform, owner);
		doAction->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		doAction->SetActorLabel(GetLableName(owner, (actionName.ToString() + "_" + "DoAction")));
		doAction->SetDatas(actionData->DoActionDatas);
		doAction->SetSkillDatas(actionData->DoSkillDatas);
		UGameplayStatics::FinishSpawningActor(doAction, transform);

		if (!!equipment)
		{
			doAction->SetEquipped(equipment->GetEquipped());
		}

		if (!!attachment)
		{
			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &ACDoAction::OnAttachmentEndOverlap);

			attachment->OnAttachmentCollision.AddDynamic(doAction, &ACDoAction::OnAttahmentCollision);
			attachment->OffAttachmentCollision.AddDynamic(doAction, &ACDoAction::OffAttahmentCollision);
		}
	}

	(*OutData) = NewObject<UCAction>();
	(*OutData)->ActionName = actionName;
	(*OutData)->Type = actionData->ActionType;
	(*OutData)->Attachment = attachment;
	(*OutData)->Equipment = equipment;
	(*OutData)->DoAction = doAction;
}

void UCActionComponent::SetUnarmedMode()
{
	SetMode(EActionType::Unarmed);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetDoubleAxeMode()
{
	SetMode(EActionType::DoubleAxe);
}

void UCActionComponent::SetEnemyMode()
{
	SetMode(EActionType::Enemy);
}

bool UCActionComponent::IsUnarmedMode()
{
	return Type == EActionType::Unarmed;
}

bool UCActionComponent::IsOneHandMode()
{
	return Type == EActionType::OneHand;
}

bool UCActionComponent::IsDoubleAxeMode()
{
	return Type == EActionType::DoubleAxe;
}

bool UCActionComponent::IsEnemyMode()
{
	return Type == EActionType::Enemy;
}


UCActionComponent::UCActionComponent()
{
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < (int32)EActionType::Max; i++)
		SetData((EActionType)i, &Datas[i]);
}

void UCActionComponent::OffAllCollision()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (!!Datas[i])
		{
			ACAttachment* attachment = Datas[i]->GetAttachment();
			CheckNull(attachment);

			attachment->OffCollision();
		}
	}
}

void UCActionComponent::DestroyAllActions()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (Datas[i] == NULL) continue;
		if (Datas[i]->IsValidLowLevel() == false) continue;

		Datas[i]->DestoryAction();
	}
}


void UCActionComponent::SetMode(EActionType InType)
{
	CheckNull(Datas[(int32)Type]);

	if (InType == Type || InType == EActionType::Unarmed)
	{
		CLog::Print("Unquip");
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);
		equipment->Unequip();

		ChangeType(InType);

		return;
	}

	else if (IsUnarmedMode() == false)
	{
		CLog::Print("Unequip");
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);

		// 기존 무기를 해제시킴
		equipment->Unequip();
	}

	// 서로 무기를 교체하는 과정은 몽타주가 Equip 몽타주만 실행되므로, Unequip 노티파이 내용을 실행 
	if (IsUnarmedMode() == false && !!Datas[(int32)InType])
	{
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull(equipment);

		equipment->Begin_Unequip();
		equipment->End_Unequip();
	}

	if (!!Datas[(int32)InType])
	{
		CLog::Print("Equip");
		ACEquipment* equipment = Datas[(int32)InType]->GetEquipment();
		CheckNull(equipment);

		equipment->Equip();
	}

	ChangeType(InType);
}

void UCActionComponent::ChangeType(EActionType InType)
{
	PrevType = Type;
	Type = InType;

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(PrevType, Type);
}

FString UCActionComponent::GetLableName(ACharacter* InOwnerCharacter, FString InName)
{
	FString str;
	str.Append(InOwnerCharacter->GetActorLabel());
	str.Append("_");
	str.Append(InName);
	str.Append("_");
	str.Append(GetName());

	return str;
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnarmedMode());
	CheckFalse(Datas[(int32)Type]->IsValidLowLevel());

	ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	CheckFalse(action->IsValidLowLevel());

	if (!!action)
		action->DoAction();
}

void UCActionComponent::DoAction_Random()
{
	CheckTrue(IsUnarmedMode());
	CheckFalse(Datas[(int32)Type]->IsValidLowLevel());

	ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	CheckFalse(action->IsValidLowLevel());

	CLog::Print(action->GetActorLabel());

	if (!!action)
	{
		// 랜덤한 공격 Index 생성
		action->DoAction_RandomIndex();
		action->DoAction(action->GetIndex());
	}
}

void UCActionComponent::DoSkill(const FString& InSkillName)
{
	CheckTrue(IsUnarmedMode());
	CheckFalse(Datas[(int32)Type]->IsValidLowLevel());

	ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	CheckFalse(action->IsValidLowLevel());

	if (!!action)
		action->DoSkill(InSkillName);
}

void UCActionComponent::DoSkill(const int32& InIndex)
{
	CheckTrue(IsUnarmedMode());
	CheckFalse(Datas[(int32)Type]->IsValidLowLevel());

	ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	CheckFalse(action->IsValidLowLevel());

	if (!!action)
		action->DoSkill(InIndex);
}

void UCActionComponent::DoSkill_Random()
{
	CheckTrue(IsUnarmedMode());
	CheckFalse(Datas[(int32)Type]->IsValidLowLevel());

	ACDoAction* action = Datas[(int32)Type]->GetDoAction();
	CheckFalse(action->IsValidLowLevel());

	if (!!action)
	{
		// 랜덤한 공격 Index 생성
		action->DoSkill_RandomIndex();
		action->DoSkill(action->GetIndex());
	}
}