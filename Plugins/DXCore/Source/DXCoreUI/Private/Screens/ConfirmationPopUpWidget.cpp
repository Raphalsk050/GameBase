// Fill out your copyright notice in the Description page of Project Settings.


#include "Screens/ConfirmationPopUpWidget.h"

void UConfirmationPopUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateTexts(ConfirmationTitle, ConfirmationDescription);

	Confirm_Button->SetTriggeringInputAction(ConfirmInputAction);
	Decline_Button->SetTriggeringInputAction(DeclineInputAction);

	Confirm_Button->DisplayActionText = true;
	Confirm_Button->UpdateStyle();

	Decline_Button->DisplayActionText = true;
	Decline_Button->UpdateStyle();
}

void UConfirmationPopUpWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Confirm_Button->OnClicked().AddLambda([this]()
		{
			this->OnOptionSelected.Broadcast(EOptionType::OT_Confirmed);
			this->ClosePopup();
		});

	Decline_Button->OnClicked().AddLambda([this]()
		{
			this->OnOptionSelected.Broadcast(EOptionType::OT_Declined);
			this->ClosePopup();
		});
}

void UConfirmationPopUpWidget::ClosePopup()
{
	DeactivateWidget();
	if (_destroyOnDecline) RemoveFromParent();
}

void UConfirmationPopUpWidget::UpdateTexts(const FText& Title, const FText& Description)
{
	ConfirmationTitle = Title;
	ConfirmationDescription = Description;
	
	if (ConfirmationTitle_TextBlock != nullptr)
		ConfirmationTitle_TextBlock->SetText(ConfirmationTitle);

	if (ConfirmationDescription_TextBlock != nullptr)
		ConfirmationDescription_TextBlock->SetText(ConfirmationDescription);
}