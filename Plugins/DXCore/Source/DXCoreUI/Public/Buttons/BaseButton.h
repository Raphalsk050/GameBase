#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Components/TextBlock.h"
#include "BaseButton.generated.h"

class UConfirmationPopUpWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmationPopUpCreated, UConfirmationPopUpWidget*, PopUpCreated);
/**
 * A Button with a text component. Can create a ConfirmationPopUp widget if IsDecisiveButton is true.
 */
//class UConfirmationPopUpWidget;
UCLASS()
class DXCOREUI_API UBaseButton : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnClicked() override;

	UFUNCTION(BlueprintCallable)
	virtual void UpdateStyle(); 

	UPROPERTY(EditAnywhere)
	bool DisplayActionText = false;

	/** Defines the text that appears on the button*/
	UPROPERTY(EditAnywhere)
	FText ButtonContextName;

	/** Button text component */
	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
	UTextBlock* ButtonContext_TextBlock;

	/** If true, when the button is clicked, a Confirmation PopUp will be created.*/
	UPROPERTY(EditAnywhere, Category = "ConfirmationPopUp")
	bool IsDecisiveButton;

	UPROPERTY(EditAnywhere, meta=(EditCondition="IsDecisiveButton"), Category = "ConfirmationPopUp")
	TSubclassOf<UConfirmationPopUpWidget> ConfirmationPopUpClass;
	
	UPROPERTY(EditAnywhere, meta=(EditCondition="IsDecisiveButton"), Category = "ConfirmationPopUp")
	FText ConfirmationPopUpTitle;

	UPROPERTY(EditAnywhere, meta=(EditCondition="IsDecisiveButton"), Category = "ConfirmationPopUp")
	FText ConfirmationPopUpDescription;

	UPROPERTY(EditAnywhere, meta=(EditCondition="IsDecisiveButton", RowType = CommonInputActionDataBase), Category = "ConfirmationPopUp")
	FDataTableRowHandle ConfirmationPopUpConfirmInputAction;
	
	UPROPERTY(EditAnywhere, meta=(EditCondition="IsDecisiveButton", RowType = CommonInputActionDataBase), Category = "ConfirmationPopUp")
	FDataTableRowHandle ConfirmationPopUpDeclineInputAction;

	FOnConfirmationPopUpCreated OnConfirmationPopUpCreated;

	UConfirmationPopUpWidget* GetConfirmationPopUp() { return _confirmationPopUp; }

protected:
	void SpawnConfirmationPopUp();
	
private:
	UPROPERTY()
	UConfirmationPopUpWidget* _confirmationPopUp = nullptr;
};
