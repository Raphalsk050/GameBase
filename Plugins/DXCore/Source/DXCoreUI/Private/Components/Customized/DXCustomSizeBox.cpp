// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/Customized/DXCustomSizeBox.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Components/Customized/DXCustomBox.h"
#include "Components/Customized/DXCustomSizeBoxSlot.h"

#define LOCTEXT_NAMESPACE "DXCustomUMG"

/////////////////////////////////////////////////////
// UDXCustomSizeBox

UDXCustomSizeBox::UDXCustomSizeBox(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsVariable = false;
	UWidget::SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	MinAspectRatio = 1;
	MaxAspectRatio = 1;
}

void UDXCustomSizeBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MySizeBox.Reset();
}

TSharedRef<SWidget> UDXCustomSizeBox::RebuildWidget()
{
	MySizeBox = SNew(SDXCustomBox);

	if (GetChildrenCount() > 0)
	{
		UDXCustomSizeBoxSlot* CustomSizeBoxSlot = Cast<UDXCustomSizeBoxSlot>(GetContentSlot());
		if (CustomSizeBoxSlot)
			CustomSizeBoxSlot->BuildSlot(MySizeBox.ToSharedRef());
	}

	return MySizeBox.ToSharedRef();
}

void UDXCustomSizeBox::SetWidthOverride(float InWidthOverride)
{
	bOverride_WidthOverride = true;
	WidthOverride = InWidthOverride;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetWidthOverride(InWidthOverride);
	}
}

void UDXCustomSizeBox::ClearWidthOverride()
{
	bOverride_WidthOverride = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetWidthOverride(FOptionalSize());
	}
}

void UDXCustomSizeBox::SetHeightOverride(float InHeightOverride)
{
	bOverride_HeightOverride = true;
	HeightOverride = InHeightOverride;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetHeightOverride(InHeightOverride);
	}
}

void UDXCustomSizeBox::ClearHeightOverride()
{
	bOverride_HeightOverride = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetHeightOverride(FOptionalSize());
	}
}

void UDXCustomSizeBox::SetMinDesiredWidth(float InMinDesiredWidth)
{
	bOverride_MinDesiredWidth = true;
	MinDesiredWidth = InMinDesiredWidth;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinDesiredWidth(InMinDesiredWidth);
	}
}

void UDXCustomSizeBox::ClearMinDesiredWidth()
{
	bOverride_MinDesiredWidth = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinDesiredWidth(FOptionalSize());
	}
}

void UDXCustomSizeBox::SetMinDesiredHeight(float InMinDesiredHeight)
{
	bOverride_MinDesiredHeight = true;
	MinDesiredHeight = InMinDesiredHeight;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinDesiredHeight(InMinDesiredHeight);
	}
}

void UDXCustomSizeBox::ClearMinDesiredHeight()
{
	bOverride_MinDesiredHeight = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinDesiredHeight(FOptionalSize());
	}
}

void UDXCustomSizeBox::SetMaxDesiredWidth(float InMaxDesiredWidth)
{
	bOverride_MaxDesiredWidth = true;
	MaxDesiredWidth = InMaxDesiredWidth;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxDesiredWidth(InMaxDesiredWidth);
	}
}

void UDXCustomSizeBox::ClearMaxDesiredWidth()
{
	bOverride_MaxDesiredWidth = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxDesiredWidth(FOptionalSize());
	}
}

void UDXCustomSizeBox::SetMaxDesiredHeight(float InMaxDesiredHeight)
{
	bOverride_MaxDesiredHeight = true;
	MaxDesiredHeight = InMaxDesiredHeight;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxDesiredHeight(InMaxDesiredHeight);
	}
}

void UDXCustomSizeBox::ClearMaxDesiredHeight()
{
	bOverride_MaxDesiredHeight = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxDesiredHeight(FOptionalSize());
	}
}

void UDXCustomSizeBox::SetMinAspectRatio(float InMinAspectRatio)
{
	bOverride_MinAspectRatio = true;
	MinAspectRatio = InMinAspectRatio;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinAspectRatio(InMinAspectRatio);
	}
}

void UDXCustomSizeBox::ClearMinAspectRatio()
{
	bOverride_MinAspectRatio = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMinAspectRatio(FOptionalSize());
	}
}

void UDXCustomSizeBox::SetMaxAspectRatio(float InMaxAspectRatio)
{
	bOverride_MaxAspectRatio = true;
	MaxAspectRatio = InMaxAspectRatio;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxAspectRatio(InMaxAspectRatio);
	}
}

void UDXCustomSizeBox::ClearMaxAspectRatio()
{
	bOverride_MaxAspectRatio = false;
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetMaxAspectRatio(FOptionalSize());
	}
}

void UDXCustomSizeBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (bOverride_WidthOverride)
	{
		SetWidthOverride(WidthOverride);
	}
	else
	{
		ClearWidthOverride();
	}

	if (bOverride_HeightOverride)
	{
		SetHeightOverride(HeightOverride);
	}
	else
	{
		ClearHeightOverride();
	}

	if (bOverride_MinDesiredWidth)
	{
		SetMinDesiredWidth(MinDesiredWidth);
	}
	else
	{
		ClearMinDesiredWidth();
	}

	if (bOverride_MinDesiredHeight)
	{
		SetMinDesiredHeight(MinDesiredHeight);
	}
	else
	{
		ClearMinDesiredHeight();
	}

	if (bOverride_MaxDesiredWidth)
	{
		SetMaxDesiredWidth(MaxDesiredWidth);
	}
	else
	{
		ClearMaxDesiredWidth();
	}

	if (bOverride_MaxDesiredHeight)
	{
		SetMaxDesiredHeight(MaxDesiredHeight);
	}
	else
	{
		ClearMaxDesiredHeight();
	}

	if (bOverride_MinAspectRatio)
	{
		SetMinAspectRatio(MinAspectRatio);
	}
	else
	{
		ClearMinAspectRatio();
	}

	if (bOverride_MaxAspectRatio)
	{
		SetMaxAspectRatio(MaxAspectRatio);
	}
	else
	{
		ClearMaxAspectRatio();
	}
}

UClass* UDXCustomSizeBox::GetSlotClass() const
{
	return UDXCustomSizeBoxSlot::StaticClass();
}

void UDXCustomSizeBox::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if (MySizeBox.IsValid())
	{
		CastChecked<UDXCustomSizeBoxSlot>(InSlot)->BuildSlot(MySizeBox.ToSharedRef());
	}
}

void UDXCustomSizeBox::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MySizeBox.IsValid())
	{
		MySizeBox->SetContent(SNullWidget::NullWidget);
	}
}

#if WITH_EDITOR

const FText UDXCustomSizeBox::GetPaletteCategory()
{
	return LOCTEXT("Panel", "Panel");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE