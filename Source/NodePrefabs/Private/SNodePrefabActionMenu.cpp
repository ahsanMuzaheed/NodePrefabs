#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
TODO remove if not needed anymore
#endif

//// Copyright 2019 Guenther Schrenk (Rumbleball)
//
//
//#include "SNodePrefabActionMenu.h"
//#include "UObject/StrongObjectPtr.h"
//#include "Widgets/Input/SSearchBox.h"
//#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
//#include "GraphActionNode.h"
//#endif
//#include "SScrollBorder.h"
//
//
//void SNodePrefabActionMenu::Construct(const FArguments& InArgs, const bool bIsReadOnly /*= true*/)
//{
//	
//	SGraphActionMenu::Construct(SGraphActionMenu::FArguments()
//		.AutoExpandActionMenu(InArgs._AutoExpandActionMenu)
//	.AlphaSortItems(InArgs._AlphaSortItems)
//	.ShowFilterTextBox(InArgs._ShowFilterTextBox)
//	.UseSectionStyling(InArgs._UseSectionStyling)
//	.OnActionSelected(InArgs._OnActionSelected)
//	.OnActionDoubleClicked(InArgs._OnActionDoubleClicked)
//	.OnActionDragged(InArgs._OnActionDragged)
//		.OnCategoryDragged(InArgs._OnCategoryDragged)
//		//.OnContextMenuOpening(InArgs._OnContextMenuOpening)  // Is added to treeview
//		.OnCreateWidgetForAction(InArgs._OnCreateWidgetForAction)
//		.OnCreateCustomRowExpander(InArgs._OnCreateCustomRowExpander)
//		.OnCollectAllActions(InArgs._OnCollectAllActions)
//		.OnCollectStaticSections(InArgs._OnCollectStaticSections)
//		.OnCategoryTextCommitted(InArgs._OnCategoryTextCommitted)
//		.OnCanRenameSelectedAction(InArgs._OnCanRenameSelectedAction)
//		.OnGetSectionTitle(InArgs._OnGetSectionTitle)
//		.OnGetSectionToolTip(InArgs._OnGetSectionToolTip)
//		.OnGetSectionWidget(InArgs._OnGetSectionWidget)
//		.OnGetFilterText(InArgs._OnGetFilterText)
//		.OnActionMatchesName(InArgs._OnActionMatchesName)
//#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
//		.OnExpansionChanged(InArgs._InExpansionChanged)
//#endif
//		, bIsReadOnly);
//
//
//	check(FilteredRootAction.IsValid());
//	itemSource.Add(FilteredRootAction);
//
//	TreeView = SNew(STreeView<TreeType>)
//		.ItemHeight(24)
//		.TreeItemsSource(&itemSource) //.TreeItemsSource(&(this->FilteredRootAction->Children))
//		.OnGenerateRow_Lambda([this, bIsReadOnly](TSharedPtr<FGraphActionNode> InItem, const TSharedRef<STableViewBase>& OwnerTable) { return MakeWidget(InItem, OwnerTable, bIsReadOnly); })
//		.OnSelectionChanged_Lambda([this](TSharedPtr< FGraphActionNode > InSelectedItem, ESelectInfo::Type SelectInfo) { OnItemSelected(InSelectedItem, SelectInfo); })
//		.OnMouseButtonDoubleClick_Lambda([this](TSharedPtr< FGraphActionNode > InClickedItem) { OnItemDoubleClicked(InClickedItem); })
//		.OnContextMenuOpening(InArgs._OnContextMenuOpening)
//		.OnGetChildren_Lambda([this](TSharedPtr<FGraphActionNode> InItem, TArray< TSharedPtr<FGraphActionNode> >& OutChildren) { OnGetChildrenForCategory(InItem, OutChildren); })
//		.SelectionMode(ESelectionMode::Single)
//		.OnItemScrolledIntoView_Lambda([this](TSharedPtr<FGraphActionNode> InActionNode, const TSharedPtr<ITableRow>& InWidget) {OnItemScrolledIntoView(InActionNode, InWidget); })
//		.OnSetExpansionRecursive_Lambda([this](TSharedPtr<FGraphActionNode> InTreeNode, bool bInIsItemExpanded) { OnSetExpansionRecursive(InTreeNode, bInIsItemExpanded); })
//		.HighlightParentNodesForSelection(true)
//		.OnExpansionChanged(InArgs._OnExpansionChanged);
//
//
//	this->ChildSlot
//		[
//			SNew(SVerticalBox)
//
//			// FILTER BOX
//		+ SVerticalBox::Slot()
//		.AutoHeight()
//		[
//			SAssignNew(FilterTextBox, SSearchBox)
//			// If there is an external filter delegate, do not display this filter box
//		.Visibility(InArgs._OnGetFilterText.IsBound() ? EVisibility::Collapsed : EVisibility::Visible)
//		.OnTextChanged_Lambda([this](const FText& InFilterText) { OnFilterTextChanged(InFilterText); })
//		.OnTextCommitted_Lambda([this](const FText& InText, ETextCommit::Type CommitInfo) { OnFilterTextCommitted(InText, CommitInfo); })
//		]
//
//	// ACTION LIST
//	+ SVerticalBox::Slot()
//		.Padding(FMargin(0.0f, 2.0f, 0.0f, 0.0f))
//		.FillHeight(1.f)
//		[
//			SNew(SScrollBorder, TreeView.ToSharedRef())
//			[
//				TreeView.ToSharedRef()
//			]
//		]
//		];
//
//	// When the search box has focus, we want first chance handling of any key down events so we can handle the up/down and escape keys the way we want
//	FilterTextBox->SetOnKeyDownHandler(FOnKeyDown::CreateSP(this, &SGraphActionMenu::OnKeyDown));
//
//	if (!InArgs._ShowFilterTextBox)
//	{
//		FilterTextBox->SetVisibility(EVisibility::Collapsed);
//	}
//
//	// Get all actions.
//	RefreshAllActions(false);
//}
