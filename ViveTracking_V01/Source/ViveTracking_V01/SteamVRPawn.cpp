// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamVRPawn.h"

// Sets default values
ASteamVRPawn::ASteamVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASteamVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASteamVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASteamVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ASteamVRPawn::AngleOffsetCorrector(float InAngle)
{
	if (InAngle > 180.0f)
	{
		InAngle = InAngle - 360.0f;
	}

	return InAngle;
}

