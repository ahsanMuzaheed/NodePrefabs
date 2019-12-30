// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "SharedPointer.h"
#include "UnrealString.h"
#include "NoExportTypes.h"
#include "StrongObjectPtr.h"

class SGraphEditorImpl;
class FChildren;
class SGraphPanel;
class UNodePrefab;

class FNodePrefabLibrary
{
public:
	/**
	 * Returns the GraphEditor under the mouse cursor if there is any.
	 */
	static TSharedPtr<SGraphEditorImpl> GetGraphEditorUnderMouse(const FVector2D CursorPos);

	/**
	 * Tries to past the given string node representation into the given graph. Returns true when successful
	 */
	static bool PasteNodesIntoGraph(TSharedPtr<SGraphEditorImpl> graphEditor, const FString& nodesAsString);
	
	static void GetNodePrefabsForGraph(TWeakPtr<SGraphEditorImpl> graph, TArray<UNodePrefab*>& outPrefabs);
	static void GetNodePrefabsForGraph(TWeakPtr<SGraphEditorImpl> graph, TArray<TStrongObjectPtr<UNodePrefab>>& outPrefabs);
	
	/**
	 * Evaluates if we shall include the given asset depending on the settings.
	 */
	static bool ShallIncludeNodePrefab(const FAssetData& asset);

	static TSharedPtr<SWidget> FindChildWidgetRecursive(FChildren* children, const FName& widgetType);
};