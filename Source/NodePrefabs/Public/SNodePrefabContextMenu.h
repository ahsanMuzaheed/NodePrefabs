// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBorder.h"
#include "StrongObjectPtr.h"

class SScrollBox;

/**
 * Context menu for Node Prefabs that is popped up in the GraphEditor (shift+rightclick)
 */
class NODEPREFABS_API SNodePrefabContextMenu : public SBorder, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SNodePrefabContextMenu)
	{}
		SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, FPointerEvent inMouseEvent, TWeakPtr<SGraphEditorImpl> inGraphEditor);

	//~ Begin FGCObject Interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector);
	virtual FString GetReferencerName() const override;
	//~ End FGCObject Interface

	TSharedRef<ITableRow> OnGenerateListRow(UNodePrefab* item, const TSharedRef<STableViewBase>& OwnerTable);

protected:
	FReply OnPrefabButtonClicked(UNodePrefab*);

private:
	void SpawnPrefab(const FAssetData& assetData);

	// Filtered list of NodePrefabs that are displayed by this widget
	TArray<UNodePrefab*> nodePrefabsFiltered;
	TSharedPtr<SScrollBox> scrollBox;
	FPointerEvent mouseEvent;
	TWeakPtr<SGraphEditorImpl> graphEditor;
};
