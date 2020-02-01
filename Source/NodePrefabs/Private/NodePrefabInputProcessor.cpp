// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "NodePrefabInputProcessor.h"
#include "Input/Events.h"
#include "NodePrefabLibrary.h"
#include "Framework/Application/SlateApplication.h"
#include "SNodePrefabContextMenu.h"
#include "NodePrefabActions.h"
#include "Framework/Commands/UICommandList.h"
#include "SGraphPanel.h"
#include "SNodePrefabActionMenu.h"

bool FNodePrefabInputProcessor::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	// Shift+RightMouse
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton
		&& MouseEvent.IsShiftDown()
		&& !MouseEvent.IsControlDown()
		&& !MouseEvent.IsAltDown()
		&& !MouseEvent.IsCommandDown()
		)
	{
		// Check if the mouse is above a SNodePanel
		TSharedPtr<SGraphEditor> graphEditor = FNodePrefabLibrary::GetGraphEditorUnderMouse(MouseEvent.GetScreenSpacePosition());

		if (!graphEditor.IsValid())
		{
			return false;
		}
		TSharedPtr<SWidget> graphPanelUncasted = FNodePrefabLibrary::FindChildWidgetRecursive(StaticCastSharedPtr<SCompoundWidget>(graphEditor)->GetChildren(), FName("SGraphPanel"));
		if (!graphPanelUncasted.IsValid())
		{
			return false;
		}
		TSharedPtr<SGraphPanel> graphPanel = StaticCastSharedPtr<SGraphPanel>(graphPanelUncasted);

		TSharedPtr<SNodePrefabContextMenu> widget;
		SAssignNew(widget, SNodePrefabContextMenu, MouseEvent, graphEditor);

		//SGraphPanel::SummonContextMenu
		TSharedPtr<IMenu> Menu = FSlateApplication::Get().PushMenu(
			graphPanel.ToSharedRef(),
			FWidgetPath(),
			widget.ToSharedRef(),
			MouseEvent.GetScreenSpacePosition(),
			FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
		);

		FSlateApplication::Get().SetUserFocus(0, widget->GetActionMenu()->GetFilterTextBox());

		return true;
	}

	return false;
}
