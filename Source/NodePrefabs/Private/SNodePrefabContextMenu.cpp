// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "SNodePrefabContextMenu.h"
#include "EditorStyle.h"
#include "AssetRegistryModule.h"
#include "AssetData.h"
#include "NodePrefab.h"
#include "EdGraphUtilities.h"
#include "Private/SGraphEditorImpl.h"
#include "NodePrefabLibrary.h"
#include "AssetEditorManager.h"
#include "EdGraphNode_Comment.h"
#include "KismetEditorUtilities.h"


void SNodePrefabContextMenu::SpawnPrefab(const FAssetData& assetData)
{
	UNodePrefab* nodePrefab = Cast<UNodePrefab>(assetData.GetAsset());

	TSharedPtr<SGraphEditorImpl> graphEditorPinned = graphEditor.Pin();
	if (nodePrefab && graphEditorPinned.IsValid())
	{
		nodePrefab->PasteIntoGraph(graphEditorPinned);//, mouseEvent.GetScreenSpacePosition());
	}
}

void SNodePrefabContextMenu::Construct(const FArguments& InArgs, FPointerEvent inMouseEvent, TWeakPtr<SGraphEditorImpl> inGraphEditor)
{
	// Build the widget layout
	SBorder::Construct(SBorder::FArguments()
		.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
		.Padding(5)
		[
			SNew(SBox)
			.MaxDesiredHeight(350)
		[
			SAssignNew(scrollBox, SScrollBox)
		]
		]
	);

	scrollBox->SetScrollBarRightClickDragAllowed(true);

	graphEditor = inGraphEditor;
	mouseEvent = inMouseEvent;

	FNodePrefabLibrary::GetNodePrefabsForGraph(graphEditor, nodePrefabsFiltered);
	if (nodePrefabsFiltered.Num() > 0)
	{
		scrollBox->AddSlot()
			[
				SNew(SListView<UNodePrefab*>)
				.ItemHeight(24)
			.ListItemsSource(&nodePrefabsFiltered)
			.OnGenerateRow_Raw(this, &SNodePrefabContextMenu::OnGenerateListRow)
			];
	}

	if (scrollBox->GetChildren()->Num() == 0)
	{
		scrollBox->AddSlot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString(TEXT("No GraphNodePrefab Assets created for this graph type."))))
			];
	}

}

void SNodePrefabContextMenu::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObjects(nodePrefabsFiltered);
}

FString SNodePrefabContextMenu::GetReferencerName() const
{
	return TEXT("SNodePrefabContextMenu");
}

FReply SNodePrefabContextMenu::OnPrefabButtonClicked(UNodePrefab* prefab)
{

	TSharedPtr<SGraphEditorImpl> graphEditorPinned;
	if (graphEditor.IsValid())
	{
		graphEditorPinned = graphEditor.Pin();
		UBlueprint* blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(graphEditorPinned->GetCurrentGraph());
		FSlateRect previousSelectionBounds;
		const bool bBoundsValid = FKismetEditorUtilities::GetBoundsForSelectedNodes(blueprint, previousSelectionBounds, 50.0f);
	
		prefab->PasteIntoGraph(graphEditorPinned);

		// The editor auto selects pasted nodes.
		// If after pasting the prefab there is only one selected node, see if it is a comment
		FGraphPanelSelectionSet selectedAfter = graphEditorPinned->GetSelectedNodes();
		if (bBoundsValid && selectedAfter.Num() == 1)
		{
			if (selectedAfter.Array()[0]->GetClass() == UEdGraphNode_Comment::StaticClass())
			{
				// YEY it is! - Wrap it around the previous selected nodes
				UEdGraphNode_Comment* commentNode = Cast<UEdGraphNode_Comment>(selectedAfter.Array()[0]);
				previousSelectionBounds = previousSelectionBounds.ExtendBy(FMargin(0.f, 50.f, 0.f, 0.f)); // Increase the distance between comment head and nodes
				commentNode->NodePosX = previousSelectionBounds.GetTopLeft().X;
				commentNode->NodePosY = previousSelectionBounds.GetTopLeft().Y;
				commentNode->ResizeNode(previousSelectionBounds.GetSize());
			}
		}

	}

	FSlateApplication::Get().DismissMenuByWidget(AsShared()); // hack as the context menu is not closing directly
	return FReply::Handled();
}

TSharedRef<ITableRow> SNodePrefabContextMenu::OnGenerateListRow(UNodePrefab* item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<UNodePrefab*>, OwnerTable)
		.ShowSelection(false)
		.Content()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			// Color indicator
			SNew(SBox)
			.WidthOverride(9)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("AssetThumbnail.Border"))
		.BorderBackgroundColor(item->color)
		]
		]
	+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		[
			// Button to paste NodePrefab
			SNew(SButton)
			.ButtonColorAndOpacity(FLinearColor::Transparent)
		.OnClicked(this, &SNodePrefabContextMenu::OnPrefabButtonClicked, item)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FEditorStyle::GetColor("ForegroundColor"))
		//.Font(FEditorStyle::GetFontStyle("FontAwesome.14"))
		.Text(FText::FromString(item->displayName.IsEmpty() ? item->GetName() : item->displayName))
		]
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			// Button for editing the NodePrefab
			SNew(SButton)
			.ButtonColorAndOpacity(FLinearColor::Transparent)
		.ToolTipText(NSLOCTEXT("NodePrefabs", "NodePrefabs.ContextWidget.EditButton.Tooltip", "Open the NodePrefab to Edit"))
		.OnClicked_Lambda([item]() {
		FAssetEditorManager::Get().OpenEditorForAsset(item);
		return FReply::Handled();
	})
		[
			SNew(SImage)
			.Image(FEditorStyle::GetBrush("ToolBar.Icon"))
		]
		]
		];
}

