#pragma once

#include "FrontendStructTypes.generated.h"

USTRUCT()
struct FOptionsDataEditConditionDescription
{
	GENERATED_BODY()

public:

	void SetEditConditionFunc(TFunction<bool()> InEditConditionFunc) { EditConditionFunc = InEditConditionFunc; }
	bool IsValid() const { return EditConditionFunc != nullptr; }
	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			return EditConditionFunc();
		}

		return true;
	}

	FString GetDisabledRichReason() const { return DisabledRichReason; }
	void SetDisabledRichReason(const FString& InRichReason)
	{
		DisabledRichReason = InRichReason;
	}

	bool HasForcedStringValie() const { return DisabledForcedStringValue.IsSet(); }
	FString GetDisabledForcedStringValue() const { return DisabledForcedStringValue.GetValue(); }
	void SetDisabledForcedStringValue(const FString& InRichReason)
	{
		DisabledForcedStringValue = InRichReason;
	}
	
private:

	TFunction<bool()> EditConditionFunc;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;
};
