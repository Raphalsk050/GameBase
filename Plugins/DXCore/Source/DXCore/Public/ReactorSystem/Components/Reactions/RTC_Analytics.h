#pragma once
#include "CoreMinimal.h"
#include "AnalyticsEventAttribute.h"
#include "ReactorSystem/Components/ReactionComponent.h"
#include "RTC_Analytics.generated.h"

class FAnalytics;

/** Blueprint accessible version of the analytics event struct */
USTRUCT(BlueprintType)
struct FDXAnalyticsEventAttr
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Analytics")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Analytics")
	FString Value;
};

//Sends analytics data to the Analytics System
UCLASS(ClassGroup=(Reaction), meta=(BlueprintSpawnableComponent))
class DXCORE_API URTC_Analytics : public UReactionComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<FDXAnalyticsEventAttr> ObjectEventAttributes;
	
	TSharedPtr<IAnalyticsProvider> AnalyticsProvider;

	static inline TArray<FAnalyticsEventAttribute> ConvertAttrs(const TArray<FDXAnalyticsEventAttr>& Attributes)
	{
		TArray<FAnalyticsEventAttribute> Converted;
		Converted.Reserve(Attributes.Num());
		for (const FDXAnalyticsEventAttr& Attr : Attributes)
		{
			Converted.Emplace(Attr.Name, Attr.Value);
		}
		return Converted;
	}


public:
	virtual void OnReactorActivating_Implementation(AController* controller) override;
};
