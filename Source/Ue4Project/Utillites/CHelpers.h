#pragma once
#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"

class UE4PROJECT_API CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");
		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
		verifyf(!!obj, L"asset");
		*OutObject = obj;
	}

	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);

			return;
		}

		InActor->SetRootComponent((*InComponent));
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutClass = asset.Class;
	}

	template<typename T>
	static void FindActors(class UWorld* InWorld, TArray<T*>& OutActors)
	{
		OutActors.Empty();

		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(InWorld, T::StaticClass(), actors);

		for (AActor* actor : actors)
			OutActors.Add(Cast<T>(actor));
	}

	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** InComponent, FName InName)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);
	}


	// 컴포넌트 하나 가져옴(어느 액터에 있는)
	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		// 액터에 해당 클래스로 된 컴포넌트를 하나 찾아줌 
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T>
	static FString GetEnumToString(T InType, FName InEnumName)
	{
		const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, *InEnumName.ToString(), true);
		if (!EnumPtr)
			return FString("Invalid");
		return EnumPtr->GetNameStringByValue((int64)InType);
	}

	// Only Defined -> ENUM_RANGE_BY_COUNT(T, MaxCount);
	template<typename T>
	static T GetStringToEnum(FName InEnumName, FString InChangedName)
	{
		for (T iter : TEnumRange<T>())
		{
			if (GetEnumToString<T>(iter, InEnumName) == InChangedName)
				return iter;
		}

		return T();
	}
};