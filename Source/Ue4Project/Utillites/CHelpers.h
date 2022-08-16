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

class UE4PROJECT_API CMath
{
public:
	static void GetBezierCurve(FVector InP0, FVector InP1, FVector InP2, FVector InP3, float InT, FVector& OutResult)
	{
		// 1. 일반적인 계산

		// 사용하게될 수식 미리 연산
		// float u = 1.0f - InT;
		// float t2 = InT * InT;
		// float u2 = u * u;
		// float u3 = u2 * u;
		// float t3 = t2 * InT;

		// 4개의 점이 주어졌을때 p0와 p3를 p1과 p2를 향하여 보간
		// OutResult = u3 * InP0 + (3.0f * InT * u2 ) * InP1 + (3.0f * t2 * u) * InP2 + t3 * InP3;

		// 2. Lerp를 활용(InT 값만큼 연산하여 최종적인 점의 위치를 구함) 
		FVector a = FMath::Lerp(InP0, InP1, InT);
		FVector b = FMath::Lerp(InP1, InP2, InT);
		FVector c = FMath::Lerp(InP2, InP3, InT);

		FVector d = FMath::Lerp(a, b, InT);
		FVector e = FMath::Lerp(b, c, InT);

		OutResult = FMath::Lerp(d, e, InT); // 최종 값(f)
	}
};