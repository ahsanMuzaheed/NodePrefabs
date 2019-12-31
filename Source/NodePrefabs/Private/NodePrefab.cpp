// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "NodePrefab.h"
#include "NodePrefabLibrary.h"

bool UNodePrefab::PasteIntoGraph(TSharedPtr<SGraphEditor> graphEditor)
{
	return FNodePrefabLibrary::PasteNodesIntoGraph(graphEditor, prefab);
}
