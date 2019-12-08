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
			SAssignNew(verticalBox, SVerticalBox)
			+ SVerticalBox::Slot()
		.VAlign(VAlign_Top)
		.AutoHeight()
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
		.Padding(5)
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Center)
		.Text(FText::FromString(TEXT("NodePrefabs")))
		]
		]
		]
	);

	//scrollBox->SetScrollBarRightClickDragAllowed(true);

	graphEditor = inGraphEditor;
	mouseEvent = inMouseEvent;

	// Build TreeStructure
	treeRoot = MakeShareable(new FTreeNode(TEXT("")));

	FNodePrefabLibrary::GetNodePrefabsForGraph(graphEditor, nodePrefabsFiltered);
	for (UNodePrefab* prefab : nodePrefabsFiltered)
	{
		treeRoot->AddPrefab_RootOnly(prefab);
	}
	treeRoot->SortTree_RootOnly();

	if (nodePrefabsFiltered.Num() > 0)
	{
		verticalBox->AddSlot()
			[
				SNew(SBox)
				.MaxDesiredHeight(450)
			[
				
					SNew(STreeView<TreeType>)
					.ItemHeight(24)
			.TreeItemsSource(&(treeRoot->children))
			.OnGenerateRow_Raw(this, &SNodePrefabContextMenu::OnTreeViewGenerateRow)
			.OnGetChildren_Raw(this, &SNodePrefabContextMenu::OnTreeViewGetChildren)
				
			]
			];
	}
	else
	{
		verticalBox->AddSlot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString(TEXT("No NodePrefab Assets created for this graph type."))))
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
	if (graphEditor.IsValid())
	{
		TSharedPtr<SGraphEditorImpl> graphEditorPinned = graphEditor.Pin();
		FSlateRect previousSelectionBounds;
		const bool bBoundsValid = graphEditorPinned->GetBoundsForSelectedNodes(previousSelectionBounds, 50.0f);

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

TSharedRef<ITableRow> SNodePrefabContextMenu::OnTreeViewGenerateRow(TreeType item, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedPtr<STableRow<TreeType>> row;
	SAssignNew(row, STableRow<TreeType>, OwnerTable)
		.ShowSelection(false);

	if (item->prefab)
	{
		// Create TableRow for the prefab
		row->SetContent(
			SNew(SHorizontalBox)
			// Color indicator
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(9)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("AssetThumbnail.Border"))
			.BorderBackgroundColor(item->prefab->color)
			]
			]
		// Button to paste NodePrefab
		+ SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SNew(SButton)
				.ButtonColorAndOpacity(FLinearColor(0.14902f, 0.14902f, 0.14902f, 1.f))
			.OnClicked(this, &SNodePrefabContextMenu::OnPrefabButtonClicked, item->prefab)
			[
				SNew(STextBlock)
				.ColorAndOpacity(FEditorStyle::GetColor("ForegroundColor"))
			//.Font(FEditorStyle::GetFontStyle("FontAwesome.14"))
			.Text(FText::FromString(item->prefab->GetListName()))
			]
			]
		// Button for editing the NodePrefab
		+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.ButtonColorAndOpacity(FLinearColor(0.14902f, 0.14902f, 0.14902f, 1.f))
			.ToolTipText(NSLOCTEXT("NodePrefabs", "NodePrefabs.ContextWidget.EditButton.Tooltip", "Open the NodePrefab to Edit"))
			.OnClicked_Lambda([item]() {
			FAssetEditorManager::Get().OpenEditorForAsset(item->prefab);
			return FReply::Handled();
		})
			[
				SNew(SImage)
				.Image(FEditorStyle::GetBrush("ToolBar.Icon"))
			]
			]
		);
	}
	else
	{
		FSlateFontInfo categoryFont = FCoreStyle::Get().GetFontStyle("NormalText");
		categoryFont.Size = 11;

		// Create TableRow for category
		row->SetContent(
			SNew(STextBlock)
			.Text(FText::FromString(item->name))
			.Font(categoryFont)
		);
	}

	return row.ToSharedRef();
}

void SNodePrefabContextMenu::FTreeNode::AddPrefab_RootOnly(UNodePrefab* inPrefab)
{
	if (!inPrefab) return;

	TArray<FString> categories;
	if (!inPrefab->category.IsEmpty())
	{
		inPrefab->category.ParseIntoArray(categories, TEXT("|"));
	}

	TSharedPtr<FTreeNode> currentNode = AsShared();
	for (const FString& category : categories)
	{
		TSharedPtr<FTreeNode>* found = currentNode->children.FindByPredicate([&category](const TSharedPtr<FTreeNode>& child) {
			return child->name == category;
		}
		);

		if (found)
		{
			currentNode = *found;
		}
		else
		{
			currentNode = currentNode->children.Add_GetRef(MakeShareable(new FTreeNode(category)));
		}
	}

	currentNode->children.Add(MakeShareable(new FTreeNode(inPrefab)));
}

void SNodePrefabContextMenu::FTreeNode::SortTree_RootOnly()
{
	children.Sort([](const TSharedPtr<FTreeNode>& a, const TSharedPtr<FTreeNode>& b) {
		if (a->prefab && b->prefab)
		{
			return a->prefab->GetListName() < b->prefab->GetListName();
		}

		// Keep all prefabs below the categories
		if (a->prefab)
		{
			return false;
		}
		else if (b->prefab)
		{
			return true;
		}
		
		return a->name < b->name;
	});

	for (TSharedPtr<FTreeNode> child : children)
	{
		child->SortTree_RootOnly();
	}
}
