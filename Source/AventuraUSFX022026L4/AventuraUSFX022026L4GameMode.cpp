// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L4GameMode.h"
#include "AventuraUSFX022026L4Pawn.h"
#include "Engine/World.h"
#include "Plataforma.h"
#include "PlataformaAerea.h"
#include "PlataformaAcuatica.h"
#include "PlataformaTerrestre.h"
#include "PlataformaSubterranea.h"
//Paimball
#include "PlataformaIndestructible.h"
#include "PlataformaDestructible.h"
// Singleton enemigo
#include "Enemigo.h"
// Factory Method
#include "FabricaPlataformas.h"

// Patron Abstract Factory
#include "FabricaAbstracta.h"
#include "FabricaRoja.h"
#include "FabricaAzul.h"

AAventuraUSFX022026L4GameMode::AAventuraUSFX022026L4GameMode()
{
	DefaultPawnClass = AAventuraUSFX022026L4Pawn::StaticClass();
}

void AAventuraUSFX022026L4GameMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World == nullptr)
	{
		return;
	}

	FRotator Rotacion(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters Parametros;

	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Posiciones disponibles dentro del mini escenario
	TArray<FVector> PosicionesDisponibles;

	for (int X = 350; X <= 750; X += 130)
	{
		for (int Y = -350; Y <= 350; Y += 175)
		{
			PosicionesDisponibles.Add(
				FVector(X, Y, 160.0f)
			);
		}
	}

	//Creacion de 5 a 20 plataformas
	int CantidadPlataformas = FMath::RandRange(5, 20);
	aPlataformas.Reserve(CantidadPlataformas);
	APlataforma* PlataformaActual = nullptr;

	// Creamos las dos fabricas concretas
	FabricaRoja FabricaDeRojas;
	FabricaAzul FabricaDeAzules;

	// Abstract Factory - Generamos plataformas con las fabricas concretas
	for (int i = 0; i < CantidadPlataformas; i++)
	{
		int IndiceAleatorio = FMath::RandRange(0, PosicionesDisponibles.Num() - 1);
		FVector SpawnLocation = PosicionesDisponibles[IndiceAleatorio];
		
		// Seleccionamos una de las dos fabricas
		int TipoAleatorio = FMath::RandRange(0, 1);
		FabricaAbstracta* Fabrica = nullptr;
		if (TipoAleatorio == 0)
		{
			Fabrica = &FabricaDeAzules;
		}
		else
		{
			Fabrica = &FabricaDeRojas;
		}
		// Creamos la plataforma mediante la fabrica seleccionada
		PlataformaActual = Fabrica->CrearPlataforma(World, SpawnLocation, Rotacion);		if (IsValid(PlataformaActual))
		{
			aPlataformas.Add(PlataformaActual);
		}
		PosicionesDisponibles.RemoveAt(IndiceAleatorio);
	}


	// Factory Method - Crear las otras cuatro plataformas
	AFabricaPlataformas* FabricaOtras = World->SpawnActor<AFabricaPlataformas>(
		FVector::ZeroVector, FRotator::ZeroRotator, Parametros);

	if (IsValid(FabricaOtras))
	{
		// Posiciones para cada tipo de plataforma
		FVector PosicionesExtras[4] = {
			FVector(200.0f, -850.0f, 200.0f), // Aerea - izquierda
			FVector(650.0f, -850.0f, 160.0f), // Terrestre - izquierda
			FVector(200.0f,  850.0f, 160.0f), // Subterranea - derecha
			FVector(650.0f,  850.0f, 160.0f) };  // Acuatica - derecha
		for (int Tipo = 0; Tipo < 4; Tipo++)
		{
			APlataforma* NuevaPlataforma = FabricaOtras->CrearPlataforma(Tipo, PosicionesExtras[Tipo], Rotacion);
			if (IsValid(NuevaPlataforma))
			{
				aPlataformas.Add(NuevaPlataforma);
			}
		}
	}

	/*
	// Factory Method - Crear 20 plataformas aleatorias
	AFabricaPlataformas* FabricaOtras = World->SpawnActor<AFabricaPlataformas>(
		FVector::ZeroVector, FRotator::ZeroRotator, Parametros);

	if (IsValid(FabricaOtras))
	{
		TArray<FVector> PosicionesIzquierda;
		TArray<FVector> PosicionesDerecha;

		// Posiciones disponibles a ambos lados del Paintball
		for (int X = 200; X <= 650; X += 90)
		{
			PosicionesIzquierda.Add(FVector(X, -850.0f, 160.0f));
			PosicionesIzquierda.Add(FVector(X, -1000.0f, 160.0f));

			PosicionesDerecha.Add(FVector(X, 850.0f, 160.0f));
			PosicionesDerecha.Add(FVector(X, 1000.0f, 160.0f));
		}

		// Crear 10 plataformas en cada lado
		for (int Lado = 0; Lado < 2; Lado++)
		{
			TArray<FVector>& Posiciones = (Lado == 0)
				? PosicionesIzquierda
				: PosicionesDerecha;

			for (int i = 0; i < 10; i++)
			{
				// Seleccionamos una posicion y un tipo aleatorio
				int Indice = FMath::RandRange(0, Posiciones.Num() - 1);
				int Tipo = FMath::RandRange(0, 3);

				FVector Posicion = Posiciones[Indice];

				// Las plataformas aereas tienen mayor altura
				if (Tipo == 0)
				{
					Posicion.Z = 200.0f;
				}

				// Creamos la plataforma mediante Factory Method
				APlataforma* NuevaPlataforma =
					FabricaOtras->CrearPlataforma(Tipo, Posicion, Rotacion);

				if (IsValid(NuevaPlataforma))
				{
					aPlataformas.Add(NuevaPlataforma);
				}

				// Evitamos repetir la misma posicion
				Posiciones.RemoveAt(Indice);
			}
		}
	}*


	/*antes para plataforma 5 o 20
	for (int i = 0; i < CantidadPlataformas; i++)
	{
		int IndiceAleatorio = FMath::RandRange(0,PosicionesDisponibles.Num() - 1);
		FVector SpawnLocation = PosicionesDisponibles[IndiceAleatorio];
		int TipoAleatorio = FMath::RandRange(0, 1);
		if (TipoAleatorio == 0)
		{
			PlataformaActual = World->SpawnActor<APlataformaIndestructible>(SpawnLocation,Rotacion,Parametros);
		}
		else
		{
			PlataformaActual = World->SpawnActor<APlataformaDestructible>(SpawnLocation,Rotacion,Parametros);
		}

		if (IsValid(PlataformaActual))
		{
			aPlataformas.Add(PlataformaActual);
		}

		PosicionesDisponibles.RemoveAt(IndiceAleatorio);
	}*/


	// Patrones-Singleton - intenta crear varios enemigos
	for (int i = 0; i <= 4; i++)
	{
		FVector PosicionEnemigo(200.0f, -200.0f + (i * 100.0f), 200.0f);

		AEnemigo* SpawnedEnemigo = World->SpawnActor<AEnemigo>(PosicionEnemigo,Rotacion,Parametros);

		if (SpawnedEnemigo)
		{
			if (SpawnedEnemigo->VerificarSingleton())
			{
				Enemigo = SpawnedEnemigo;

				GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("%s has been created"),*Enemigo->GetName()));
			}
		}
	}




	/* Lab4
	aPlataformas.Reserve(31);
	APlataforma* PlataformaActual;
	for (int32 i = 0; i < 10; i++)
	{
		int32 Fila = i / 5;
		int32 Columna = i % 5;
		FVector SpawnLocation(-800.0f + Columna * 400.0f, -800.0f + Fila * 400.0f, 1400.0f);
		PlataformaActual = World->SpawnActor<APlataformaAerea>(SpawnLocation, Rotacion, Parametros);
		if (IsValid(PlataformaActual))
		{
			aPlataformas.Add(PlataformaActual);
		}
	}

	for (int32 i = 0; i < 6; i++)
	{
		FVector SpawnLocation(-1000.0f + i * 400.0f, 200.0f, 100.0f);

		PlataformaActual = World->SpawnActor<APlataformaTerrestre>(SpawnLocation, Rotacion, Parametros);

		if (IsValid(PlataformaActual))
		{
			aPlataformas.Add(PlataformaActual);
		}
	}

	for (int32 i = 0; i < 4; i++)
	{
		int32 Fila = i / 2;
		int32 Columna = i % 2;

		FVector SpawnLocation(-200.0f + Columna * 400.0f, 600.0f + Fila * 400.0f, -400.0f);

		PlataformaActual = World->SpawnActor<APlataformaSubterranea>(SpawnLocation, Rotacion, Parametros);

		if (IsValid(PlataformaActual))
		{
			aPlataformas.Add(PlataformaActual);
		}
	}

	for (int32 i = 0; i < 5; i++)
	{
		FVector SpawnLocation(-500.0f + i * 400.0f, 1400.0f, 300.0f);

		PlataformaActual = World->SpawnActor<APlataformaAcuatica>(SpawnLocation, Rotacion, Parametros);

		if (IsValid(PlataformaActual))
		{
			aPlataformas.Add(PlataformaActual);
		}
	}

	for (int32 i = 0; i < 6; i++)
	{
		FVector SpawnLocation(-130.0f + i * 300.0f, -134.0f, 160.0f);

		int32 TipoAleatorio = FMath::RandRange(1, 4);

		switch (TipoAleatorio)
		{
		case 1:
			PlataformaActual = World->SpawnActor<APlataformaAerea>(SpawnLocation, Rotacion, Parametros);
			break;

		case 2:
			PlataformaActual = World->SpawnActor<APlataformaTerrestre>(SpawnLocation, Rotacion, Parametros);
			break;

		case 3:
			PlataformaActual = World->SpawnActor<APlataformaSubterranea>(SpawnLocation, Rotacion, Parametros);
			break;

		case 4:
			PlataformaActual =
				World->SpawnActor<APlataformaAcuatica>(SpawnLocation, Rotacion, Parametros);
			break;
		}

		if (IsValid(PlataformaActual))
		{
			aPlataformas.Add(PlataformaActual);
		}
	}

	GetWorldTimerManager().SetTimer(TimerMovimiento, this, &AAventuraUSFX022026L4GameMode::IniciarMovimiento, 5.0f, false);

	GetWorldTimerManager().SetTimer(TimerEliminarUnaPlataformaPorHija, this, &AAventuraUSFX022026L4GameMode::EliminarUnaPlataformaPorHija, 5.0f, true);

	GetWorldTimerManager().SetTimer(TimerReposicionarPlataformas, this, &AAventuraUSFX022026L4GameMode::ReposicionarPlataformas, 5.0f, false);
*/
}


