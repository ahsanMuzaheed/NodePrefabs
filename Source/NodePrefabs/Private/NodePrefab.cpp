// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "NodePrefab.h"
#include "NodePrefabLibrary.h"
#include "NodePrefabSettings.h"

UNodePrefab::UNodePrefab()
{
	color = UNodePrefabSettings::Get()->DefaultNodePrefabColor;
}

bool UNodePrefab::PasteIntoGraph(TSharedPtr<SGraphEditor> graphEditor) const
{
	return FNodePrefabLibrary::PasteNodesIntoGraph(graphEditor, prefab);
}
