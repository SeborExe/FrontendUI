// Fill out your copyright notice in the Description page of Project Settings.


#include "FrontUI/AsyncActions/AsyncAction_PushConfirmScreen.h"

#include "FrontUI/Subsystem/FrontendUISubsystem.h"

UAsyncAction_PushConfirmScreen* UAsyncAction_PushConfirmScreen::PushConfirmScreen(const UObject* WorldContextObject,
                                                                                  EConfirmScreenType ScreenType, FText InScreenTitle, FText InScreenMessage)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncAction_PushConfirmScreen* Node = NewObject<UAsyncAction_PushConfirmScreen>();
			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMessage = InScreenMessage;

			Node->RegisterWithGameInstance(World);
			return Node;
		}
	}

	return nullptr;
}

void UAsyncAction_PushConfirmScreen::Activate()
{
	UFrontendUISubsystem* FrontendUISubsystem = UFrontendUISubsystem::Get(CachedOwningWorld.Get());
	if (FrontendUISubsystem)
	{
		FrontendUISubsystem->PushConfirmScreenToModalStackAsync(CachedScreenType, CachedScreenTitle, CachedScreenMessage, [this](EConfirmScreenButtonType ButtonType)
		{
			OnConfirmScreenButtonClicked.Broadcast(ButtonType);
			SetReadyToDestroy();
		});
	}
}
