#pragma once

#include "CoreMinimal.h"
#include "Buttons/SettingButton.h"
#include "SettingSelectorButton.generated.h"

/**
 * With the SettingSelectorButton you can navigate between the elements of an Array.
 */
UCLASS()
class DXCOREUI_API USettingSelectorButton : public USettingButton
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	/** Back to previous element */
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* LeftButton;

	/** Advance to the next element */
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* RightButton;
};
