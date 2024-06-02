// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonActivatableWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "TitleScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class DXCOREUI_API UTitleScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

protected:
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	/** The screen created after title screen close */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UCommonActivatableWidget> MainMenuWidgetClass;

	/** The input that trigger the close of this screen */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (RowType = CommonInputActionDataBase))
	FDataTableRowHandle StartInputAction;

	/** The full screen image to show in title menu, or a just small logo */
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> LogoTexture;

	/** Image component to handle the LogoTexture */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	UImage* LogoImage;

	/** Anim to close the TitleScreen */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* OpenAnim;

	/** The button that handle the click logic to close this screen */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonButtonBase* Start_Button;

private:
	UFUNCTION()
	void OpenMainScreen();
	
	UPROPERTY()
	FWidgetAnimationDynamicEvent _onOpenAnimFinish;
};
