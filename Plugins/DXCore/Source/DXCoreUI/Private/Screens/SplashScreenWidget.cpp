// Fill out your copyright notice in the Description page of Project Settings.


#include "Screens/SplashScreenWidget.h"

void USplashScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	StartSplashShow();
}

void USplashScreenWidget::StartSplashShow()
{
	if (_playing) return;

	_onImageEnter.BindDynamic(this, &USplashScreenWidget::FadeFinished);
	_onImageExit.BindDynamic(this, &USplashScreenWidget::IdleFinished);
	
	BindToAnimationFinished(Fade, _onImageEnter);
	BindToAnimationFinished(Idle, _onImageExit);

	_playing = true;

	ChangeImage(0);
}

void USplashScreenWidget::FadeFinished()
{
	if (_currentIndex > 0)
	{
		ChangeImage(_currentIndex);
	}
	else
	{
		PlayAnimation(Idle, 0.0, 1, EUMGSequencePlayMode::Forward, (Idle->GetEndTime() - Idle->GetStartTime()) / IdleAnimationDuration, false);
		
		_currentIndex = -_currentIndex;	
	}
}

void USplashScreenWidget::IdleFinished()
{
	PlayAnimation(Fade, 0.0, 1, EUMGSequencePlayMode::Reverse, (Fade->GetEndTime() - Fade->GetStartTime()) / FadeAnimationDuration, false);
}

void USplashScreenWidget::ChangeImage(int Index)
{
	if (Index >= SplashImages.Num())
	{
		_playing = false;
		
		OnFinishSplash();
	}
	else
	{
		_currentIndex = (Index + 1) * -1; //Reverse fading
	
		SplashImage->SetBrushFromSoftTexture(SplashImages[Index]);
	
		PlayAnimation(Fade, 0.0, 1, EUMGSequencePlayMode::Forward, (Fade->GetEndTime() - Fade->GetStartTime()) / FadeAnimationDuration, false);
	}
}

void USplashScreenWidget::OnFinishSplash_Implementation()
{
	//Implemented in Blueprint
}