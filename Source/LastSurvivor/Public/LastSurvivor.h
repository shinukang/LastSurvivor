#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LastSurvivor, Log, All);

#define LSCHECK(Expr, ...) { if(!(Expr)) { LSLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}
#define LSLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define LSLOG_S(Verbosity) UE_LOG(LastSurvivor, Verbosity, TEXT("%s"), *LSLOG_CALLINFO)
#define LSLOG(Verbosity, Format, ...) UE_LOG(LastSurvivor, Verbosity, TEXT("%s"), *LSLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

FName SurfaceTypeToName(TEnumAsByte<EPhysicalSurface> SurfaceType);
