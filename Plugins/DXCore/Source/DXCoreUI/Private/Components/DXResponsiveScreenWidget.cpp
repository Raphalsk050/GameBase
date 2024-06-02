// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/DXResponsiveScreenWidget.h"
#include "Components/Customized/DXCustomSizeBox.h"
#include "DXGameSettings/Public/Core/DXGameSettingsSubsystem.h"
#include "DXGameSettings/Public/Core/SettingObject.h"


void UDXResponsiveScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GameSettingsSubsystem = GetGameInstance()->GetSubsystem<UDXGameSettingsSubsystem>();
	check(GameSettingsSubsystem);

	GameSettingsSubsystem->OnSettingChange.AddDynamic(this, &UDXResponsiveScreenWidget::OnSettingChange);
}

void UDXResponsiveScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateSreenSize(true);
}

void UDXResponsiveScreenWidget::OnSettingChange(TSubclassOf<USettingObject> Setting)
{
	if (!Setting) return;

	if (Setting->GetName() != SettingObjectAttached->GetName()) return;
	
	UpdateSreenSize(false);
}

void UDXResponsiveScreenWidget::UpdateSreenSize(bool bFromConstruct)
{
	GameSettingsSubsystem = GetGameInstance()->GetSubsystem<UDXGameSettingsSubsystem>();
	if (!GameSettingsSubsystem) return;

	FIntVector2 screenTargetRatio;
	
	if (bFromConstruct)
	{
		int value =  FCString::Atoi(ToCStr(GameSettingsSubsystem->GetSettingSavedValue(SettingObjectAttached)));
		screenTargetRatio = GetDesiredScreenRatio(value);
	}
	else
	{
		int value =  FCString::Atoi(ToCStr(GameSettingsSubsystem->GetSettingCurrentValue(SettingObjectAttached)));
		screenTargetRatio = GetDesiredScreenRatio(value);
	}
	if(screenTargetRatio.Y > 0)
		WidescreenRatio_SizeBox->SetMaxAspectRatio(screenTargetRatio.X / (float)screenTargetRatio.Y);
}

FIntVector2 UDXResponsiveScreenWidget::GetDesiredScreenRatio(int32 SettingValue)
{
	FIntVector2 screenRatio = FIntVector2();
	//
	// if (SettingObjectAttached.GetDefaultObject()->GetSettingOptions().Num() <= 0)
	// 	return screenRatio;
	//
	// FText option = SettingObjectAttached.GetDefaultObject()->GetSettingOptions()[SettingValue].OptionName;
	//
	// FString leftSplit, rightSplit;
	// option.ToString().Split(":", &leftSplit, &rightSplit);
	//
	// screenRatio.X = FCString::Atoi(*leftSplit);
	// screenRatio.Y = FCString::Atoi(*rightSplit);
	//
	// UE_LOG(LogTemp, Warning, TEXT("Desired screen ratio is: X:%i, Y:%i"), screenRatio.X, screenRatio.Y);
	
	return screenRatio;
}
