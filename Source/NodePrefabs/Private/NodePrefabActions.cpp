// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "NodePrefabActions.h"
#include "Toolkit/NodePrefabToolkit.h"



void FNodePrefabActions::OpenAssetEditor(const TArray<UObject *>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor /*= TSharedPtr<IToolkitHost>()*/)
{
	for (auto it = InObjects.CreateConstIterator(); it; ++it)
	{
		UNodePrefab* prefab = Cast<UNodePrefab>(*it);
		if (prefab)
		{
			FNodePrefabsToolkit::Create(prefab, EditWithinLevelEditor);
		}
	}
}
