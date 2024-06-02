#pragma once
#include "CoreMinimal.h"
#include "Core/SettingObject.h"
#include "BrightnessSetting.generated.h"

UCLASS()
class DXGAMESETTINGS_API UBrightnessSetting : public USettingObject
{
	GENERATED_BODY()

public:
	
	/** This value serves as a minimum output parameter for a remapping that generates the value that will be used by the brightness. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinOutputBrightnessValue = 0.0f;

	/** This value serves as the maximum output parameter for a remapping that generates the value that will be used by the brightness. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxOutputBrightnessValue = 0.0f;
	
	virtual void InitSetting_Implementation(UObject* WorldContextObject) override; 
	virtual void ChangeSettingValue_Implementation(const FString& NewValue) override;
};
