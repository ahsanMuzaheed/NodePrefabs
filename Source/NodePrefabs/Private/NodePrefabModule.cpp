// Copyright 2019 Guenther Schrenk (Rumbleball)

#include "NodePrefabModule.h"
#include "Modules/ModuleManager.h"
#include "NodePrefabActions.h"
#include "GenericPlatform/GenericApplication.h"
#include "Windows/WindowsApplication.h"
#include "Framework/Application/SlateApplication.h"
#include "NodePrefabInputProcessor.h"
#include "ISettingsModule.h"
#include "NodePrefabSettings.h"

#define LOCTEXT_NAMESPACE "FNodePrefabsModule"


class FNodePrefabsModule :
	public IModuleInterface
{
public:

	//~ IModuleInterface interface
	virtual void StartupModule() override
	{
		RegisterEditorSettings();
		RegisterAssetTools();
		RegisterInputHandling();
	}


	virtual void ShutdownModule() override
	{
		UnregisterEditorSettings();
		UnregisterAssetTools();
		UnregisterInputHandling();
	}

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
	//~ IModuleInterface interface

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
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().RegisterInputPreProcessor(inputProcessor);
		}
	}

	void UnregisterInputHandling()
	{
		if (FSlateApplication::IsInitialized())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(inputProcessor);
		}
	}

	void RegisterEditorSettings()
	{
		// Registering some settings is just a matter of exposing the default UObject of
		// your desired class, feel free to add here all those settings you want to expose
		// to your LDs or artists.

		if (ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			UNodePrefabSettings* settings = UNodePrefabSettings::Get();

			// Register the settings
			settingsModule->RegisterSettings(settings->GetContainerName(), settings->GetCategoryName(), settings->GetSectionName(),
				NSLOCTEXT("NodePrefabs", "NodePrefab.Settings.Name", "NodePrefabs"),
				NSLOCTEXT("NodePrefabs", "NodePrefab.Settings.Description", "Settings for NodePrefab Assets and the corresponding Graph Menu"),
				settings
			);
		}
	}

	void UnregisterEditorSettings()
	{
		if (ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
		{
			settingsModule->UnregisterSettings(UNodePrefabSettings::GetContainerName(), UNodePrefabSettings::GetCategoryName(), UNodePrefabSettings::GetSectionName());
		}
	}


private:
	/** The collection of registered asset type actions. */
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;

	TSharedPtr<FNodePrefabInputProcessor> inputProcessor;
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNodePrefabsModule, NodePrefabs)
