#include "Characters/Michelle.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Item.h"
#include "Animation/AnimMontage.h"


AMichelle::AMichelle()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
}


void AMichelle::BeginPlay()
{
	Super::BeginPlay();
}


void AMichelle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMichelle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AMichelle::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AMichelle::MoveRight);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AMichelle::LookUp);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &AMichelle::LookRight);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &AMichelle::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &AMichelle::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AMichelle::Attack);
}


void AMichelle::MoveForward(float value) {

	if (ActionState != EActionState::ECS_Unoccupied) { return; }

	if (Controller && (value != 0)) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}


void AMichelle::MoveRight(float value) {

	if (ActionState != EActionState::ECS_Unoccupied) { return; }

	if (Controller && (value != 0)) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}


void AMichelle::LookUp(float value) {
	if (Controller && (value != 0)) {
		AddControllerPitchInput(value);
	}
}


void AMichelle::LookRight(float value) {
	if (Controller && (value != 0)) {
		AddControllerYawInput(value);
	}
}


void AMichelle::EKeyPressed()
{
	AWeapon* OverLappingWeapon = Cast<AWeapon>(OverLappingItem);
	if (OverLappingWeapon)
	{
		OverLappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverLappingItem = nullptr;
		EquippedWeapon = OverLappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquippedMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::ECS_EquippingWeapon;
		}
		else if (CanArm())
		{
			PlayEquippedMontage(FName("equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::ECS_EquippingWeapon;
		}
	}
}


void AMichelle::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Play(AttackMontage);

		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName;

		switch (Selection) {
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}


bool AMichelle::CanAttack()
{
	return ActionState == EActionState::ECS_Unoccupied &&
		   CharacterState != ECharacterState::ECS_Unequipped;
}


void AMichelle::Attack() {

	if (CanAttack()){
		PlayAttackMontage();
		ActionState = EActionState::ECS_Attacking;
	}
}


void AMichelle::AttackEnd()
{
	ActionState = EActionState::ECS_Unoccupied;
}


void AMichelle::PlayEquippedMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool AMichelle::CanDisarm()
{
	return ActionState == EActionState::ECS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
}

bool AMichelle::CanArm()
{
	return ActionState == EActionState::ECS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped &&
		EquippedWeapon;
}

void AMichelle::Disarm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void AMichelle::Arm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void AMichelle::FinishEquipping()
{
	ActionState = EActionState::ECS_Unoccupied;
}
