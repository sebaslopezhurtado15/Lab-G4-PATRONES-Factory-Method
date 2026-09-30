//Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"
#include "AventuraUSFX022026L4GameMode.generated.h"

class APlataforma;

//PATRONES
class AEnemigo;

UCLASS(MinimalAPI)
class AAventuraUSFX022026L4GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAventuraUSFX022026L4GameMode();

protected:
	virtual void BeginPlay() override;


public:

	UPROPERTY()TArray<APlataforma*> aPlataformas;

	// Guarda la referencia al enemigo Singleton
	UPROPERTY()AEnemigo* Enemigo;

	FTimerHandle TimerMovimiento;
	FTimerHandle TimerEliminarUnaPlataformaPorHija;
	FTimerHandle TimerReposicionarPlataformas;


	void IniciarMovimiento();
	void EliminarUnaPlataformaPorHija();
	void ReposicionarPlataformas();
};