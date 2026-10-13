// Fill out your copyright notice in the Description page of Project Settings.


#include "FabricaRoja.h"

#include "PlataformaDestructible.h"
#include "AventuraUSFX022026L4Projectile.h"
#include "Engine/World.h"


FabricaRoja::FabricaRoja()
{
}

FabricaRoja::~FabricaRoja()
{
}

// Sobrescribimos el metodo para crear plataformas destructibles
APlataforma* FabricaRoja::CrearPlataforma(UWorld* World, const FVector& Posicion, const FRotator& Rotacion)
{
    if (World == nullptr)
    {
        return nullptr;
    }

    FActorSpawnParameters Parametros;
    Parametros.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    return World->SpawnActor<APlataformaDestructible>(Posicion, Rotacion, Parametros);
}

// Sobrescribimos el metodo para crear pelotas normales
AAventuraUSFX022026L4Projectile* FabricaRoja::CrearPelota(UWorld* World, const FVector& Posicion, const FRotator& Rotacion)
{
    if (World == nullptr)
    {
        return nullptr;
    }
    return World->SpawnActor<AAventuraUSFX022026L4Projectile>(Posicion, Rotacion);
}
