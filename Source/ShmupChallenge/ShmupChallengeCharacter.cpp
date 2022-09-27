// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShmupChallengeCharacter.h"

#include "DrawDebugHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ShmupPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AShmupChallengeCharacter

AShmupChallengeCharacter::AShmupChallengeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
	// Create a follow camera
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Top Down Camera"));
	TopDownCamera->SetupAttachment(GetRootComponent()); 
	TopDownCamera->bUsePawnControlRotation = false; 
}

void AShmupChallengeCharacter::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<AShmupPlayerController>(GetController());

	DefaultCameraDistance = FVector::Dist(TopDownCamera->GetComponentLocation(), GetActorLocation());
	TopDownCamera->SetAbsolute(true, true);
}

void AShmupChallengeCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult Hit;
	if (PC && PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
	{
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Hit.ImpactPoint);
		Rotation.Pitch = Rotation.Roll = 0;
		SetActorRotation(Rotation);
	}

	TopDownCamera->SetWorldLocation(GetActorLocation() - TopDownCamera->GetComponentRotation().Vector() * DefaultCameraDistance);
}

void AShmupChallengeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AShmupChallengeCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShmupChallengeCharacter::MoveRight);
}

void AShmupChallengeCharacter::MoveForward(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
}

void AShmupChallengeCharacter::MoveRight(float Value)
{
	AddMovementInput(FVector::RightVector, Value);
}
