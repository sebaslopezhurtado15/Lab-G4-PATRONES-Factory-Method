// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemigo.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AEnemigo::AEnemigo()
{
	PrimaryActorTick.bCanEverTick = true;

	// Malla para visualizar al enemigo
	MallaEnemigo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MallaEnemigo"));
	MallaEnemigo->SetupAttachment(RootComponent);

	// Malla del enemigo
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Malla(TEXT("/Game/StarterContent/Props/SM_Statue.SM_Statue"));

	if (Malla.Succeeded())
	{
		MallaEnemigo->SetStaticMesh(Malla.Object);

		// Escala 3
		MallaEnemigo->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	}

	
}

// Called when the game starts or when spawned
void AEnemigo::BeginPlay()
{
	Super::BeginPlay();
}

bool AEnemigo::VerificarSingleton()
{
	// Busca los enemigos existentes
	TArray<AActor*> Instances;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemigo::StaticClass(), Instances);

	if (Instances.Num() > 1)
	{
		// Ya existe otro enemigo, guardamos el primero
		Instance = Cast<AEnemigo>(Instances[0]);

		GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,FString::Printf(TEXT("%s already exists"), *Instance->GetName()));

		// Destruye este nuevo enemigo
		Destroy();

		return false;
	}

	return true;
}


// Called every frame
void AEnemigo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemigo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

