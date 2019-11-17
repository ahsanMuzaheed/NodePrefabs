// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "CoreMinimal.h"
#include "EditorUndoClient.h"

class UNodePrefab;

/**
 * 
 */
class NODEPREFABS_API FNodePrefabsToolkit
	: public FAssetEditorToolkit
	, public FEditorUndoClient
	, public FGCObject
{
public:
	~FNodePrefabsToolkit();

	static const FName toolkitIdentifier;
	static const FName toolkitTabId;

	// We need an Init function to init Super classes that make use form SharedFromThis
	// as SharedFromThis is not usable in Constructor.
	void Initialize(const TSharedPtr< class IToolkitHost >& InitToolkitHost);


	static TSharedRef<FNodePrefabsToolkit> Create(UNodePrefab* prefab, const TSharedPtr< class IToolkitHost >& InitToolkitHost);

	//~ IToolkit interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	//~ IToolkit interface

	//~ FGCObject interface
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	//~ FGCObject interface

private:
	FNodePrefabsToolkit(UNodePrefab* prefab);

	/** Callback for spawning the our editor widget. */
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& Args);

	UNodePrefab* prefabToEdit;

};
