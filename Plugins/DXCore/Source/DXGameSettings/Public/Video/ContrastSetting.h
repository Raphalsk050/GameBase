#pragma once

#include "CoreMinimal.h"
#include "Core/SettingObject.h"
#include "Engine/PostProcessVolume.h"
#include "ContrastSetting.generated.h"

/**
 * This class should only be used as a parent class for audio SettingObjects.
 */
UCLASS()
class DXGAMESETTINGS_API UContrastSetting : public USettingObject
{
	GENERATED_BODY()

public:
	/** This value serves as a minimum output parameter for a remapping that generates the value that will be used by the contrast. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinContrastValue = 0.0f;

	/** This value serves as the maximum output parameter for a remapping that generates the value that will be used by the contrast. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxContrastValue = 0.0f;
	
	virtual void InitSetting_Implementation(UObject* WorldContextObject) override; 
	virtual void ChangeSettingValue_Implementation(const FString& NewValue) override;

private:
	const UObject* _worldContext;
	APostProcessVolume* _postProcessVolume;
	
	void ChangePostProcessSettings(const FString& NewValue) const;
};
