// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "NodePrefabSettings.h"

FName UNodePrefabSettings::settingsContainer = FName("Editor");
FName UNodePrefabSettings::settingsCategory = FName("Plugins");
FName UNodePrefabSettings::settingsSection = FName("NodePrefabs");

UNodePrefabSettings::UNodePrefabSettings()
{
	// Per default include all Prefabs
	FDirectoryPath base;
	base.Path = FString(TEXT("/Game"));
	PathToIncludeRecursive.Add(base);
}

UNodePrefabSettings::~UNodePrefabSettings()
{
}
