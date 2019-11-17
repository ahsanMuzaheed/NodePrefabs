// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "NodePrefab.h"
#include "NodePrefabFactory.generated.h"

/**
 * 
 */
UCLASS()
class NODEPREFABS_API UNodePrefabFactory : public UFactory
{
	GENERATED_BODY()
	UNodePrefabFactory()
	{
		SupportedClass = UNodePrefab::StaticClass();
		bCreateNew = true;
		bEditAfterNew = true;
	}

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags
		, UObject* Context, FFeedbackContext* Warn)  override;

	virtual bool ShouldShowInNewMenu() const override
	{
		return true;
	}
};
