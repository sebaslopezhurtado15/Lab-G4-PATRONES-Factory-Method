// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
//GameplayStatics para poder usar funciones útiles de Unreal
#include "Kismet/GameplayStatics.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creamos el componente de malla para poder ver el Inventory
	MallaInventory = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaInventory"));
	RootComponent = MallaInventory;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cubo(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cubo.Succeeded())
	{
		MallaInventory->SetStaticMesh(Cubo.Object);
	}

	// Busca las instancias existentes de esta clase
	TArray<AActor*> Instances;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AInventory::StaticClass(),Instances);
	if (Instances.Num() > 1)
	{
		// Ya existe otro Inventory, guardamos el primero encontrado
		Instance = Cast<AInventory>(Instances[0]);
		GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Yellow,FString::Printf(TEXT("%s already exists"),*Instance->GetName()));
		// Destruye este nuevo Inventory
		Destroy();
	}

}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

