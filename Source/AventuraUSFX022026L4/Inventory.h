// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.generated.h"

UCLASS()
class AVENTURAUSFX022026L4_API AInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInventory();

	// Malla para visualizar el Inventory en el nivel
	UPROPERTY(VisibleAnywhere)class UStaticMeshComponent* MallaInventory;

	// La instancia de esta clase,Este puntero guardará la dirección del Inventory que ya existe.
	UPROPERTY()AInventory* Instance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
