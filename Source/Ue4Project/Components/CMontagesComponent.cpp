#include "Components/CMontagesComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCMontagesComponent::UCMontagesComponent()
{
}

void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	CheckNull(DataTable);

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);
	
	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		}//for(data)
	}//for(i)
}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackStep()
{
	PlayAnimMontage(EStateType::Backstep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayMantle1M()
{
	PlayAnimMontage(EStateType::Mantle1M);
}

void UCMontagesComponent::PlayMantle15M()
{
	PlayAnimMontage(EStateType::Mantle15M);
}

void UCMontagesComponent::PlayMantle2M()
{
	PlayAnimMontage(EStateType::Mantle2M);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InState)
{
	ACharacter* character = Cast<ACharacter>(GetOwner());

	const FMontageData* data = Datas[(int32)InState];

	if (!!data)
	{
		if (!!data->AnimMontage)
			character->PlayAnimMontage(data->AnimMontage, data->PlayRatio, data->StartSection);
	}
}