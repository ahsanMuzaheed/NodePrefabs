// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once


#include "NodePrefab.h"
#include "AssetTypeActions_Base.h"
#include "Templates/SharedPointer.h"

/**
 * Action for creating the NodePrefab asset.
 */
class  FNodePrefabActions : public FAssetTypeActions_Base
{
public:

	virtual uint32 GetCategories() override
	{
		return EAssetTypeCategories::Misc;
	}

	virtual FColor GetTypeColor() const override
	{
		return FColor::Green;
	}

	virtual UClass* GetSupportedClass() const override
	{
		return UNodePrefab::StaticClass();
	}

	virtual FText GetName() const override
	{
		return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_NodePrefab", "Node Prefab");
	}

	virtual void OpenAssetEditor(const TArray<UObject *>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

};
