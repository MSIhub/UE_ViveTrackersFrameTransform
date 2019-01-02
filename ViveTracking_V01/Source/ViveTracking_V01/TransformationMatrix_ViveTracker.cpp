// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformationMatrix_ViveTracker.h"
#include "Misc/CoreMiscDefines.h"
#include "Math/Matrix.h"
#include "Math/UnrealMathUtility.h"


// Sets default values for this component's properties
UTransformationMatrix_ViveTracker::UTransformationMatrix_ViveTracker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	FMatrix {
		ForceInitToZero
	};
	
	// ...
}


// Called when the game starts
void UTransformationMatrix_ViveTracker::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTransformationMatrix_ViveTracker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FMatrix UTransformationMatrix_ViveTracker::TransformationMatrix(FRotator InRot, FVector InTrans)
{
	/**
		PRY convention used
	*/
	float alpha = InRot.Yaw;
	float beta = InRot.Pitch;
	float gamma = InRot.Roll;
	float P_x = InTrans.X;
	float P_y = InTrans.Y;
	float P_z = InTrans.Z;

	FMatrix T_trackerToBase;
	
	//column 0
	float T00 = FMath::Cos(alpha)*FMath::Cos(beta) - FMath::Sin(alpha)*FMath::Sin(beta)*FMath::Sin(gamma);
	float T10 = FMath::Cos(beta)*FMath::Sin(alpha) + FMath::Cos(alpha)*FMath::Sin(beta)*FMath::Sin(gamma);
	float T20 = -1.f * FMath::Cos(gamma)*FMath::Sin(beta);
	float T30 = 0.f;
	
	T_trackerToBase.M[0][0] = T00;
	T_trackerToBase.M[1][0] = T10;
	T_trackerToBase.M[2][0] = T20;
	T_trackerToBase.M[3][0] = T30;

	//column 1
	float T01 = -1.f * FMath::Cos(gamma)* FMath::Sin(alpha);
	float T11 = FMath::Cos(alpha)*FMath::Cos(gamma);
	float T21 = FMath::Sin(gamma);
	float T31 = 0.f;;

	T_trackerToBase.M[0][1] = T01;
	T_trackerToBase.M[1][1] = T11;
	T_trackerToBase.M[2][1] = T21;
	T_trackerToBase.M[3][1] = T31;
	
	//column 2
	float T02 = FMath::Cos(alpha)*FMath::Sin(beta) + FMath::Cos(beta)*FMath::Sin(alpha)*FMath::Sin(gamma);
	float T12 = FMath::Sin(alpha)*FMath::Sin(beta) - FMath::Cos(alpha)*FMath::Cos(beta)*FMath::Sin(gamma);
	float T22 = FMath::Cos(beta)*FMath::Cos(gamma);
	float T32 = 0.f;

	T_trackerToBase.M[0][2] = T02;
	T_trackerToBase.M[1][2] = T12;
	T_trackerToBase.M[2][2] = T22;
	T_trackerToBase.M[3][2] = T32;
	
	//column 3
	float T03 = P_x;
	float T13 = P_y;
	float T23 = P_z;
	float T33 = 1.f;

	T_trackerToBase.M[0][3] = T03;
	T_trackerToBase.M[1][3] = T13;
	T_trackerToBase.M[2][3] = T23;
	T_trackerToBase.M[3][3] = T33;

	return T_trackerToBase;
}


