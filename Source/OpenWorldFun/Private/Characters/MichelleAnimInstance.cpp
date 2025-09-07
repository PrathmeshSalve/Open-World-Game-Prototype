#include "Characters/MichelleAnimInstance.h"
#include "Characters/Michelle.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UMichelleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Michelle = Cast<AMichelle>(TryGetPawnOwner());
	if (Michelle) {
		MichelleMovement = Michelle->GetCharacterMovement();
	}
}


void UMichelleAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (MichelleMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(MichelleMovement->Velocity);
		IsFalling = MichelleMovement->IsFalling();
		CharacterState = Michelle->GetCharacterState();
	}
}
