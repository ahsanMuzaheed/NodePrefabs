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

	UPROPERTY(config, EditAnywhere, Category = "NodePrefab")
	FLinearColor DefaultNodePrefabColor;

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
	 * Check to show to show the search bar in the GraphMenu
	 */
	UPROPERTY(config, EditAnywhere, Category = "Graph Menu")
	bool bShowSearchBar = true;

#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
#include "KeepExpanded.inl"
#endif



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
