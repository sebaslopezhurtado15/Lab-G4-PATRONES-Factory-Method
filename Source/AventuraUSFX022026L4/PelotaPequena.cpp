// Fill out your copyright notice in the Description page of Project Settings.

#include "PelotaPequena.h"
#include "Components/StaticMeshComponent.h"


//Constructor 
APelotaPequena::APelotaPequena()
{
    // Reducimos el tamaño de la pelota heredada
    GetProjectileMesh()->SetRelativeScale3D(FVector(0.7f, 0.7f, 0.7f));
}
