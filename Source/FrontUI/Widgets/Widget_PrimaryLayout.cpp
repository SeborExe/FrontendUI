// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontUI/Widgets/Widget_PrimaryLayout.h"
#include "FrontUI/Debug/FrontendDebugHelper.h"

UCommonActivatableWidgetContainerBase* UWidget_PrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("Can not find the widget stack by tag: %s"), *InTag.ToString());

	return RegisteredWidgetStackMap.FindRef(InTag);
}

void UWidget_PrimaryLayout::RegisterWidgetClass(UPARAM(meta=(Categories="Frontend.WidgetStack")) FGameplayTag InSTackTag, UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InSTackTag))
		{
			RegisteredWidgetStackMap.Add(InSTackTag, InStack);
			//Debug::Print(TEXT("Widget Stack registered under the tag ") + InSTackTag.ToString());
		}
	}
}
