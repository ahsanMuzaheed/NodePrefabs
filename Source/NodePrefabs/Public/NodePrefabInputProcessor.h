// Copyright 2019 Guenther Schrenk (Rumbleball)

#pragma once

#include "CoreMinimal.h"
#include "Framework\Application\IInputProcessor.h"

/**
 * Input handler for NodePrefabs
 */
class NODEPREFABS_API FNodePrefabInputProcessor : public IInputProcessor
{
public:


	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		// do nothing
		return; 
	}

	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent);
};
