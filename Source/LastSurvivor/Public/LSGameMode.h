#pragma once

#include "LastSurvivor.h"
#include "GameFramework/GameModeBase.h"
#include "LSGameMode.generated.h"

UCLASS()
class LASTSURVIVOR_API ALSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALSGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
};
