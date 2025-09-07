#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/CharacterTypes.h"
#include "Michelle.generated.h"


class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class AWeapon;


UCLASS()
class OPENWORLDFUN_API AMichelle : public ACharacter
{
	GENERATED_BODY()

public:
	AMichelle();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	//Callbacks for inputs
	void MoveForward(float value);
	void MoveRight(float value);
	void LookRight(float value);
	void LookUp(float value);
	void EKeyPressed();
	void Attack();

	//Play montage functions
	void PlayAttackMontage();
	
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool CanAttack();

	void PlayEquippedMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

private:
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess))
	EActionState ActionState = EActionState::ECS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverLappingItem;

	UPROPERTY(VisibleInstanceOnly, Category = Weapon)
	AWeapon* EquippedWeapon;

	//Attack Animation Montage
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackMontage;

	//Equip Animation Montage
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipMontage;

public:
	FORCEINLINE void SetOverLappingItem(AItem* Item) { OverLappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
