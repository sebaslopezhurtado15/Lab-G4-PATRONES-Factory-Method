// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Declaracion adelantada
class UWorld;
class APlataforma;
class AAventuraUSFX022026L4Projectile;


class AVENTURAUSFX022026L4_API FabricaAbstracta
{
public:
	FabricaAbstracta();

	virtual ~FabricaAbstracta(); 

	//método virtual puro
	virtual APlataforma* CrearPlataforma(UWorld* World,const FVector& Posicion,const FRotator& Rotacion) = 0;
	virtual AAventuraUSFX022026L4Projectile* CrearPelota(UWorld* World, const FVector& Posicion, const FRotator& Rotacion) = 0;
};
