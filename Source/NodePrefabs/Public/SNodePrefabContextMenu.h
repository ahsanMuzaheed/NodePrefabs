// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Layout/SBorder.h"
#include "SGraphActionMenu.h"

struct FCreateWidgetForActionData;
class SNodePrefabActionMenu;

/**
 * Context menu for Node Prefabs that is popped up in the GraphEditor (shift+rightclick)
 */
class NODEPREFABS_API SNodePrefabContextMenu : public SBorder
{
public:
	SLATE_BEGIN_ARGS(SNodePrefabContextMenu)
	{}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, FPointerEvent inMouseEvent, TWeakPtr<SGraphEditor> inGraphEditor);

	TSharedPtr<SGraphActionMenu> GetActionMenu()
	{
		return graphActionMenu;
	}

protected:
	FReply OnPrefabButtonClicked(UNodePrefab*);

	void OnGraphActionMenu_CollectAllActions(FGraphActionListBuilderBase& OutAllActions);
	TSharedRef<SWidget> OnGraphActionMenu_CreateWidgetForAction(FCreateWidgetForActionData* const InCreateData);
	void OnGraphActionMenu_OnActionSelected(const TArray<TSharedPtr<FEdGraphSchemaAction>>& SelectedAction, ESelectInfo::Type InSelectionType);
#if ENGINE_MAJOR_VERSION >= 4 && ENGINE_MINOR_VERSION >= 25
	void OnGraphActionMenu_ExpansionChanged(SNodePrefabActionMenu::TreeType item, bool bExpanded);
	void OnGraphActionMenu_OnEntryInitialized(SNodePrefabActionMenu::TreeType item, bool bExpanded);
#endif

private:
	void SpawnPrefab(const FAssetData& assetData);


	TSharedPtr<SGraphActionMenu> graphActionMenu;

	TSharedPtr<SVerticalBox> verticalBox;
	FPointerEvent mouseEvent;
	TWeakPtr<SGraphEditor> graphEditor;
};
