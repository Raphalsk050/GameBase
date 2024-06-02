#include "Buttons/BaseButton.h"

#include "CommonActionWidget.h"
#include "Screens/ConfirmationPopUpWidget.h"

void UBaseButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if(!ButtonContext_TextBlock) return;

	if (ButtonContextName.IsEmpty()) return;
	
	ButtonContext_TextBlock->SetText(ButtonContextName);
}

void UBaseButton::NativeConstruct()
{
	Super::NativeConstruct();

	if(!ButtonContext_TextBlock) return;
	
	if(DisplayActionText)
		ButtonContext_TextBlock->SetText(InputActionWidget->GetDisplayText());
	else
		ButtonContext_TextBlock->SetText(ButtonContextName);
}

void UBaseButton::NativeOnClicked()
{
	Super::NativeOnClicked();
	
	if (IsDecisiveButton)
	{
		SpawnConfirmationPopUp();
	}
}

void UBaseButton::UpdateStyle()
{
	if (DisplayActionText)
		ButtonContext_TextBlock->SetText(InputActionWidget->GetDisplayText());
}

void UBaseButton::SpawnConfirmationPopUp()
{
	if (!ConfirmationPopUpClass) return;
	
	_confirmationPopUp = CreateWidget<UConfirmationPopUpWidget>(GetWorld(), ConfirmationPopUpClass, TEXT("ConfirmationPopUp"));
		
	FVector2D viewportSize = FVector2D(1080, 720);
	if (UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport())
	{
		ViewportClient->GetViewportSize(viewportSize);
	}
		
	_confirmationPopUp->SetPositionInViewport(FVector2D(viewportSize.X/2,viewportSize.Y/2));
	_confirmationPopUp->SetAlignmentInViewport(FVector2D(0.5, 0.5));

	_confirmationPopUp->ConfirmationTitle = ConfirmationPopUpTitle;
	_confirmationPopUp->ConfirmationDescription = ConfirmationPopUpDescription;
	_confirmationPopUp->ConfirmInputAction = ConfirmationPopUpConfirmInputAction;
	_confirmationPopUp->DeclineInputAction = ConfirmationPopUpDeclineInputAction;

	OnConfirmationPopUpCreated.Broadcast(_confirmationPopUp);
		
	_confirmationPopUp->AddToViewport();
	_confirmationPopUp->ActivateWidget();
}
