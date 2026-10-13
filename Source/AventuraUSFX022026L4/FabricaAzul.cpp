// Fill out your copyright notice in the Description page of Project Settings.


#include "FabricaAzul.h"

#include "PlataformaIndestructible.h"
#include "PelotaPequena.h"
#include "Engine/World.h"

FabricaAzul::FabricaAzul()
{
}

FabricaAzul::~FabricaAzul()
{
}

// Sobrescribimos el metodo para crear plataformas indestructibles
APlataforma* FabricaAzul::CrearPlataforma(UWorld* World, const FVector& Posicion, const FRotator& Rotacion)
{
    if (World == nullptr)
    {
        return nullptr;
    }

    FActorSpawnParameters Parametros;
    Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<APlataformaIndestructible>(Posicion, Rotacion, Parametros);
}

// Sobrescribimos el metodo para crear pelotas pequeñas
AAventuraUSFX022026L4Projectile* FabricaAzul::CrearPelota(UWorld* World, const FVector& Posicion, const FRotator& Rotacion)
{
    if (World == nullptr)
    {
        return nullptr;
    }
    return World->SpawnActor<APelotaPequena>(Posicion, Rotacion);
}