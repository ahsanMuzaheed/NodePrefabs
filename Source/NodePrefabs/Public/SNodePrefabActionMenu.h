#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
TODO remove if not needed anymore
#endif


//// Copyright 2019 Guenther Schrenk (Rumbleball)
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "SGraphActionMenu.h"
//
///**
// * As we need to access a dispatcher of STreeView that can only be set via the 
// * constructor arguments, we inherit from SGraphActionMenu, redirect everything
// * and create the widget tree again.
// */
//class NODEPREFABS_API SNodePrefabActionMenu : public SGraphActionMenu
//{
//public:
//	typedef  TSharedPtr<FGraphActionNode> TreeType;
//
//	// Need to copy the SGraphActionMenu arguments. These are passed to parent in Construct
//	SLATE_BEGIN_ARGS(SNodePrefabActionMenu)
//		: _AutoExpandActionMenu(false)
//		, _AlphaSortItems(true)
//		, _ShowFilterTextBox(true)
//		, _UseSectionStyling(false)
//	{ }
//
//	SLATE_EVENT(FOnActionSelected, OnActionSelected)
//		SLATE_EVENT(FOnActionDoubleClicked, OnActionDoubleClicked)
//		SLATE_EVENT(FOnActionDragged, OnActionDragged)
//		SLATE_EVENT(FOnCategoryDragged, OnCategoryDragged)
//		SLATE_EVENT(FOnContextMenuOpening, OnContextMenuOpening)
//		SLATE_EVENT(FOnCreateWidgetForAction, OnCreateWidgetForAction)
//		SLATE_EVENT(FOnCreateCustomRowExpander, OnCreateCustomRowExpander)
//		SLATE_EVENT(FOnCollectAllActions, OnCollectAllActions)
//		SLATE_EVENT(FOnCollectStaticSections, OnCollectStaticSections)
//		SLATE_EVENT(FOnCategoryTextCommitted, OnCategoryTextCommitted)
//		SLATE_EVENT(FCanRenameSelectedAction, OnCanRenameSelectedAction)
//		SLATE_EVENT(FGetSectionTitle, OnGetSectionTitle)
//		SLATE_EVENT(FGetSectionToolTip, OnGetSectionToolTip)
//		SLATE_EVENT(FGetSectionWidget, OnGetSectionWidget)
//		SLATE_EVENT(FGetFilterText, OnGetFilterText)
//		SLATE_EVENT(FOnActionMatchesName, OnActionMatchesName)
//		SLATE_EVENT(STreeView<TreeType>::FOnExpansionChanged, OnExpansionChanged)
//		SLATE_ARGUMENT(bool, AutoExpandActionMenu)
//		SLATE_ARGUMENT(bool, AlphaSortItems)
//		SLATE_ARGUMENT(bool, ShowFilterTextBox)
//		SLATE_ARGUMENT(bool, UseSectionStyling)
//		SLATE_END_ARGS()
//
//		void Construct(const FArguments& InArgs, const bool bIsReadOnly = true);
//
//protected:
//	void OnTreeViewExpansionChanged(TreeType item, bool bExpanded);
//
//	TArray<TSharedPtr<FGraphActionNode>> itemSource;
//
//};
