// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FrontUI/Widgets/Options/DataObjects/ListDataObject_Base.h"
#include "ListDataObject_Collection.generated.h"

UCLASS()
class FRONTUI_API UListDataObject_Collection : public UListDataObject_Base
{
	GENERATED_BODY()

public:

	void AddChildListData(UListDataObject_Base* InChildListData);

	virtual TArray<UListDataObject_Base*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;

private:

	UPROPERTY(Transient)
	TArray<UListDataObject_Base*> ChildListDataArray;
};
