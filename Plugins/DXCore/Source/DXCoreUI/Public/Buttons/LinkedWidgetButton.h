#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BaseButton.h"
#include "LinkedWidgetButton.generated.h"

/**
 *This class is used to create/activate a widget according to WidgetToOpen.
 */
UCLASS()
class DXCOREUI_API ULinkedWidgetButton : public UBaseButton
{
	GENERATED_BODY()

public:
	/**The widget class chosen to be created/opened*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonActivatableWidget> WidgetToOpen;

private:
	/** Save the created widget*/
	UCommonActivatableWidget* _linkedWidget;

public:
	virtual void NativeOnClicked() override;

	/** Create a widget based on WidgetToOpen. If the widget already exists, activate the selected widget */
	void OpenChosenWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void OnLinkedWidgetDeactivated();
	void OnLinkedWidgetDeactivated_Implementation();
};
