#include "ReactorSystem/Components/Reactions/RTC_Analytics.h"
#include <Analytics.h>
#include "Interfaces/IAnalyticsProvider.h"


void URTC_Analytics::BeginPlay()
{
	Super::BeginPlay();
	
	if(FAnalytics::Get().IsAvailable())
	{
		AnalyticsProvider = FAnalytics::Get().GetDefaultConfiguredProvider();
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
			FString::Printf(TEXT("%s: analytics provider not set. Reaction component will not work."), *GetNameSafe(this)));
		}
	}
}

void URTC_Analytics::OnReactorActivating_Implementation(AController* controller)
{
	Super::OnReactorActivating_Implementation(controller);

	if(AnalyticsProvider)
	{
		//force instigator/object info
		FDXAnalyticsEventAttr Instigator;
		Instigator.Name = ("Instigator");
		Instigator.Value = GetNameSafe(controller);

		FDXAnalyticsEventAttr Object;
		Object.Name = "Object";
		Object.Value = GetNameSafe(this->GetOwner());

		ObjectEventAttributes.Push(Object);
		ObjectEventAttributes.Push(Instigator);
		
		AnalyticsProvider->RecordEvent("ObjectEvents", ConvertAttrs(ObjectEventAttributes));
	}
}
