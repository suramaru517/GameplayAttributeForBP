// Copyright 2023-2024 Metaseven. All Rights Reserved.

#include "GameplayAttributeForBP.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Editor.h"
#include "EditorUtilityBlueprint.h"
#include "EditorUtilitySubsystem.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FGameplayAttributeForBPModule"

void FGameplayAttributeForBPModule::StartupModule()
{
	IAssetRegistry::Get()->OnFilesLoaded().AddStatic(&FGameplayAttributeForBPModule::RunEditorUtilityBlueprints);
}

void FGameplayAttributeForBPModule::RunEditorUtilityBlueprints()
{
	FARFilter ARFilter;
	ARFilter.PackagePaths.Add(FName(TEXT("/GameplayAttributeForBP/EditorUtilities")));
	ARFilter.ClassPaths.Add(UEditorUtilityBlueprint::StaticClass()->GetClassPathName());
	ARFilter.bRecursivePaths = true;
	ARFilter.bRecursiveClasses = true;

	TArray<FAssetData> Assets;
	IAssetRegistry::Get()->GetAssets(ARFilter, Assets);

	for (const FAssetData& Asset : Assets)
	{
		UEditorUtilityBlueprint* EditorUtilityBlueprint = Cast<UEditorUtilityBlueprint>(Asset.GetAsset());

		if (GEditor)
		{
			GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->TryRun(EditorUtilityBlueprint);
		}
	}

	UToolMenus::Get()->RefreshAllWidgets();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameplayAttributeForBPModule, GameplayAttributeForBP)
