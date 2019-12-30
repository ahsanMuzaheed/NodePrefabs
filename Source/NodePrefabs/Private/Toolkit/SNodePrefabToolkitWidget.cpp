// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "SNodePrefabToolkitWidget.h"
#include "NodePrefab.h"
#include "SMultiLineEditableTextBox.h"
#include "Private/SDetailsView.h"
#include "ModuleManager.h"
#include "PropertyEditorModule.h"

SNodePrefabsToolkitWidget::~SNodePrefabsToolkitWidget()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}


void SNodePrefabsToolkitWidget::Construct(const FArguments& inArgs, UNodePrefab* inPrefab)
{
	prefab = inPrefab;

	//detailsView->SetObject(inPrefab);
	FPropertyEditorModule& propertyModule = FModuleManager::Get().LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs detailsViewArgs;
	detailsViewArgs.bAllowSearch = false;
	detailsViewArgs.bAllowFavoriteSystem = false;
	detailsViewArgs.bHideSelectionTip = true;
	detailsViewArgs.bUpdatesFromSelection = false;

	TSharedRef<IDetailsView> detailsView = propertyModule.CreateDetailView(detailsViewArgs);
	detailsView->SetObject(prefab);

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			detailsView
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.f, 10.f, 0.f, 0.f)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("NodePrefabs", "ToolkitWidget.ClipboardText", "Node construct clipboard data"))
		]
	+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(editableTextBox, SMultiLineEditableTextBox)
			.HintText(NSLOCTEXT("NodePrefabs", "ToolkitWidget.ClipboardHint", "Paste the clipboard data of your node construct here"))
			.OnTextChanged(this, &SNodePrefabsToolkitWidget::HandleEditableTextBoxTextChanged)
		.OnTextCommitted(this, &SNodePrefabsToolkitWidget::HandleEditableTextBoxTextCommitted)
		.Text(FText::FromString(prefab->prefab))
		]
		];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SNodePrefabsToolkitWidget::HandleTextAssetPropertyChanged);
}


/* STextAssetEditor callbacks
 *****************************************************************************/

void SNodePrefabsToolkitWidget::HandleEditableTextBoxTextChanged(const FText& NewText)
{
	prefab->MarkPackageDirty();
}


void SNodePrefabsToolkitWidget::HandleEditableTextBoxTextCommitted(const FText& Comment, ETextCommit::Type CommitType)
{
	prefab->prefab = editableTextBox->GetText().ToString();
}


void SNodePrefabsToolkitWidget::HandleTextAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == prefab)
	{
		editableTextBox->SetText(FText::FromString(prefab->prefab));
	}
}
