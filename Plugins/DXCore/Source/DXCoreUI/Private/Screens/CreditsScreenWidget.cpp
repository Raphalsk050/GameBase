// Fill out your copyright notice in the Description page of Project Settings.


#include "Screens/CreditsScreenWidget.h"

void UCreditsScreenWidget::NativeOnActivated()
{
	Super::NativeOnActivated();

	CreateCredits();
}

void UCreditsScreenWidget::CreateCredits()
{
	// TArray<FName> RowNames = CreditsData->GetRowNames();
	// FString* Context;
	// for (FName RowName : RowNames)
	// {
	// 	CreditsData->FindRow(RowName, Context);
	// 	//stuff
	// 	RolesArray.AddUnique();
	// }

	switch (CreditsType)
	{
	case ECreditsType::Scrolling:
		ScrollingCredits();
		break;

	case ECreditsType::PerBlock:
		PerBlockCredits();
		break;
	}
}

void UCreditsScreenWidget::ScrollingCredits()
{
	for (FText Role : RolesArray)
	{
		CurrentRole = Role;
		CreateCreditsRoles();
	}
}

void UCreditsScreenWidget::PerBlockCredits()
{
	ShowCreditsBlock(RolesIndex);
}

void UCreditsScreenWidget::ShowCreditsBlock(int32 Index)
{

}

void UCreditsScreenWidget::CreateCreditsRoles()
{

}

void UCreditsScreenWidget::CreateCreditsNames(FName Name)
{

}
