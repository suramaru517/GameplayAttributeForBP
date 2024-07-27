// Copyright 2023-2024 Metaseven. All Rights Reserved.

#include "GABPToolBarButton.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AttributeSet.h"
#include "Engine/Blueprint.h"

UGABPToolBarButton::UGABPToolBarButton()
{
	Data.Menu = FName(TEXT("AssetEditorToolbar.CommonActions"));
	Data.Section = FName(TEXT("GABP"));
	Data.Name = FName(TEXT("GABP"));
	Data.Label = FText::FromString(FString(TEXT("GABP")));
	Data.ToolTip = FText::FromString(FString(TEXT("GABP")));
	Data.Icon = InactiveIcon;
	Data.Advanced.EntryType = EMultiBlockType::ToolBarButton;
	Data.Advanced.UserInterfaceActionType = EUserInterfaceActionType::ToggleButton;
	Data.Advanced.StyleNameOverride = FName(TEXT("CalloutToolbar"));
}

ECheckBoxState UGABPToolBarButton::GetCheckState_Implementation(const FToolMenuContext& Context) const
{
	return bIsActivated ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

// When GABP button is pressed
void UGABPToolBarButton::ToggleActivation()
{
	// Set ARFilter
	FARFilter ARFilter;
	ARFilter.PackagePaths.Add(FName(TEXT("/GameplayAttributeForBP/AttributeSets")));
	ARFilter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	ARFilter.bRecursivePaths = true;
	ARFilter.bRecursiveClasses = true;

	// Load Blueprints
	TArray<FAssetData> Assets;
	IAssetRegistry::Get()->GetAssets(ARFilter, Assets);

	TArray<TSubclassOf<UObject>> AttributeSetClasses;

	// Collect child Blueprint classes of UAttributeSet
	for (const FAssetData& Asset : Assets)
	{
		const UBlueprint* Blueprint = Cast<UBlueprint>(Asset.GetAsset());
		const TSubclassOf<UObject> Class = Blueprint->GeneratedClass;

		if (Class && Class->IsChildOf<UAttributeSet>())
		{
			AttributeSetClasses.Emplace(Class);
		}
	}

	bIsActivated ? Deactivate(AttributeSetClasses) : Activate(AttributeSetClasses);
}

// When GABP button is turned on
void UGABPToolBarButton::Activate(const TArray<TSubclassOf<UObject>>& AttributeSetClasses)
{
	// Disguise as a C++ class
	for (const TSubclassOf<UObject>& AttributeSetClass : AttributeSetClasses)
	{
		ClassGeneratedBys.Emplace(AttributeSetClass, AttributeSetClass->ClassGeneratedBy);
		AttributeSetClass->ClassGeneratedBy = nullptr;
	}

	bIsActivated = true;
	Data.Icon = ActiveIcon;
}

// When GABP button is turned off
void UGABPToolBarButton::Deactivate(const TArray<TSubclassOf<UObject>>& AttributeSetClasses)
{
	// Remove C++ class disguise
	for (const TSubclassOf<UObject>& AttributeSetClass : AttributeSetClasses)
	{
		UObject** ClassGeneratedBy = ClassGeneratedBys.Find(AttributeSetClass);

		if (*ClassGeneratedBy)
		{
			AttributeSetClass->ClassGeneratedBy = *ClassGeneratedBy;
		}
	}

	ClassGeneratedBys.Empty();

	bIsActivated = false;
	Data.Icon = InactiveIcon;
}
