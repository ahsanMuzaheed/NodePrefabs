// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

class FText;
class ISlateStyle;
class UTextAsset;
class UNodePrefab;

/**
 *
 */
class NODEPREFABS_API SNodePrefabsToolkitWidget : public SCompoundWidget, public FGCObject
{
public:

	SLATE_BEGIN_ARGS(SNodePrefabsToolkitWidget) { }
	SLATE_END_ARGS()

public:
	virtual ~SNodePrefabsToolkitWidget();

	/**
	 * Construct this widget
	 *
	 * @param inArgs The declaration data for this widget.
	 * @param inPrefab The UNodePrefab asset to edit.
	 */
	void Construct(const FArguments& inArgs, UNodePrefab* inPrefab);

private:

	/** Callback for text changes in the editable text box. */
	void HandleEditableTextBoxTextChanged(const FText& NewText);

	/** Callback for committed text in the editable text box. */
	void HandleEditableTextBoxTextCommitted(const FText& Comment, ETextCommit::Type CommitType);

	/** Callback for property changes in the text asset. */
	void HandleTextAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent);

	//~ FGCObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		Collector.AddReferencedObject(prefab);
	}
	//~ FGCObject interface

private:

	/** Holds the editable text box widget. */
	TSharedPtr<SMultiLineEditableTextBox> editableTextBox;

	/** Pointer to the Prefab that is being edited. */
	UNodePrefab* prefab;
};
