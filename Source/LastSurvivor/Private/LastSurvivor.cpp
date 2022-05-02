#include "LastSurvivor.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LastSurvivor);

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, LastSurvivor, "LastSurvivor" );

FName SurfaceTypeToName(TEnumAsByte<EPhysicalSurface> SurfaceType)
{
	FName Name;
	
	switch(SurfaceType)
	{
	case SurfaceType1:
		Name = FName("Concrete");
		break;
	case SurfaceType2:
		Name = FName("Dirt");
		break;
	case SurfaceType3:
		Name = FName("Metal");
		break;
	case SurfaceType4:
		Name = FName("Water");
		break;
	case SurfaceType5:
		Name = FName("Wood");
		break;
	default:
		Name = FName("Grass");
	}

	return Name;
}
