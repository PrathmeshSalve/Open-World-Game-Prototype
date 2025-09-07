#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterTypes.h"
#include "MichelleAnimInstance.generated.h"

class AMichelle;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class OPENWORLDFUN_API UMichelleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	UPROPERTY(BlueprintReadWrite)
	AMichelle* Michelle;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* MichelleMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character state")
	ECharacterState CharacterState;
};