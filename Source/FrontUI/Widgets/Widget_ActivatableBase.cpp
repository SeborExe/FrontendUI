// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontUI/Widgets/Widget_ActivatableBase.h"
#include "FrontUI/Controllers/FrontendPlayerController.h"

AFrontendPlayerController* UWidget_ActivatableBase::GetOwningFrontendPlayerController()
{
	if (!CachedOwningFrontendPC.IsValid())
	{
		CachedOwningFrontendPC = GetOwningPlayer<AFrontendPlayerController>();
	}

	return CachedOwningFrontendPC.IsValid()? CachedOwningFrontendPC.Get() : nullptr;
}
