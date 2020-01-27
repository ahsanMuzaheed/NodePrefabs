// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBorder.h"
//#include "StrongObjectPtr.h"

class SScrollBox;


/**
 * Context menu for Node Prefabs that is popped up in the GraphEditor (shift+rightclick)
 */
class NODEPREFABS_API SNodePrefabContextMenu : public SBorder, public FGCObject
{
	// Node to create a category-tree from for our prefabs.
	// A node can either have a category or a prefab
	struct FTreeNode : public TSharedFromThis<FTreeNode>
	{
		FTreeNode(TSharedPtr<FTreeNode> inParent, const FString& inCategoryPart)
			: categoryPart(inCategoryPart), prefab(nullptr), parent(inParent)
		{}

		FTreeNode(TSharedPtr<FTreeNode> inParent, UNodePrefab* inPrefab)
			: prefab(inPrefab), parent(inParent)
		{}

		// Either the categories are set
		const FString categoryPart; // Last category part of "category"
		// or the prefab
		UNodePrefab* prefab;

		TSharedPtr<FTreeNode> parent;
		TArray<TSharedPtr<FTreeNode>> children;

		void AddPrefab_RootOnly(UNodePrefab* inPrefab);
		void SortTree_RootOnly();

		/**
		 * Read through the parent chain to build the full category with '|' separator
		 * up to and including this node.
		 */
		FString GetCategoryFull();
	};

	typedef TSharedPtr<FTreeNode> TreeType;

public:
	SLATE_BEGIN_ARGS(SNodePrefabContextMenu)
	{}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, FPointerEvent inMouseEvent, TWeakPtr<SGraphEditor> inGraphEditor);

	//~ Begin FGCObject Interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector);
	virtual FString GetReferencerName() const override;
	//~ End FGCObject Interface

	TSharedRef<ITableRow> OnTreeViewGenerateRow(TreeType item, const TSharedRef<STableViewBase>& OwnerTable);
	void OnTreeViewGetChildren(TreeType inItem, TArray<TreeType>& childItems)
	{
		childItems = inItem->children;
	}
	void OnTreeViewExpansionChanged(TreeType item, bool bExpanded);

protected:
	FReply OnPrefabButtonClicked(UNodePrefab*);

private:
	void SpawnPrefab(const FAssetData& assetData);

	TSharedPtr<FTreeNode> treeRoot;

	// Filtered list of NodePrefabs that are displayed by this widget
	TArray<UNodePrefab*> nodePrefabsFiltered;
	TSharedPtr<SVerticalBox> verticalBox;
	FPointerEvent mouseEvent;
	TWeakPtr<SGraphEditor> graphEditor;
};
