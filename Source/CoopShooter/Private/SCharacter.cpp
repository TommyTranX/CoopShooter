// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	ZoomedFOV = 65.0f;


}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	DefaultFOV = CameraComp -> FieldOfView;

	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;


	CameraComp->SetFieldOfView(CurrentFOV);
    

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent ->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent ->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent ->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent ->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (CameraComp){
		return CameraComp -> GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ASCharacter::MoveForward(float Value)
{
	
	AddMovementInput(GetActorForwardVector() * Value);
}


void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}


void ASCharacter::BeginCrouch()
{
	 
	Crouch();
}



void ASCharacter::EndCrouch()
{
	
	UnCrouch();
}

void ASCharacter::BeginZoom()
{
	
	bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
	
	bWantsToZoom = false;
}

