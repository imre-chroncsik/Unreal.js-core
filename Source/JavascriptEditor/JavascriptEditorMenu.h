#pragma once

#include "JavascriptEditorLibrary.h"
#include "JavascriptEditorMenu.generated.h"


/**
 * 
 */
UCLASS()
class JAVASCRIPTEDITOR_API UJavascriptEditorMenu : public UWidget
{
	GENERATED_UCLASS_BODY()

public:	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FOnHook, FName, Hook);

#if WITH_EDITOR
	UPROPERTY()
	FJavascriptUICommandList CommandList;

	UPROPERTY()
	FOnHook OnHook;

	UFUNCTION(BlueprintCallable, Category = "Javascript | Editor")
	static void AddPullDownMenu(const FName& Id, const FText& MenuLabel, const FText& Tooltip);
	
	void Setup(TSharedRef<SBox> VerticalBox);
	
	virtual TSharedRef<SWidget> RebuildWidget();
#endif
};
