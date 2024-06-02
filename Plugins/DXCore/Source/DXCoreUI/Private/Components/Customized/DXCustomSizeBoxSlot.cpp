// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/Customized/DXCustomSizeBoxSlot.h"

#include "Components/Customized/DXCustomBox.h"
#include "Widgets/SNullWidget.h"
#include "Components/Widget.h"

/////////////////////////////////////////////////////
// UDXCustomSizeBoxSlot

UDXCustomSizeBoxSlot::UDXCustomSizeBoxSlot(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Padding = FMargin(0, 0);

	HorizontalAlignment = HAlign_Fill;
	VerticalAlignment = VAlign_Fill;
}

void UDXCustomSizeBoxSlot::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	SizeBox.Reset();
}

void UDXCustomSizeBoxSlot::BuildSlot(TSharedRef<SDXCustomBox> InSizeBox)
{
	SizeBox = InSizeBox;

	SynchronizeProperties();

	SizeBox.Pin()->SetContent(Content ? Content->TakeWidget() : SNullWidget::NullWidget);
}

void UDXCustomSizeBoxSlot::SetPadding(FMargin InPadding)
{
	Padding = InPadding;
	if ( SizeBox.IsValid() )
	{
		SizeBox.Pin()->SetPadding(InPadding);
	}
}

void UDXCustomSizeBoxSlot::SetHorizontalAlignment(EHorizontalAlignment InHorizontalAlignment)
{
	HorizontalAlignment = InHorizontalAlignment;
	if ( SizeBox.IsValid() )
	{
		SizeBox.Pin()->SetHAlign(InHorizontalAlignment);
	}
}

void UDXCustomSizeBoxSlot::SetVerticalAlignment(EVerticalAlignment InVerticalAlignment)
{
	VerticalAlignment = InVerticalAlignment;
	if ( SizeBox.IsValid() )
	{
		SizeBox.Pin()->SetVAlign(InVerticalAlignment);
	}
}

void UDXCustomSizeBoxSlot::SynchronizeProperties()
{
	SetPadding(Padding);
	SetHorizontalAlignment(HorizontalAlignment);
	SetVerticalAlignment(VerticalAlignment);
}
