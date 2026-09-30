// Fill out your copyright notice in the Description page of Project Settings.


#include "Singleton_Main.h"
#include "Inventory.h"

// Sets default values
ASingleton_Main::ASingleton_Main()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASingleton_Main::BeginPlay()
{
	Super::BeginPlay();

	/* Creamos 4 Inventory
	for (int i = 0; i <= 4; i++)
	{
		//Creo un puntero llamado SpawnedInventory que puede apuntar a un objeto AInventory
		AInventory* SpawnedInventory = GetWorld()->SpawnActor<AInventory>(AInventory::StaticClass());
		if (SpawnedInventory)
		{
			// Si el inventario se creó correctamente, guarda ese inventario creado en la variable Inventory
			Inventory = SpawnedInventory;
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Yellow,FString::Printf(TEXT("%s has been created"),*Inventory->GetName()));
		}
	}*/

	//Creamos 4 Inventory dentro de la caja
	for (int i = 0; i <= 4; i++)
	{
		FVector PosicionInventory(200.0f, 0.0f, 200.0f);
		//Creo un puntero llamado SpawnedInventory que puede apuntar a un objeto AInventory y Obtiene la posición donde tú colocaste Singleton_Main.
		AInventory* SpawnedInventory = GetWorld()->SpawnActor<AInventory>(AInventory::StaticClass(),PosicionInventory,FRotator::ZeroRotator);
		if (SpawnedInventory)
		{
			// Si el inventario se creó correctamente, guarda ese inventario creado en la variable Inventory
			Inventory = SpawnedInventory;
			GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Yellow,FString::Printf(TEXT("%s has been created"),*Inventory->GetName()));
		}
	}
	

}

// Called every frame
void ASingleton_Main::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

