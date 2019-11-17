// Copyright 2019 Guenther Schrenk (Rumbleball)

#include "NodePrefabModule.h"
#include "Modules/ModuleManager.h"
#include "NodePrefabActions.h"
#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsApplication.h"
#include "Framework/Application/SlateApplication.h"
#include "NodePrefabInputProcessor.h"

#define LOCTEXT_NAMESPACE "FNodePrefabsModule"


class FNodePrefabsModule : 
	public IModuleInterface
	, public IHasMenuExtensibility
	, public IHasToolBarExtensibility
{
public:

	//~ IModuleInterface interface
	virtual void StartupModule() override
	{
		RegisterAssetTools();
		RegisterMenuExtensions();
		RegisterInputHandling();

	}


	virtual void ShutdownModule() override
	{
		UnregisterAssetTools();
		UnregisterMenuExtensions();
		unregisterInputHandling();
	}	

	//virtual bool SupportsDynamicReloading() override
	//{
	//	return true;
	//}
	//~ IModuleInterface interface
	
	//~ IHasMenuExtensibility interface
	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override
	{
		return MenuExtensibilityManager;
	}
	//~ IHasMenuExtensibility interface

	//~ IHasToolBarExtensibility interface
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override
	{
		return ToolBarExtensibilityManager;
	}
	//~ IHasToolBarExtensibility interface

protected:

	/** Registers asset tool actions. */
	void RegisterAssetTools()
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		RegisterAssetTypeAction(AssetTools, MakeShareable(new FNodePrefabActions()));
	}

	/**
	 * Registers a single asset type action.
	 *
	 * @param AssetTools The asset tools object to register with.
	 * @param Action The asset type action to register.
	 */
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		RegisteredAssetTypeActions.Add(Action);
	}

	/** Unregisters asset tool actions. */
	void UnregisterAssetTools()
	{
		FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");

		if (AssetToolsModule != nullptr)
		{
			IAssetTools& AssetTools = AssetToolsModule->Get();

			for (auto Action : RegisteredAssetTypeActions)
			{
				AssetTools.UnregisterAssetTypeActions(Action);
			}
		}
	}

	void RegisterInputHandling()
	{
		inputProcessor = MakeShareable(new FNodePrefabInputProcessor());
		FSlateApplication::Get().RegisterInputPreProcessor(inputProcessor);
	}

	void unregisterInputHandling()
	{
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(inputProcessor);
		}
	}

	

	/** Registers main menu and tool bar menu extensions. */
	void RegisterMenuExtensions()
	{
		MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
		ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
	}

	/** Unregisters main menu and tool bar menu extensions. */
	void UnregisterMenuExtensions()
	{
		MenuExtensibilityManager.Reset();
		ToolBarExtensibilityManager.Reset();
	}

private:
	/** The collection of registered asset type actions. */
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;

	// Note: These seem to be needed to populate the ToolkitWindows with content
	// when opened. No clue why.
	/** Holds the tool bar extensibility manager. */
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
	/** Holds the menu extensibility manager. */
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;

	TSharedPtr<FNodePrefabInputProcessor> inputProcessor;
};

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNodePrefabsModule, NodePrefabs)