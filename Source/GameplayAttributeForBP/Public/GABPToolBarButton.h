// Copyright 2023-2024 Metaseven. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityToolMenu.h"
#include "Templates/SubclassOf.h"
#include "GABPToolBarButton.generated.h"

UCLASS()
class GAMEPLAYATTRIBUTEFORBP_API UGABPToolBarButton : public UEditorUtilityToolMenuEntry
{
	GENERATED_BODY()

	UGABPToolBarButton();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GABP")
	void Run();

	virtual ECheckBoxState GetCheckState_Implementation(const FToolMenuContext& Context) const override;

protected:
	UFUNCTION(BlueprintCallable, Category = "GABP")
	void ToggleActivation();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GABP")
	bool bIsActivated = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GABP")
	FScriptSlateIcon ActiveIcon = FScriptSlateIcon(FName(TEXT("NiagaraEditorStyle")), FName(TEXT("NiagaraEditor.Overview.DebugActive")), FName(TEXT("NiagaraEditor.Overview.DebugActive")));

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GABP")
	FScriptSlateIcon InactiveIcon = FScriptSlateIcon(FName(TEXT("NiagaraEditorStyle")), FName(TEXT("NiagaraEditor.Overview.DebugInactive")), FName(TEXT("NiagaraEditor.Overview.DebugInactive")));

private:
	void Activate(const TArray<TSubclassOf<UObject>>& AttributeSetClasses);

	void Deactivate(const TArray<TSubclassOf<UObject>>& AttributeSetClasses);

	UPROPERTY()
	TMap<TSubclassOf<UObject>, UObject*> ClassGeneratedBys;
};
