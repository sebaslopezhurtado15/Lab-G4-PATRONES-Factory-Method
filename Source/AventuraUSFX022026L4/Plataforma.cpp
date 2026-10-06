// LABORATORIO 04 - SIS457
// GRUPO 4
// Plataformas con movimientos independientes y eliminacion aleatoria

// Fill out your copyright notice in the Description page of Project Settings.

#include "Plataforma.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
//Factory Method
#include "PlataformaDestructible.h"
#include "PlataformaIndestructible.h"
#include "Engine/World.h"

// Sets default values
APlataforma::APlataforma()
{
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlataformaMeshAsset(
		TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Plane.Shape_Plane'")
	);

	mallaPlataforma =
		CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaPlataforma"));
	RootComponent = mallaPlataforma;

	if (PlataformaMeshAsset.Succeeded())
	{
		mallaPlataforma->SetStaticMesh(PlataformaMeshAsset.Object);
	}
	mallaPlataforma->SetMobility(EComponentMobility::Movable);

	//perfil de colisión BlockAll
	mallaPlataforma->SetCollisionProfileName(TEXT("BlockAll"));

	// Valores aleatorios de plataforma
	movimientoLimitesMaximos = FVector(
		FMath::RandRange(200.0f, 400.0f),
		FMath::RandRange(200.0f, 400.0f),
		FMath::RandRange(200.0f, 400.0f)
	);

	movimientoLimitesMinimos = FVector(
		FMath::RandRange(-400.0f, -200.0f),
		FMath::RandRange(-400.0f, -200.0f),
		FMath::RandRange(-400.0f, -200.0f)
	);

	movimientoVelocidades = FVector(
		FMath::RandRange(50.0f, 200.0f),
		FMath::RandRange(50.0f, 200.0f),
		FMath::RandRange(50.0f, 200.0f)
	);

	movimientoDireccion = FVector(
		FMath::RandRange(0, 1) == 0 ? -1.0f : 1.0f,
		FMath::RandRange(0, 1) == 0 ? -1.0f : 1.0f,
		FMath::RandRange(0, 1) == 0 ? -1.0f : 1.0f
	);

	movimientoEjes = FVector(1.0f, 0.0f, 0.0f);


	bMover = false;
}


//Factory Method
APlataforma* APlataforma::CrearPlataforma(UWorld* World,int Tipo,const FVector& Posicion,const FRotator& Rotacion)
{
	if (World == nullptr)
	{
		return nullptr;
	}
	FActorSpawnParameters Parametros;
	Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (Tipo == 0)
	{
		return World->SpawnActor<APlataformaIndestructible>(Posicion,Rotacion,Parametros);
	}
	else
	{
		return World->SpawnActor<APlataformaDestructible>(Posicion,Rotacion,Parametros);
	}
}

void APlataforma::BeginPlay()
{
	Super::BeginPlay();


	posicionInicial = GetActorLocation();
}

void APlataforma::ConfigurarMovimiento(int32 Tipo)
{
	// 10 combinaciones
	FVector TiposMovimiento[10] =
	{
		FVector(1.0f, 0.0f, 0.0f),
		FVector(0.0f, 1.0f, 0.0f),
		FVector(0.0f, 0.0f, 1.0f),

		FVector(1.0f, 1.0f, 0.0f),
		FVector(1.0f, 0.0f, 1.0f),
		FVector(0.0f, 1.0f, 1.0f),

		FVector(1.0f, 1.0f, 1.0f),

		FVector(1.0f, 0.5f, 0.0f),
		FVector(0.5f, 1.0f, 0.5f),
		FVector(1.0f, 0.5f, 1.0f)
	};

	movimientoEjes = TiposMovimiento[Tipo % 10];
}

void APlataforma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (!bMover)
	{
		return;
	}

	posicionActual = GetActorLocation();

	FVector Diferencia = posicionActual - posicionInicial;


	if (Diferencia.X >= movimientoLimitesMaximos.X)
	{
		movimientoDireccion.X = -1.0f;
	}
	else if (Diferencia.X <= movimientoLimitesMinimos.X)
	{
		movimientoDireccion.X = 1.0f;
	}

	if (Diferencia.Y >= movimientoLimitesMaximos.Y)
	{
		movimientoDireccion.Y = -1.0f;
	}
	else if (Diferencia.Y <= movimientoLimitesMinimos.Y)
	{
		movimientoDireccion.Y = 1.0f;
	}

	if (Diferencia.Z >= movimientoLimitesMaximos.Z)
	{
		movimientoDireccion.Z = -1.0f;
	}
	else if (Diferencia.Z <= movimientoLimitesMinimos.Z)
	{
		movimientoDireccion.Z = 1.0f;
	}

	FVector posicionNueva = posicionActual + (movimientoEjes *movimientoDireccion *movimientoVelocidades *DeltaTime);

	SetActorLocation(posicionNueva);
}

void APlataforma::IniciarMovimiento()
{
	bMover = true;
}

void APlataforma::DetenerMovimiento()
{
	bMover = false;
}