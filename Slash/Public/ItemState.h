#pragma once

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Acquired UMETA(DisplayName = "Acquired"),
	EIS_NonAcquired UMETA(DisplayName = "NonAcquired")
};