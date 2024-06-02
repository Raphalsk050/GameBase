// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "SplashScreenWidget.generated.h"

/**
 *  This class displays a sequence of flat images on screen
 */
UCLASS()
class DXCOREUI_API USplashScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnActivated() override;
	
	UFUNCTION(BlueprintCallable)
	void StartSplashShow();

	UFUNCTION(BlueprintNativeEvent)
	void OnFinishSplash();
	
protected:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	UImage* SplashImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Fade;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Idle;

	/** Fade animation, is the animation that's occur in the beginning and in the end of the splash show. */
	UPROPERTY(EditAnywhere)
	float FadeAnimationDuration = 1.0;

	/** Idle animation, is the animation that's occur during the splash show. */
	UPROPERTY(EditAnywhere)
	float IdleAnimationDuration = 2.0;

	/** The images that you need to show in the splash show. */
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<UTexture2D>> SplashImages;
	
private:
	UFUNCTION()
	void FadeFinished();

	UFUNCTION()
	void IdleFinished();

	void ChangeImage(int Index);

	UPROPERTY()
	FWidgetAnimationDynamicEvent _onImageEnter;

	UPROPERTY()
	FWidgetAnimationDynamicEvent _onImageExit;
	
	int _currentIndex;
	bool _playing = false;
};
