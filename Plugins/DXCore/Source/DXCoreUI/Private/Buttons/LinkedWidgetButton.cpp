#include "Buttons/LinkedWidgetButton.h"

void ULinkedWidgetButton::NativeOnClicked()
{
	Super::NativeOnClicked();

	if (!IsDecisiveButton)
		OpenChosenWidget();

	if (_linkedWidget)
	{
		_linkedWidget->OnDeactivated().AddLambda([this]()
		{
			OnLinkedWidgetDeactivated();
		});
	}
}

void ULinkedWidgetButton::OpenChosenWidget()
{
	if (_linkedWidget == nullptr)
	{
		_linkedWidget = CreateWidget<UCommonActivatableWidget>(GetWorld(), WidgetToOpen, TEXT("ChosenWidget"));
		_linkedWidget->AddToViewport();
		_linkedWidget->ActivateWidget();
	}
	else
	{
		_linkedWidget->ActivateWidget();
	}
}

void ULinkedWidgetButton::OnLinkedWidgetDeactivated_Implementation()
{
	
}
