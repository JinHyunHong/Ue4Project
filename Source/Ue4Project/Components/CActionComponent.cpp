#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CAction.h"
#include "Actions/CAttachment.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "GameFramework/Character.h"

void UCActionComponent::SetAction(const FString& InItemName)
{
	DestroyAllActions();

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);

	EActionType prevType = EActionType::Unarmed;
	if (Data) prevType = Data->Type;

	FActionData* actionData = DataTable->FindRow<FActionData>(FName(*InItemName), FString(""));
	CheckNull(actionData);

	FTransform transform;
	
	ACAttachment* attachment = NULL;
	
	if (!!actionData->AttachmentClass)
	{
		attachment = owner->GetWorld()->SpawnActorDeferred<ACAttachment>(actionData->AttachmentClass, transform, owner);
		attachment->SetActorLabel(GetLableName(owner, "Attachment"));
		UGameplayStatics::FinishSpawningActor(attachment, transform);
	}
	
	ACEquipment* equipment = NULL;
	
	if (!!actionData->EquipmentClass)
	{
		equipment = owner->GetWorld()->SpawnActorDeferred<ACEquipment>(actionData->EquipmentClass, transform, owner);
		equipment->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		equipment->SetActorLabel(GetLableName(owner, "Equipment"));
		equipment->SetData(actionData->EquipmentData);

		UGameplayStatics::FinishSpawningActor(equipment, transform);
	
		if (!!attachment)
		{
			equipment->OnEquipmentDelegate.AddDynamic(attachment, &ACAttachment::OnEquip);
			equipment->OnUnequipmentDelegate.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}
	
	ACDoAction* doAction = NULL;
	
	if (!!actionData->DoActionClass)
	{
		doAction = owner->GetWorld()->SpawnActorDeferred<ACDoAction>(actionData->DoActionClass, transform, owner);
		doAction->AttachToComponent(owner->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true));
		doAction->SetActorLabel(GetLableName(owner, "DoAction"));
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
	
	Data = NewObject<UCAction>();
	Data->ItemName = FName(*InItemName);
	Data->Type = actionData->ActionType;
	Data->Attachment = attachment;
	Data->Equipment = equipment;
	Data->DoAction = doAction;

	SetMode(prevType);

	if (OnActionTypeChanged.IsBound())
		OnActionTypeChanged.Broadcast(prevType, Data->Type);
}

bool UCActionComponent::IsUnarmedMode()
{
	CheckNullResult(Data, false);
	return Data->Type == EActionType::Unarmed;
}

bool UCActionComponent::IsOneHandMode()
{
	CheckNullResult(Data, false);
	return Data->Type == EActionType::OneHand;
}

UCActionComponent::UCActionComponent()
{
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCActionComponent::OffAllCollision()
{
	CheckNull(Data);

	ACAttachment* attachment = Data->GetAttachment();
	CheckNull(attachment);

	attachment->OffCollision();
}

void UCActionComponent::DestroyAllActions()
{
	CheckNull(Data);

	if (!!Data->GetAttachment())
		Data->GetAttachment()->Destroy();

	if (!!Data->GetEquipment())
		Data->GetEquipment()->Destroy();

	if (!!Data->GetDoAction())
		Data->GetDoAction()->Destroy();
}


void UCActionComponent::SetMode(EActionType InPrevType)
{
	if (InPrevType == Data->Type || IsUnarmedMode())
	{
		ACEquipment* equipment = Data->GetEquipment();
		if (!!equipment)
			equipment->Unequip();

		return;
	}

	else if (IsUnarmedMode() == false)
	{
		ACEquipment* equipment = Data->GetEquipment();
		CheckNull(equipment);

		equipment->Unequip();
	}

	if (!!Data)
	{
		ACEquipment* equipment = Data->GetEquipment();
		CheckNull(equipment);

		equipment->Equip();
	}
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
	CheckNull(Data);
	CheckNull(Data->GetDoAction());

	ACDoAction* action = Data->GetDoAction();

	if (!!action)
		action->DoAction();
}

void UCActionComponent::DoSkill(const FString& InSkillName)
{
	CheckTrue(IsUnarmedMode());
	CheckNull(Data);
	CheckNull(Data->GetDoAction());

	ACDoAction* action = Data->GetDoAction();

	if (!!action)
		action->DoSkill(InSkillName);
}

void UCActionComponent::DoSkill(const int32& InIndex)
{
	CheckTrue(IsUnarmedMode());
	CheckNull(Data);
	CheckNull(Data->GetDoAction());

	ACDoAction* action = Data->GetDoAction();

	if (!!action)
		action->DoSkill(InIndex);
}