void AAventuraUSFX022026L4GameMode::IniciarMovimiento()
{
	if (aPlataformas.Num() == 0)
	{
		return;
	}

	for (APlataforma* Plataforma : aPlataformas)
	{
		if (IsValid(Plataforma))
		{
			Plataforma->IniciarMovimiento();
		}
	}

	
}

void AAventuraUSFX022026L4GameMode::EliminarUnaPlataformaPorHija()
{
	for (int32 i = 0; i < aPlataformas.Num(); i++)
	{
		APlataformaAerea* Aerea = Cast<APlataformaAerea>(aPlataformas[i]);

		if (IsValid(Aerea))
		{
			Aerea->Destroy();
			aPlataformas.RemoveAt(i);
			break;
		}
	}

	for (int32 i = 0; i < aPlataformas.Num(); i++)
	{
		APlataformaTerrestre* Terrestre = Cast<APlataformaTerrestre>(aPlataformas[i]);

		if (IsValid(Terrestre))
		{
			Terrestre->Destroy();
			aPlataformas.RemoveAt(i);
			break;
		}
	}

	for (int32 i = 0; i < aPlataformas.Num(); i++)
	{
		APlataformaSubterranea* Subterranea = Cast<APlataformaSubterranea>(aPlataformas[i]);

		if (IsValid(Subterranea))
		{
			Subterranea->Destroy();
			aPlataformas.RemoveAt(i);
			break;
		}
	}

	for (int32 i = 0; i < aPlataformas.Num(); i++)
	{
		APlataformaAcuatica* Acuatica = Cast<APlataformaAcuatica>(aPlataformas[i]);

		if (IsValid(Acuatica))
		{
			Acuatica->Destroy();
			aPlataformas.RemoveAt(i);
			break;
		}
	}

	if (aPlataformas.Num() == 0)
	{
		GetWorldTimerManager().ClearTimer(TimerEliminarUnaPlataformaPorHija);
		GetWorldTimerManager().ClearTimer(TimerMovimiento);
		GetWorldTimerManager().ClearTimer(TimerReposicionarPlataformas);

	}
}

void AAventuraUSFX022026L4GameMode::ReposicionarPlataformas()
{
	FVector posicionAereos = FVector(-1090.0f, -1310.0f, 200.0f);
	FVector posicionTerrestres = FVector(-1030.0f, -1220.0f, 200.0f);

	for (APlataforma* plataformaActual : aPlataformas)
	{
		if (plataformaActual->TipoPlataforma == ETipoPlataforma::PLATAFORMA_AEREA)
		{
			plataformaActual->SetActorLocation(posicionAereos);
		}

		if (plataformaActual->TipoPlataforma == ETipoPlataforma::PLATAFORMA_TERRESTRE)
		{
			plataformaActual->SetActorLocation(posicionTerrestres);
		}

	}

}

