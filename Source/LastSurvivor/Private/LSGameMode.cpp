#include "LSGameMode.h"
#include "LSCharacter.h"
#include "LSPlayerController.h"

ALSGameMode::ALSGameMode()
{
	DefaultPawnClass = ALSCharacter::StaticClass();
	PlayerControllerClass = ALSPlayerController::StaticClass();
}

void ALSGameMode::PostLogin(APlayerController* NewPlayer)
{
	LSLOG(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	LSLOG(Warning, TEXT("PostLogin End"));
}


