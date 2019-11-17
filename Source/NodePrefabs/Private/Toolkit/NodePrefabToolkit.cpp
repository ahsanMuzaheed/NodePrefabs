// Copyright 2019 Guenther Schrenk (Rumbleball)

#include "NodePrefabToolkit.h"
#include "CoreMinimal.h"
#include "NodePrefab.h"
#include "Toolkit/SNodePrefabToolkitWidget.h"
#include "SDockTab.h"
#include "EditorStyleSet.h"


#define LOCTEXT_NAMESPACE "NodePrefabToolkit"

const FName FNodePrefabsToolkit::toolkitIdentifier = FName("NodePrefabToolkit");
const FName FNodePrefabsToolkit::toolkitTabId = FName("BNPWidgetTab");

FNodePrefabsToolkit::FNodePrefabsToolkit(UNodePrefab* prefab)
	: prefabToEdit(prefab)
{
	check(prefabToEdit);
	// Support undo/redo
	prefabToEdit->SetFlags(RF_Transactional);
	GEditor->RegisterForUndo(this);
}

FNodePrefabsToolkit::~FNodePrefabsToolkit()
{
	prefabToEdit->ClearFlags(RF_Transactional);
	GEditor->UnregisterForUndo(this);
}


void FNodePrefabsToolkit::Initialize(const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	// create tab layout
	// Not sure what this actually does. Seems not to have any effekt.
	const TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("Standalone_BNPToolkit")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.66f)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.1f)
				)
				->Split
				(
					FTabManager::NewStack()
					->AddTab(toolkitTabId, ETabState::OpenedTab)
					->SetHideTabWell(true)
					->SetSizeCoefficient(0.9f)
				)
			)
		);

	FAssetEditorToolkit::InitAssetEditor(
		EToolkitMode::Standalone
		, InitToolkitHost
		, toolkitIdentifier
		, layout
		, true //bCreateDefaultStandaloneMenu
		, true //bCreateDefaultToolbar
		, prefabToEdit
	);
}

void FNodePrefabsToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_NodePrefabEditor", "Node Prefab Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(toolkitTabId, FOnSpawnTab::CreateSP(this, &FNodePrefabsToolkit::HandleTabManagerSpawnTab))
		.SetDisplayName(LOCTEXT("TextEditorTabName", "Node Prefab Editor"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));
}


void FNodePrefabsToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(toolkitTabId);
}

TSharedRef<FNodePrefabsToolkit> FNodePrefabsToolkit::Create(UNodePrefab* prefab, const TSharedPtr< class IToolkitHost >& InitToolkitHost)
{
	TSharedRef< FNodePrefabsToolkit> newToolkit = MakeShareable(new FNodePrefabsToolkit(prefab));
	newToolkit->Initialize(InitToolkitHost);
	return newToolkit;
}

FText FNodePrefabsToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Node Prefab Editor");
}

FName FNodePrefabsToolkit::GetToolkitFName() const
{
	return FName("NodePrefabEditor");
}

FString FNodePrefabsToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "NodePrefab").ToString();
}

FLinearColor FNodePrefabsToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

void FNodePrefabsToolkit::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(prefabToEdit);
}

TSharedRef<SDockTab> FNodePrefabsToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args)
{
	TSharedPtr<SWidget> TabWidget = SNew(SNodePrefabsToolkitWidget, prefabToEdit);
	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}

#undef LOCTEXT_NAMESPACE
