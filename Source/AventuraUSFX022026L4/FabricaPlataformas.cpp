// Fill out your copyright notice in the Description page of Project Settings.


#include "FabricaPlataformas.h"

#include "Plataforma.h"
#include "PlataformaIndestructible.h"
#include "PlataformaDestructible.h"
#include "Engine/World.h"


// Sets default values
AFabricaPlataformas::AFabricaPlataformas()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFabricaPlataformas::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFabricaPlataformas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Crea una plataforma según el tipo, posición y rotación recibidos.
APlataforma* AFabricaPlataformas::CrearPlataforma(int Tipo, const FVector& Posicion, const FRotator& Rotacion)
{
    UWorld* World = GetWorld();
    if (World == nullptr)
    {
        return nullptr;
    }
    FActorSpawnParameters Parametros;
    Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    if (Tipo == 0)
    {
        return World->SpawnActor<APlataformaIndestructible>(Posicion, Rotacion, Parametros);
    }
    else if (Tipo == 1)
    {
        return World->SpawnActor<APlataformaDestructible>(Posicion, Rotacion, Parametros);
    }
    return nullptr;
}
