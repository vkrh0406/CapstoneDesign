// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());

	
	// 카메라 위치를 눈 살짝 위쪽으로 잡습니다.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	// 폰의 카메라 로테이션 제어를 허용합니다.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// 일인칭 메시 컴포넌트입니다.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	// 소유 플레이어만 이 메시를 볼 수 있습니다.
	FPSMesh->SetOnlyOwnerSee(true);
	// FPS 메시를 FPS 카메라에 붙입니다.
	FPSMesh->SetupAttachment(FPSCameraComponent);
	// 일부 환경 섀도잉을 꺼 메시가 하나인 듯 보이는 느낌을 유지합니다.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	
	
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetOwnerNoSee(true);
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookRight", this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJump);
}

void AMyCharacter::MoveForward(float Value)
{
	// 어느 쪽이 전방인지 알아내어, 플레이어가 그 방향으로 이동하고자 한다고 기록합니다.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::MoveRight(float Value)
{
	// 어느 쪽이 전방인지 알아내어, 플레이어가 그 방향으로 이동하고자 한다고 기록합니다.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::StartJump()
{
	bPressedJump = true;
}
void AMyCharacter::StopJump()
{
	bPressedJump = false;
}