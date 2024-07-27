// Copyright 2023-2024 Metaseven. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGameplayAttributeForBPModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

private:
	static void RunEditorUtilityBlueprints();
};
