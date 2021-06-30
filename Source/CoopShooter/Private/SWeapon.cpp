// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopShooter/CoopShooter.h"


// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));//SkeletalMeshComponent is used to create an instance of an animated SkeletalMesh asset.


	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

}

// Called when the game starts or when spawned


void ASWeapon::Fire(){
	//trace from pawn eyes to central location

	AActor* MyOwner = GetOwner(); //Actor is the base class for an Object that can be placed or spawned in a level. Actors may contain a collection of ActorComponents, which can be used to control how actors move, how they are rendered, etc. The other main function of an Actor is the replication of properties and function calls across the network during play.
	//GetOwner(): Actor is the base class for an Object that can be placed or spawned in a level. Actors may contain a collection of ActorComponents, which can be used to control how actors move, how they are rendered, etc. The other main function of an Actor is the replication of properties and function calls across the network during play.
	if (MyOwner){
		FVector EyeLocation;
		FRotator EyeRotation;//Implements a container for rotation information. All rotation values are stored in degrees.
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); //passed by ref so it will fill the variables.
		//GetActorEyesViewPoint: Returns the point of view of the actor. Note that this doesn't mean the camera, but the 'eyes' of the actor. For example, for a Pawn, this would define the eye height location, and view rotation (which is different from the pawn rotation which has a zeroed pitch component). A camera first person view will typically use this view point. Most traces (weapon, AI) will be done from this view point.

		FVector TraceEnd = EyeLocation + (EyeRotation.Vector()*10000);//Convert a rotation into a unit vector facing in its direction.
		FVector ShotDirection = EyeRotation.Vector();

		FCollisionQueryParams QueryParams;//Structure that defines parameters passed into collision function
		QueryParams.AddIgnoredActor(MyOwner);//Add a collection of actors for this trace to ignore
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;//Whether we should trace against complex collision
		QueryParams.bReturnPhysicalMaterial = true;


		FVector TracerEndPoint = TraceEnd; //particle "Target" parameter
		FHitResult Hit; //Ctor for easily creating "fake" hits from limited data.

		//GetWorld() return world which that object belongs to,
		//LineTraceSingleByChannel(): Trace a ray against the world using a specific channel and return the first blocking hit
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams)){
			//hits something. See here for more on parameters: https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Engine/UWorld/LineTraceSingleByChannel/
			AActor* HitActor = Hit.GetActor();
			//UGameplayStatics: Static class with useful gameplay utility functions that can be called from both Blueprint and C++
			//ApplyPointDamage: Hurts the specified actor with the specified impact.
			UGameplayStatics::ApplyPointDamage(HitActor, 20, ShotDirection, Hit, MyOwner -> GetInstigatorController(), this, DamageType );

			

			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());


			UParticleSystem* SelectedEffect = nullptr;
			switch (SurfaceType)
			{
			case SURFACE_FLESHDEFAULT:
			case SURFACE_FLESHVULNERABLE:
				SelectedEffect = FleshImpactEffect;
				break;
			
			default:
				SelectedEffect = DefaultImpactEffect;
				break;
			}
			if (SelectedEffect){
			//SpawnEmitterAtLocation; Plays the specified effect at the given location and rotation, fire and forget.
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}

		}
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
		PlayFireEffect(TracerEndPoint);

		

	}
	
}

void ASWeapon::PlayFireEffect(FVector TraceEnd){
	if (MuzzleEffect){
		//Plays the specified effect attached to and following the specified component.
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
		
		}
		
		if (TracerEffect){
			//Get world-space socket or bone location.
			FVector MuzzleLocation = MeshComp-> GetSocketLocation(MuzzleSocketName);
			//A particle emitter.
			UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
			if (TracerComp){
				//Set Vector Parameter Value
				TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);

			}
		}

	//Pawn is the base class of all actors that can be possessed by players or AI.
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{	//PlayerControllers are used by human players to control Pawns.
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}

// Called every frame



