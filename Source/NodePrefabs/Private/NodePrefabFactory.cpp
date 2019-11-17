// Copyright 2019 Guenther Schrenk (Rumbleball)


#include "NodePrefabFactory.h"
#include "NodePrefab.h"

UObject* UNodePrefabFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UNodePrefab* prefab = NewObject<UNodePrefab>(InParent, Class, Name, Flags);

	return prefab;
}
