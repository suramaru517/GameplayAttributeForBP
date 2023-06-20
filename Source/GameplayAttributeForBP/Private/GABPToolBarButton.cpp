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

// GABP�{�^���������ꂽ��
void UGABPToolBarButton::ToggleActivation()
{
	// FARFilter ��ݒ�
	FARFilter ARFilter;
	ARFilter.PackagePaths.Add(FName(TEXT("/GameplayAttributeForBP/AttributeSets")));
	ARFilter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	ARFilter.bRecursivePaths = true;
	ARFilter.bRecursiveClasses = true;
	
	// BP�����[�h
	TArray<FAssetData> Assets;
	IAssetRegistry::Get()->GetAssets(ARFilter, Assets);

	TArray<TSubclassOf<UObject>> AttributeSetClasses;
	
	// UAttributeSet �̎qBP�N���X�����W
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

// GABP�{�^�����I���ɂȂ�����
void UGABPToolBarButton::Activate(const TArray<TSubclassOf<UObject>>& AttributeSetClasses)
{
	// C++�N���X�ɋU��
	for (const TSubclassOf<UObject>& AttributeSetClass : AttributeSetClasses)
	{
		ClassGeneratedBys.Emplace(AttributeSetClass, AttributeSetClass->ClassGeneratedBy);
		AttributeSetClass->ClassGeneratedBy = nullptr;
	}

	bIsActivated = true;
	Data.Icon = ActiveIcon;
}

// GABP�{�^�����I�t�ɂȂ�����
void UGABPToolBarButton::Deactivate(const TArray<TSubclassOf<UObject>>& AttributeSetClasses)
{
	// C++�N���X�̋U��������
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
