// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "NodePrefabInputProcessor.h"
#include "Events.h"
#include "NodePrefabLibrary.h"
#include "Private/SGraphEditorImpl.h"
#include "SlateApplication.h"
#include "SNodePrefabContextMenu.h"
#include "NodePrefabActions.h"
#include "UICommandList.h"
#include "SGraphPanel.h"

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
		TSharedPtr<SGraphEditorImpl> graphEditor = FNodePrefabLibrary::GetGraphEditorUnderMouse(MouseEvent.GetScreenSpacePosition());
		if (!graphEditor.IsValid())
		{
			return false;
		}
		TSharedPtr<SWidget> graphPanelUncasted = FNodePrefabLibrary::FindChildWidgetRecursive(graphEditor->GetChildren(), FName("SGraphPanel"));
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

		return true;
	}

	return false;
}
