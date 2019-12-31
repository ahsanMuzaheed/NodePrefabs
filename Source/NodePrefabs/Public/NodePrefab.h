// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NodePrefab.generated.h"

class SGraphEditorImpl;

/**
 * 
 */
UCLASS(NotBlueprintType, hidecategories = (Object))
class NODEPREFABS_API UNodePrefab : public UObject
{
	GENERATED_BODY()
public:

	// (Optional) Display name in the context menu. If not set, the asset name is taken
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	FString displayName;

	// (Optional) Category for the context menu, separated by '|'. E.g.: Default|Debug|Print
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	FString category;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	FLinearColor color = FLinearColor::Transparent;

	UPROPERTY()
	FString prefab;

	FString GetListName()
	{
		return displayName.IsEmpty() ? GetName() : displayName;
	}


	virtual bool IsEditorOnly() const override
	{
		return true;
	}

	bool PasteIntoGraph(TSharedPtr<SGraphEditor> graphEditor);
};
