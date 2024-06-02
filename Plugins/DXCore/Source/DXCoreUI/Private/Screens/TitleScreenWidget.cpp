// Fill out your copyright notice in the Description page of Project Settings.

#include "Screens/TitleScreenWidget.h"

void UTitleScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_onOpenAnimFinish.BindDynamic(this, &UTitleScreenWidget::OpenMainScreen);
	
	BindToAnimationFinished(OpenAnim, _onOpenAnimFinish);
	Start_Button->OnClicked().AddLambda([this]()
		{
			PlayAnimation(OpenAnim, 0.0, 1, EUMGSequencePlayMode::Forward, 1.0, false);
		});
}

void UTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (LogoImage)
		LogoImage->SetBrushFromSoftTexture(LogoTexture);
	
	if (Start_Button)
		Start_Button->SetTriggeredInputAction(StartInputAction);
}

void UTitleScreenWidget::OpenMainScreen()
{
	UCommonActivatableWidget* MainMenuWidget = CreateWidget<UCommonActivatableWidget>(this, MainMenuWidgetClass.Get());

	MainMenuWidget->AddToViewport();
	MainMenuWidget->ActivateWidget();

	RemoveFromParent();
}

UWidget* UTitleScreenWidget::NativeGetDesiredFocusTarget() const
{
	return Start_Button;
}