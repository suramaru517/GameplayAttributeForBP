// Copyright Metaseven All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class GAMEPLAYATTRIBUTEFORBP_API FGameplayAttributeForBPModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

private:
	static void RunEditorUtilityBlueprints();
};
