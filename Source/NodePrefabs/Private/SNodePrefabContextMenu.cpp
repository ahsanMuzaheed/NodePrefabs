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
#include "SGraphActionMenu.h"
#include "SExpanderArrow.h"

#include "UObject/StrongObjectPtr.h"

FLinearColor buttonColor = FLinearColor(0.14902f, 0.14902f, 0.14902f, 1.f);

/** This structure represents a context dependent action, with sufficient information for the schema to perform it. */
struct FNodePrefabContextMenuAction : public FEdGraphSchemaAction
{
	/** Simple type info. */
	static FName StaticGetTypeId() { static FName Type("FNodePrefabContextMenuAction"); return Type; }
	virtual FName GetTypeId() const override { return StaticGetTypeId(); }

	FNodePrefabContextMenuAction(UNodePrefab* inNodePrefab) : nodePrefab(inNodePrefab)
	{
		UpdateSearchData(FText(), FText()
			, FText::FromString(nodePrefab->category), FText::FromString(nodePrefab->displayName + nodePrefab->GetName()));
	}

	TStrongObjectPtr<UNodePrefab> nodePrefab;
};

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
			+ SHorizontalBox::Slot()
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

#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
	// Clear Category expansion states if the user does not want them.
	if (!UNodePrefabSettings::Get()->bKeepCategoriesExpanded)
	{
		UNodePrefabSettings::Get()->expandedCategories.Empty();
	}
#endif
	
	verticalBox->AddSlot()
		[
			SNew(SBox)
			.MaxDesiredHeight(450)
		[
			SAssignNew(graphActionMenu, SGraphActionMenu)
			.ShowFilterTextBox(UNodePrefabSettings::Get()->bShowSearchBar)
		.OnActionSelected(this, &SNodePrefabContextMenu::OnGraphActionMenu_OnActionSelected)
		.OnCollectAllActions(this, &SNodePrefabContextMenu::OnGraphActionMenu_CollectAllActions)
		.OnCreateWidgetForAction(SGraphActionMenu::FOnCreateWidgetForAction::CreateSP(this, &SNodePrefabContextMenu::OnGraphActionMenu_CreateWidgetForAction))
		// Need a custom expander cause SGraphActionMenu messes up the indent level with the default extender
		.OnCreateCustomRowExpander_Lambda([](FCustomExpanderData const& expanderData) {return SNew(SExpanderArrow, expanderData.TableRow); })
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
		.OnExpansionChanged(this, &SNodePrefabContextMenu::OnGraphActionMenu_ExpansionChanged)
		.OnEntryInitialised(this, &SNodePrefabContextMenu::OnGraphActionMenu_OnEntryInitialized)
#endif
		]
		];

	// Focus to the search bar of FGraphActionMenu is set from Outside, does not work in Constructor
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

#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
void SNodePrefabContextMenu::OnGraphActionMenu_ExpansionChanged(TSharedPtr<FGraphActionNode> item, bool bExpanded)
{
	UNodePrefabSettings* settings = UNodePrefabSettings::Get();
	if (bExpanded)
	{
		if (settings->bKeepCategoriesExpanded && item->IsCategoryNode())
		{
			settings->expandedCategories.AddUnique(item->GetCategoryPath().ToString());
		}
	}
	else
	{
		settings->expandedCategories.RemoveSingleSwap(item->GetCategoryPath().ToString());
	}
}

void SNodePrefabContextMenu::OnGraphActionMenu_OnEntryInitialized(TSharedPtr<FGraphActionNode> item, bool bExpanded)
{
	UNodePrefabSettings* settings = UNodePrefabSettings::Get();
	if (item->IsCategoryNode())
	{
		TODO Restore expansion state
	}
}

#endif

void SNodePrefabContextMenu::OnGraphActionMenu_CollectAllActions(FGraphActionListBuilderBase& OutAllActions)
{
	TArray<UNodePrefab*> nodePrefabsFiltered;
	FNodePrefabLibrary::GetNodePrefabsForGraph(graphEditor, nodePrefabsFiltered);
	if (nodePrefabsFiltered.Num() > 0)
	{
		for (UNodePrefab* prefab : nodePrefabsFiltered)
		{
			OutAllActions.AddAction(MakeShareable(new FNodePrefabContextMenuAction(prefab)));
		}
	}
	else
	{
		// Dummy to display a help text why no NodePrefabs are displayed
		OutAllActions.AddAction(MakeShareable(new FEdGraphSchemaAction_Dummy(
			FText(), FText(), FText(), 0
		)));
	}
}

TSharedRef<SWidget> SNodePrefabContextMenu::OnGraphActionMenu_CreateWidgetForAction(FCreateWidgetForActionData* const InCreateData)
{
	TSharedPtr<SHorizontalBox> widget;
	if (InCreateData->Action->GetTypeId() == FNodePrefabContextMenuAction::StaticGetTypeId())
	{
		TSharedPtr<FNodePrefabContextMenuAction> prefabAction = StaticCastSharedPtr<FNodePrefabContextMenuAction>(InCreateData->Action);
		if (prefabAction->nodePrefab)
		{
			// Create TableRow for the prefab
			SAssignNew(widget, SHorizontalBox)
				// Color indicator
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SBox)
					.WidthOverride(9)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("AssetThumbnail.Border"))
				.BorderBackgroundColor(prefabAction->nodePrefab->color)
				]
				]
			// Button to paste NodePrefab
			+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				[
					SNew(SButton)
					.ButtonColorAndOpacity(buttonColor)
				.OnClicked(this, &SNodePrefabContextMenu::OnPrefabButtonClicked, prefabAction->nodePrefab.Get())
				[
					SNew(STextBlock)
					.ColorAndOpacity(FEditorStyle::GetColor("ForegroundColor"))
				//.Font(FEditorStyle::GetFontStyle("FontAwesome.14"))
				.Text(FText::FromString(prefabAction->nodePrefab->GetListName()))
				]
				]
			// Button for editing the NodePrefab
			+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.ButtonColorAndOpacity(buttonColor)
				.ToolTipText(NSLOCTEXT("NodePrefabs", "NodePrefabs.ContextWidget.EditButton.Tooltip", "Open the NodePrefab to Edit"))
				.OnClicked_Lambda([prefabAction]() {
				FAssetEditorManager::Get().OpenEditorForAsset(prefabAction->nodePrefab.Get());
				return FReply::Handled();
			})
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("ToolBar.Icon"))
				]
				];

		}
	}
	else if (InCreateData->Action->GetTypeId() == FEdGraphSchemaAction_Dummy::StaticGetTypeId())
	{
		SAssignNew(widget, SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString(TEXT("No NodePrefab Assets created for this graph type\nor the include/exclude configuration is wrong."))))
			];
	}
	return widget.ToSharedRef();
}

void SNodePrefabContextMenu::OnGraphActionMenu_OnActionSelected(const TArray< TSharedPtr<FEdGraphSchemaAction> >& SelectedAction, ESelectInfo::Type InSelectionType)
{
	if (InSelectionType == ESelectInfo::OnKeyPress && SelectedAction.IsValidIndex(0))
	{
		if( SelectedAction[0]->GetTypeId() == FNodePrefabContextMenuAction::StaticGetTypeId())
		{	
			TSharedPtr<FNodePrefabContextMenuAction> prefabAction = StaticCastSharedPtr<FNodePrefabContextMenuAction>(SelectedAction[0]);
			
			// Just use the button function, as we want to do exactly that
			OnPrefabButtonClicked(prefabAction->nodePrefab.Get());
		}
	}
}
