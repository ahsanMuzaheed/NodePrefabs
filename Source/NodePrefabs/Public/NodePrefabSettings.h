// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "CoreMinimal.h"
#include "NodePrefabSettings.generated.h"

/**
 * Settings class for NodePrefabs
 */
UCLASS(config=EditorPerProjectUserSettings)
class NODEPREFABS_API UNodePrefabSettings : public UObject
{
public:
	GENERATED_BODY()

	UNodePrefabSettings();
	~UNodePrefabSettings();

	/**
	 * Only Assets defined by include Paths are part of the Graph Menu
	 */
	UPROPERTY(config, EditAnywhere, Category = "Graph Menu", meta = (LongPackageName))
	TArray<FDirectoryPath> PathToInclude;

	/**
	 * Only Assets defined by include Paths are part of the Graph Menu
	 */
	UPROPERTY(config, EditAnywhere, Category = "Graph Menu", meta = (LongPackageName))
	TArray<FDirectoryPath> PathToIncludeRecursive;

	/**
	 * From the paths that shall be listed, exclude these
	 */
	UPROPERTY(config, EditAnywhere, Category = "Graph Menu", meta = (LongPackageName))
	TArray<FDirectoryPath> PathToExclude;

	/**
	 * From the paths that shall be listed, exclude these
	 */
	UPROPERTY(config, EditAnywhere, Category = "Graph Menu", meta = (LongPackageName))
	TArray<FDirectoryPath> PathToExcludeRecursive;

	/**
	 * Check to keep the expanded state of categories
	 */
	UPROPERTY(config, EditAnywhere, Category = "Graph Menu")
	bool bKeepCategoriesExpanded = false;

	// List of categories that have been expanded
	UPROPERTY(config)
	TArray<FString> expandedCategories;

	static FName GetContainerName()
	{
		return settingsContainer;
	}

	static FName GetCategoryName()
	{
		return settingsCategory;
	}

	static FName GetSectionName()
	{
		return settingsSection;
	}

	static UNodePrefabSettings* Get()
	{
		return GetMutableDefault<UNodePrefabSettings>();
	}

private:
	static FName settingsContainer;
	static FName settingsCategory;
	static FName settingsSection;
};
