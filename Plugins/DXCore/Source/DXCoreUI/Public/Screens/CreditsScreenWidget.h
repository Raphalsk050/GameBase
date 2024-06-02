// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Sound/SoundCue.h"
#include "Engine/DataTable.h"
#include "CreditsScreenWidget.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class ECreditsType : uint8
{
	Scrolling UMETA(DisplayName = "Scrolling"),
	PerBlock UMETA(DisplayName = "Per Block")
};

USTRUCT(BlueprintType)
struct FCreditsRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Role;
};

UCLASS()
class DXCOREUI_API UCreditsScreenWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UDataTable* CreditsData;

	UPROPERTY()
	ECreditsType CreditsType;

	UPROPERTY()
	TArray<FText> RolesArray;

	UPROPERTY()
	FText CurrentRole;

	UPROPERTY()
	int32 RolesIndex;

	UPROPERTY()
	int32 Columns = 1;

	UPROPERTY()
	bool Skippable = true;

	UPROPERTY()
	USoundCue* CreditsMusicTheme;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget))
	UScrollBox* CreditsScreen_ScrollBox;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Idle;

	/** Scroll animation, is the animation that's occur in the */
	UPROPERTY(EditAnywhere)
	float ScrollAnimationDuration = 1.0;

	/** Idle animation, is the animation that's occur during the */
	UPROPERTY(EditAnywhere)
	float IdleAnimationDuration = 2.0;

	virtual void NativeOnActivated() override;

	UFUNCTION()
	void ScrollingCredits();

	UFUNCTION()
	void PerBlockCredits();

	UFUNCTION()
	void ShowCreditsBlock(int32 Index);

	UFUNCTION()
	void CreateCredits();

	UFUNCTION()
	void CreateCreditsRoles();

	UFUNCTION()
	void CreateCreditsNames(FName Name);
};
