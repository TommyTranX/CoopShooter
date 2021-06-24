// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SWeapon.h"

#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ASWeapon;

//ACharacter
// are Pawns that have a mesh, collision, and built-in movement logic. They are responsible for all physical interaction between the player or AI and the world, and also implement basic networking and input models. They are designed for a vertically-oriented player representation that can walk, jump, fly, and swim through the world using CharacterMovementComponent.

UCLASS()//macro is used to indicate that a C++ class will be part of Unreal's Reflection system. This is necessary for the C++ class to be recognized by the Unreal Engine editor
class COOPSHOOTER_API ASCharacter : public ACharacter //AS Character class inherit from public elements of ACharacter class
{
	GENERATED_BODY() //The GENERATED_BODY macro takes no arguments, but sets up the class to support the infrastructure required by the engine. It is required for all UCLASSes.

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") //VisibleAnywhere: Indicates that this property is visible in all property windows, but cannot be edited. This Specifier is incompatible with the "Edit" Specifiers. | BlueprintReadOnly: This property can be read by Blueprints, but not modified. This Specifier is incompatible with the BlueprintReadWrite Specifier | Category: Specifies the category of the property when displayed in Blueprint editing tools. Define nested categories using the | operator. 
	UCameraComponent* CameraComp; //Represents a camera viewpoint and settings, such as projection type, field of view, and post-process overrides. The default behavior for an actor used as the camera view target is to look for an attached camera component and use its location, rotation, and settings.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp; //Spring Arm Components provide functionality to your camera so that it can expand or retract based on gameplay situations.

	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player") //EditsDefaultOnly: Indicates that this property can be edited by property windows, but only on archetypes. This Specifier is incompatible with any of the "Visible" Specifiers.
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;

	
	float DefaultFOV;

	void BeginZoom();

	void EndZoom();

	ASWeapon* CurrentWeapon;

	void StartFire();

	void EndFire();

	UPROPERTY(EditDefaultsOnly, Category= "Player")
	TSubclassOf<ASWeapon> StarterWeaponClass; // Template class that tells the editor's property windows to list only classes derived from ASWeapon as choices for the property.

	UPROPERTY(VisibleDefaultsOnly, Category= "Player")
	FName WeaponAttachSocketName; //When you name a new asset in the Content Browser, change a parameter in a Dynamic Material Instance, or access a bone in a Skeletal Mesh, you are using FNames.

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

};
