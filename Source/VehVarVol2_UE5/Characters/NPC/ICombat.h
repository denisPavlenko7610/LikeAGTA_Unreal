// 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICombat.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UICombat : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VEHVARVOL2_UE5_API IICombat
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	int FireAttack();
};
