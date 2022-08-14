#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CursoryTypes.h"
#include "CursorySettings.generated.h"

UCLASS(config=Game, defaultconfig, meta=(DisplayName="Cursors", ToolTip="Custom hardware cursors for your game."))
class UCursorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	/** Custom cursor specs. Will be loaded on Engine startup. */
	UPROPERTY(EditAnywhere, config, Category = "Cursors")
	TSet<FCursorInfo> CustomCursorSpecs;

	/**
	 * If true, automatically focuses viewport when directly hovered.
	 * Prevents reversion to default cursor when viewport loses focus (e.g. on button press).
	 * If false, users will need to manually restore focus to viewport
	 * once lost.
	 * Can be toggled at runtime.
	 */
	UPROPERTY(EditAnywhere, config, Category = "Focus")
	bool bAutoFocusViewport{true};
};
