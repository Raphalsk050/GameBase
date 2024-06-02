#include "Video/ContrastSetting.h"
#include "Components/PostProcessComponent.h"
#include "Engine/PostProcessVolume.h"
#include "Kismet/GameplayStatics.h"

void UContrastSetting::InitSetting_Implementation(UObject* WorldContextObject)
{
	_worldContext = WorldContextObject;
	const FActorSpawnParameters SpawnInfo;
	
	_postProcessVolume = WorldContextObject->GetWorld()->SpawnActor<APostProcessVolume>(
		APostProcessVolume::StaticClass(), FVector::Zero(), FRotator::ZeroRotator, SpawnInfo);

	if (_postProcessVolume == nullptr)
		return;
		
	_postProcessVolume->Priority = 3.f;
	_postProcessVolume->bUnbound = true;
	_postProcessVolume->Settings.bOverride_ColorContrast = true;
	
	Super::InitSetting_Implementation(WorldContextObject);
}

void UContrastSetting::ChangeSettingValue_Implementation(const FString& NewValue)
{
	ChangePostProcessSettings(NewValue);
}

void UContrastSetting::ChangePostProcessSettings(const FString& NewValue) const
{
	float value = FCString::Atof(ToCStr(NewValue));
	const float newContrastValue = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(MinContrastValue, MaxContrastValue), value);
	const FString ConsoleString = "Slate.Contrast " + FString::SanitizeFloat(newContrastValue);
	UKismetSystemLibrary::ExecuteConsoleCommand(_worldContext, ConsoleString);
	
	if (_postProcessVolume != nullptr)
		_postProcessVolume->Settings.ColorContrast = FVector4(newContrastValue, newContrastValue, newContrastValue, 1.f);
}
