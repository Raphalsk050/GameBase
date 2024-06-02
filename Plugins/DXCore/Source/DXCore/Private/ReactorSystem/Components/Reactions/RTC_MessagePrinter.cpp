// Fill out your copyright notice in the Description page of Project Settings.


#include "ReactorSystem/Components/Reactions/RTC_MessagePrinter.h"

void URTC_MessagePrinter::OnReactorActivating_Implementation(AController* controller)
{	
	GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, DisplayColor, Message, true, TextScale);

	if (LogToConsole)
		UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
}