// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FabricaAbstracta.h"


class AVENTURAUSFX022026L4_API FabricaAzul : public FabricaAbstracta
{
public:
	FabricaAzul();
	~FabricaAzul();

	// Sobrescribe el metodo para crear plataformas indestructibles
	APlataforma* CrearPlataforma(UWorld* World, const FVector& Posicion, const FRotator& Rotacion) override;

	//Sobrescribe el metodo para crear pelotas pequeñas
	AAventuraUSFX022026L4Projectile* CrearPelota(UWorld* World, const FVector& Posicion, const FRotator& Rotacion) override;
};
