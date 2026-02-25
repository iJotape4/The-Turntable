#pragma once

#include "GenericPayloads.generated.h"

USTRUCT()
struct FGenericUIDialogueEvent
{
	GENERATED_BODY()
	FText Text;
	float Duration= 4.0f;
};

USTRUCT()
struct FDummyTestEvent
{
	GENERATED_BODY()	
};