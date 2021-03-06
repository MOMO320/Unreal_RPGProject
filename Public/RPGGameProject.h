// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(RPGGameProject, Log, All);

UENUM(BlueprintType)
enum class ECharacterState :uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

#define ABLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ABLOG_S(Verbosity) UE_LOG(RPGGameProject , +, TEXT("%s"), *ABLOG_CALLINFO)
#define ABLOG(Verbosity , Format , ...) UE_LOG(RPGGameProject , Verbosity, TEXT("%s%s"), *ABLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define ABCHECK(Expr, ...) {if(!(Expr)) {ABLOG(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}