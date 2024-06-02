#include "Video/BrightnessSetting.h"
#include "Kismet/GameplayStatics.h"

void UBrightnessSetting::InitSetting_Implementation(UObject* WorldContextObject)
{
	Super::InitSetting_Implementation(WorldContextObject);
}

void UBrightnessSetting::ChangeSettingValue_Implementation(const FString& NewValue)
{
	float value = FCString::Atof(ToCStr(NewValue));
	float newBrightnessValue = FMath::GetMappedRangeValueClamped
	(FVector2D(0.f, 1.f),
	FVector2D(MinOutputBrightnessValue, MaxOutputBrightnessValue), value);

	const FString ConsoleString = "Gamma " + FString::SanitizeFloat(newBrightnessValue);
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), ConsoleString);
}
