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
	UPROPERTY(EditDefaultsOnly)
	FString displayName;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor color = FLinearColor::Transparent;

	UPROPERTY()
	FString prefab;


	virtual bool IsEditorOnly() const override
	{
		return true;
	}

	bool PasteIntoGraph(TSharedPtr<SGraphEditorImpl> graphEditor);
};
