// Copyright Kpable Games


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound)
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		if (AttributeInfo.AttributeTag == AttributeTag)
		{
			return AttributeInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeInfo for tag %s not found"), *AttributeTag.ToString());
	}

	return FAuraAttributeInfo();
}
