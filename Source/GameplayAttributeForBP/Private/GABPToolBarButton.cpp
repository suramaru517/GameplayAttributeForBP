// Copyright Metaseven All Rights Reserved.

#include "GABPToolBarButton.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "AttributeSet.h"

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

// GABPボタンが押された時
void UGABPToolBarButton::ToggleActivation()
{
	// FARFilter を設定
	FARFilter ARFilter;
	ARFilter.PackagePaths.Add(FName(TEXT("/GameplayAttributeForBP/AttributeSets")));
	ARFilter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	ARFilter.bRecursivePaths = true;
	ARFilter.bRecursiveClasses = true;
	
	// BPをロード
	TArray<FAssetData> Assets;
	IAssetRegistry::Get()->GetAssets(ARFilter, Assets);

	TArray<TSubclassOf<UObject>> AttributeSetClasses;
	
	// UAttributeSet の子BPクラスを収集
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

// GABPボタンがオンになった時
void UGABPToolBarButton::Activate(const TArray<TSubclassOf<UObject>>& AttributeSetClasses)
{
	// C++クラスに偽装
	for (const TSubclassOf<UObject>& AttributeSetClass : AttributeSetClasses)
	{
		ClassGeneratedBys.Emplace(AttributeSetClass, AttributeSetClass->ClassGeneratedBy);
		AttributeSetClass->ClassGeneratedBy = nullptr;
	}

	bIsActivated = true;
	Data.Icon = ActiveIcon;
}

// GABPボタンがオフになった時
void UGABPToolBarButton::Deactivate(const TArray<TSubclassOf<UObject>>& AttributeSetClasses)
{
	// C++クラスの偽装を解除
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
