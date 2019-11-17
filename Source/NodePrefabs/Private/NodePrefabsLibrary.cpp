// Copyright 2019 Guenther Schrenk (Rumbleball)

#include "NodePrefabLibrary.h"
#include "SlateApplication.h"
#include "WidgetPath.h"
#include "WindowsPlatformApplicationMisc.h"
#include "AssetEditorManager.h"
#include "BlueprintEditorUtils.h"
#include "Private/SGraphEditorImpl.h"
#include "BlueprintEditorModule.h"
#include "SharedPointer.h"
#include "SGraphPanel.h"
#include "NodePrefab.h"
#include "ModuleManager.h"
#include "AssetRegistryModule.h"
#include "EdGraphUtilities.h"
#include "StrongObjectPtr.h"



TSharedPtr<SGraphEditorImpl> FNodePrefabLibrary::GetGraphEditorUnderMouse(const FVector2D CursorPos)
{
	FSlateApplication& slateApp = FSlateApplication::Get();
	FWidgetPath widgetPath = slateApp.LocateWindowUnderMouse(CursorPos, slateApp.GetInteractiveTopLevelWindows());
	// Return when there are now widgets.
	if (!widgetPath.IsValid()) return false;

	FName graphEdtiorImplName("SGraphEditorImpl");
	for (const FArrangedWidget& widget : widgetPath.Widgets.GetInternalArray())
	{
		if (widget.Widget->GetType() == graphEdtiorImplName)
		{
			TSharedRef<SGraphEditorImpl> graphEditor = StaticCastSharedRef<SGraphEditorImpl>(widget.Widget);
			return TSharedPtr<SGraphEditorImpl>(graphEditor);
		}
	}
	return TSharedPtr<SGraphEditorImpl>(nullptr);
}

bool FNodePrefabLibrary::PasteNodesIntoGraph(TSharedPtr<SGraphEditorImpl> graphEditor, const FString& nodesAsString)
{
	if (graphEditor.IsValid() && graphEditor->GetCurrentGraph())
	{
		FString clipBoardBackup;
		FPlatformApplicationMisc::ClipboardPaste(clipBoardBackup);

		// Copy the prefab to the clipboard
		FPlatformApplicationMisc::ClipboardCopy(*nodesAsString);
		// Trigger Ctrl+V for paste operation
		const uint32 keyCode = 86;
		const uint32 characterCode = 86;
		const FKey key = FInputKeyManager::Get().GetKeyFromCodes(keyCode, characterCode);
		const FModifierKeysState modifierState(false, false, true, false, false, false, false, false, false);
		FKeyEvent keyEvent(key, modifierState, FSlateApplication::Get().GetUserIndexForKeyboard(), false, characterCode, keyCode);
		graphEditor->OnKeyDown(graphEditor->GetCachedGeometry(), keyEvent);
		
		// Restore the old Clipboard
		FPlatformApplicationMisc::ClipboardCopy(*clipBoardBackup);
	}

	return false;
}

void FNodePrefabLibrary::GetNodePrefabsForGraph(TWeakPtr<SGraphEditorImpl> graph, TArray<UNodePrefab*>& outPrefabs)
{
	TSharedPtr<SGraphEditorImpl> graphPinned = graph.Pin();
	if (graphPinned.IsValid())
	{
		TArray<FAssetData> assetData;
		FAssetRegistryModule& assetRegistry = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		assetRegistry.Get().GetAssetsByClass(UNodePrefab::StaticClass()->GetFName(), assetData, false);

		for (const FAssetData& data : assetData)
		{
			UNodePrefab* nodePrefab = Cast<UNodePrefab>(data.GetAsset());

			// Only populate with Prefabs that can be used in the current editor.
			if (FEdGraphUtilities::CanImportNodesFromText(graphPinned->GetCurrentGraph(), nodePrefab->prefab))
			{
				outPrefabs.Add(nodePrefab);
			}
		}
	}
}

void FNodePrefabLibrary::GetNodePrefabsForGraph(TWeakPtr<SGraphEditorImpl> graph, TArray<TStrongObjectPtr<UNodePrefab>>& outPrefabs)
{
	TArray<UNodePrefab*> prefabs;
	GetNodePrefabsForGraph(graph, prefabs);
	outPrefabs.Reserve(prefabs.Num());
	for (UNodePrefab* iter : prefabs)
	{
		outPrefabs.Add(TStrongObjectPtr<UNodePrefab>(iter));
	}
}

TSharedPtr<SWidget> FNodePrefabLibrary::FindChildWidgetRecursive(FChildren* children, const FName& widgetType)
{
	if(!children) return TSharedPtr<SWidget>(nullptr);

	for (int32 i = 0; i < children->Num(); ++i)
	{
		TSharedRef<SWidget> child = children->GetChildAt(i);
		if (child->GetType() == widgetType)
		{
			return TSharedPtr<SWidget>(child);
		}
		else
		{
			FChildren* resursiveChildren = child->GetChildren();
			if (resursiveChildren)
			{
				TSharedPtr<SWidget> recursiveFind = FindChildWidgetRecursive(resursiveChildren, widgetType);
				if (recursiveFind.IsValid())
				{
					return recursiveFind;
				}
			}
		}
	}

	return TSharedPtr<SWidget>(nullptr);
}
