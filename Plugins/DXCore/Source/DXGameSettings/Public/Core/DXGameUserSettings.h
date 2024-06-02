#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DXGameUserSettings.generated.h"

UCLASS(BlueprintType, Blueprintable)
class DXGAMESETTINGS_API UDXGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UDXGameUserSettings(const FObjectInitializer& ObjectInitializer);
};
