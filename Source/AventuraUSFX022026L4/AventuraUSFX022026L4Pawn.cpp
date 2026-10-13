// Copyright Epic Games, Inc. All Rights Reserved.

#include "AventuraUSFX022026L4Pawn.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Materials/Material.h"

// Patron Abstract Factory
#include "FabricaAbstracta.h"
#include "FabricaRoja.h"
#include "FabricaAzul.h"

const FName AAventuraUSFX022026L4Pawn::MoveForwardBinding("MoveForward");
const FName AAventuraUSFX022026L4Pawn::MoveRightBinding("MoveRight");
const FName AAventuraUSFX022026L4Pawn::FireForwardBinding("FireForward");
const FName AAventuraUSFX022026L4Pawn::FireRightBinding("FireRight");

AAventuraUSFX022026L4Pawn::AAventuraUSFX022026L4Pawn()
{	
	//Pimball
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	//detecta ek overlap
	ShipMeshComponent->SetGenerateOverlapEvents(true);

	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);

	// Material negro del Pawn paimball
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialNegro(TEXT("Material'/Game/MaterialesPaintball/M_PawNegra.M_PawNegra'"));
	if (MaterialNegro.Succeeded())
	{
		ShipMeshComponent->SetMaterial(0, MaterialNegro.Object);
	}

	//Pimball
	ShipMeshComponent->SetRelativeScale3D(FVector(0.5f, 2.5f, 0.5f));

	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Centra la camara en el mini escenario
	CameraBoom->TargetOffset = FVector(400.0f, 0.0f, 0.0f);

	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when ship does

	//camara paimball
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// nace proyectil
	GunOffset = FVector(180.f, 0.f, 0.f);
	FireRate = 0.1f;
	bCanFire = true;
}

void AAventuraUSFX022026L4Pawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AAventuraUSFX022026L4Pawn::Tick(float DeltaSeconds)
{
	//pimball solo der-izq
	const float RightValue = GetInputAxisValue(MoveRightBinding);
	const FVector MoveDirection = FVector(0.0f, RightValue, 0.0f);
	 
 
	/*mov antiguo paw
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.0f).GetClampedToMaxSize(1.0f);
	*/

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		//paimball
		const FRotator NewRotation = GetActorRotation();

		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
	
	
	const float FireForwardValue = GetInputAxisValue(FireForwardBinding);
	const float FireRightValue = GetInputAxisValue(FireRightBinding);
	// Disparo diagonal con < o >
	const FVector FireDirection = FVector(FMath::Abs(FireRightValue), FireRightValue, 0.0f);

	/* antigua < o >
	const FVector FireDirection = FVector(FireForwardValue, FireRightValue, 0.f);
	*/

	// Try and fire a shot
	FireShot(FireDirection);
}

void AAventuraUSFX022026L4Pawn::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			//tipo FabricaAbstracta <- o -> 
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// Creamos las dos fabricas concretas
				FabricaRoja FabricaDeRojas;
				FabricaAzul FabricaDeAzules;
				FabricaAbstracta* Fabrica = nullptr;
				if (FireDirection.Y < 0.0f)
				{
					Fabrica = &FabricaDeAzules;
				}
				else
				{
					Fabrica = &FabricaDeRojas;
				}
				// Creamos la pelota mediante Abstract Factory
				Fabrica->CrearPelota(World, SpawnLocation, FireRotation);
			}


			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAventuraUSFX022026L4Pawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AAventuraUSFX022026L4Pawn::ShotTimerExpired()
{
	bCanFire = true;
}

