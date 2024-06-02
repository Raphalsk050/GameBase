// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Input/CommonBoundActionBar.h"
#include "ActionBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DXCOREUI_API UActionBarWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/* The BoundActionBar is mandatory, for the widget work properly. Don't avoid it! */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCommonBoundActionBar* BoundActionBar;
};
