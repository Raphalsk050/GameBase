// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Buttons/BaseButton.h"
#include "ConfirmationPopUpWidget.generated.h"


UENUM(BlueprintType)
enum class EOptionType : uint8
{
	OT_Declined UMETA(DisplayName = "Declined"),
	OT_Confirmed UMETA(DisplayName = "Confirmed")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOptionSelected, EOptionType, OptionSelected);

/**
 * This class provides a template for any confirmation pop up, calling YES and NO events
 */
UCLASS()
class DXCOREUI_API UConfirmationPopUpWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FText ConfirmationTitle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FText ConfirmationDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", RowType = CommonInputActionDataBase))
	FDataTableRowHandle ConfirmInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true", RowType = CommonInputActionDataBase))
	FDataTableRowHandle DeclineInputAction;

	UFUNCTION(BlueprintCallable)
	void UpdateTexts(const FText& Title, const FText& Description);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", ExposeOnSpawn = "true"))
	bool _destroyOnDecline = true;

	UFUNCTION()
	void ClosePopup();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ConfirmationTitle_TextBlock;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* ConfirmationDescription_TextBlock;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBaseButton* Confirm_Button;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UBaseButton* Decline_Button;

public:
	UPROPERTY(BlueprintAssignable)
	FOnOptionSelected OnOptionSelected;

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;
};
