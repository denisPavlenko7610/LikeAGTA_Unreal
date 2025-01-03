#pragma once

UENUM(BlueprintType)
enum class EMappingContextType : uint8
{
	PlayerContext UMETA(DisplayName = "Player Context"),
	VehicleContext UMETA(DisplayName = "Vehicle Context")
};