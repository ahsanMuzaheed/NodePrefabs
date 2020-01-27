// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "SNodePrefabContextMenu.h"
#include "EditorStyle.h"
#include "AssetRegistryModule.h"
#include "AssetData.h"
#include "NodePrefab.h"
#include "EdGraphUtilities.h"
#include "NodePrefabLibrary.h"
#include "Toolkits/AssetEditorManager.h"
#include "EdGraphNode_Comment.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "NodePrefabSettings.h"

FLinearColor buttonColor = FLinearColor(0.14902f, 0.14902f, 0.14902f, 1.f);

void SNodePrefabContextMenu::SpawnPrefab(const FAssetData& assetData)
{
	UNodePrefab* nodePrefab = Cast<UNodePrefab>(assetData.GetAsset());

	TSharedPtr<SGraphEditor> graphEditorPinned = graphEditor.Pin();
	if (nodePrefab && graphEditorPinned.IsValid())
	{
		nodePrefab->PasteIntoGraph(graphEditorPinned);
	}
}

void SNodePrefabContextMenu::Construct(const FArguments& InArgs, FPointerEvent inMouseEvent, TWeakPtr<SGraphEditor> inGraphEditor)
{
	FSlateFontInfo titleFont = FCoreStyle::GetDefaultFontStyle("Bold", 12);

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
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
		.FillWidth(1.0)
		.HAlign(HAlign_Fill)
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Center)
		.Text(FText::FromString(TEXT("NodePrefabs")))
		.Font(titleFont)
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		.Padding(5.f, 0.f, 0.f, 0.f)
		[
			SNew(SButton)
			.ButtonColorAndOpacity(buttonColor)
			.OnClicked_Lambda([]() {
		UNodePrefabSettings* settings = UNodePrefabSettings::Get();
		FModuleManager::LoadModuleChecked<ISettingsModule>("Settings").ShowViewer(settings->GetContainerName()
			, settings->GetCategoryName(), settings->GetSectionName()); // Copied from FSettingsMenu::OpenSettings
		return FReply::Handled();
	})
		[
			SNew(SImage)
			.Image(FEditorStyle::GetBrush("ToolBar.Icon"))
		]
		]
		]
		]
		]
	);

	graphEditor = inGraphEditor;
	mouseEvent = inMouseEvent;

	// Clear Category expansion states if the user does not want them.
	if (!UNodePrefabSettings::Get()->bKeepCategoriesExpanded)
	{
		UNodePrefabSettings::Get()->expandedCategories.Empty();
	}

	// Build TreeStructure
	treeRoot = MakeShareable(new FTreeNode(TSharedPtr<FTreeNode>(nullptr), TEXT("")));
	FNodePrefabLibrary::GetNodePrefabsForGraph(graphEditor, nodePrefabsFiltered);
	for (UNodePrefab* prefab : nodePrefabsFiltered)
	{
		treeRoot->AddPrefab_RootOnly(prefab);
	}
	treeRoot->SortTree_RootOnly();

	if (nodePrefabsFiltered.Num() > 0)
	{
		TSharedPtr<STreeView<TreeType>> treeView;

		verticalBox->AddSlot()
			[
				SNew(SBox)
				.MaxDesiredHeight(450)
			[

				SAssignNew(treeView, STreeView<TreeType>)
				.ItemHeight(24)
			.TreeItemsSource(&(treeRoot->children))
			.OnGenerateRow_Raw(this, &SNodePrefabContextMenu::OnTreeViewGenerateRow)
			.OnGetChildren_Raw(this, &SNodePrefabContextMenu::OnTreeViewGetChildren)
			.OnExpansionChanged_Raw(this, &SNodePrefabContextMenu::OnTreeViewExpansionChanged)
			]
			];

		treeView->SetOnEntryInitialized(SListView<TreeType>::FOnEntryInitialized::CreateLambda(
			[this](TreeType item, const TSharedRef<ITableRow>&row)
		{
			// At this point the row widget is fully setup and registered.
		
			// Handle Category expansion
			UNodePrefabSettings* settings = UNodePrefabSettings::Get();
			if (settings->bKeepCategoriesExpanded
				&& settings->expandedCategories.Contains(item->GetCategoryFull())
				&& !row->IsItemExpanded())
			{
				row->ToggleExpansion();
			}
		
		}));
	}
	else
	{
		verticalBox->AddSlot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString(TEXT("No NodePrefab Assets created for this graph type\nor the include/exclude configuration is wrong."))))
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
		TSharedPtr<SGraphEditor> graphEditorPinned = graphEditor.Pin();
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
				.ButtonColorAndOpacity(buttonColor)
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
				.ButtonColorAndOpacity(buttonColor)
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
		// Create TableRow for category
		FSlateFontInfo categoryFont = FCoreStyle::Get().GetFontStyle("NormalText");
		categoryFont.Size = 11;

		row->SetContent(
			SNew(STextBlock)
			.Text(FText::FromString(item->categoryPart))
			.Font(categoryFont)
		);

		// Can't handle expansion state of the category widget yet, see SetOnEntryInitialized
	}

	return row.ToSharedRef();
}

void SNodePrefabContextMenu::OnTreeViewExpansionChanged(TreeType item, bool bExpanded)
{
	UNodePrefabSettings* settings = UNodePrefabSettings::Get();
	if (bExpanded)
	{
		if (settings->bKeepCategoriesExpanded && !item->categoryPart.IsEmpty())
		{
			settings->expandedCategories.AddUnique(item->GetCategoryFull());
		}
	}
	else
	{
		settings->expandedCategories.RemoveSingleSwap(item->GetCategoryFull());
	}
}

void SNodePrefabContextMenu::FTreeNode::AddPrefab_RootOnly(UNodePrefab* inPrefab)
{
	if (!inPrefab) return;

	TArray<FString> categoriesSplit;
	if (!inPrefab->category.IsEmpty())
	{
		inPrefab->category.ParseIntoArray(categoriesSplit, TEXT("|"));
	}

	// Look for the category node to place our NodePrefab in
	TSharedPtr<FTreeNode> currentNode = AsShared();
	for (const FString& categoryPartInternal : categoriesSplit)
	{
		TSharedPtr<FTreeNode>* found = currentNode->children.FindByPredicate([&categoryPartInternal](const TSharedPtr<FTreeNode>& child) {
			return child->categoryPart == categoryPartInternal;
		}
		);

		if (found)
		{
			currentNode = *found;
		}
		else
		{
			currentNode = currentNode->children.Add_GetRef(MakeShareable(new FTreeNode(AsShared(), categoryPartInternal)));
		}
	}

	// Place the NodePrefab
	currentNode->children.Add(MakeShareable(new FTreeNode(AsShared(), inPrefab)));
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
		
		return a->categoryPart < b->categoryPart;
	});

	for (TSharedPtr<FTreeNode> child : children)
	{
		child->SortTree_RootOnly();
	}
}

FString SNodePrefabContextMenu::FTreeNode::GetCategoryFull()
{
	if (parent.IsValid())
	{
		FString category = parent->GetCategoryFull();

		if (!category.IsEmpty())
		{
			category.Append(TEXT("|"));
		}

		category.Append(categoryPart);

		return category;
	}
	else
	{
		return FString();
	}
}
