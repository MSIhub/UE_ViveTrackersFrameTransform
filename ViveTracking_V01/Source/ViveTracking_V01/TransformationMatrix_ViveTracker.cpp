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
		T = [A00 A01 A02 Px
			 A10 A11 A12 Py
			 A20 A21 A22 Pz
			 0   0   0   1]

	*/
	// Euler angle: conversion to radians
	float roll = InRot.Roll*(PI/180);
	float pitch = InRot.Pitch*(PI / 180);
	float yaw = InRot.Yaw*(PI / 180);
	
	// Position 
	float Px = InTrans.X;
	float Py = InTrans.Y;
	float Pz = InTrans.Z;

	/*YPR convention  intrinsic Convention 3D Rotation matrix 
	 %Intrinsic rotation notion used [ZYX of intrinsic = XYZ of extrinsic] */

	//COLUMN0
	float A00 = FMath::Cos(pitch)*FMath::Cos(yaw);
	float A10 = (FMath::Cos(roll)*FMath::Sin(yaw)) + (FMath::Cos(yaw)*FMath::Sin(pitch)*FMath::Sin(roll));
	float A20 = (FMath::Sin(roll)*FMath::Sin(yaw)) - (FMath::Cos(roll)*FMath::Cos(yaw)*FMath::Sin(pitch));
	//COLUMN1
	float A01 = -FMath::Cos(pitch)*FMath::Sin(yaw);
	float A11 = (FMath::Cos(roll)*FMath::Cos(yaw)) - (FMath::Sin(pitch)*FMath::Sin(roll)*FMath::Sin(yaw));
	float A21 = (FMath::Cos(yaw)*FMath::Sin(roll)) + (FMath::Cos(roll)*FMath::Sin(pitch)*FMath::Sin(yaw));
	//COLUMN2
	float A02 = FMath::Sin(pitch);
	float A12 = -FMath::Cos(pitch)*FMath::Sin(roll);
	float A22 = FMath::Cos(pitch)*FMath::Cos(roll);

	FMatrix T_trackerToBase;
	
	//COLUMN0
	T_trackerToBase.M[0][0] = A00;
	T_trackerToBase.M[1][0] = A10;
	T_trackerToBase.M[2][0] = A20;
	T_trackerToBase.M[3][0] = 0.f;
	//COLUMN1	
	T_trackerToBase.M[0][1] = A01;
	T_trackerToBase.M[1][1] = A11;
	T_trackerToBase.M[2][1] = A21;
	T_trackerToBase.M[3][1] = 0.f;
	//COLUMN2
	T_trackerToBase.M[0][2] = A02;
	T_trackerToBase.M[1][2] = A12;
	T_trackerToBase.M[2][2] = A22;
	T_trackerToBase.M[3][2] = 0.f;
	//COLUMN3
	T_trackerToBase.M[0][3] = Px;
	T_trackerToBase.M[1][3] = Py;
	T_trackerToBase.M[2][3] = Pz;
	T_trackerToBase.M[3][3] = 1.f;

	return T_trackerToBase;
}

FMatrix UTransformationMatrix_ViveTracker::TransformationMatrix_Inverse(FMatrix InTransformationMatrix)
{
	/**
		Khalil notation followed
		T_inv = [transpose(A) -transpose(A)*P
				 000		  1]

		T_inv = [A00 A10 A20 -A00*Px-A10*Py-A20*Pz
				 A01 A11 A21 -A01*Px-A11*Py-A21*Pz
				 A02 A12 A22 -A02*Px-A12*Py-A22*Pz
				 0   0   0   1 ]
	*/
	FMatrix T_Inv_trackerToBase;

	float A00 = InTransformationMatrix.M[0][0];
	float A10 = InTransformationMatrix.M[1][0];
	float A20 = InTransformationMatrix.M[2][0];

	float A01 = InTransformationMatrix.M[0][1];
	float A11 = InTransformationMatrix.M[1][1];
	float A21 = InTransformationMatrix.M[2][1];

	float A02 = InTransformationMatrix.M[0][2];
	float A12 = InTransformationMatrix.M[1][2];
	float A22 = InTransformationMatrix.M[2][2];
	
	float Px = InTransformationMatrix.M[0][3];
	float Py = InTransformationMatrix.M[1][3];
	float Pz = InTransformationMatrix.M[2][3];

	//COLUMN0
	T_Inv_trackerToBase.M[0][0] = A00;
	T_Inv_trackerToBase.M[1][0] = A01;
	T_Inv_trackerToBase.M[2][0] = A02;
	T_Inv_trackerToBase.M[3][0] = 0.f;
	//COLUMN1
	T_Inv_trackerToBase.M[0][1] = A10;
	T_Inv_trackerToBase.M[1][1] = A11;
	T_Inv_trackerToBase.M[2][1] = A12;
	T_Inv_trackerToBase.M[3][1] = 0.f;
	//COLUMN2
	T_Inv_trackerToBase.M[0][2] = A20;
	T_Inv_trackerToBase.M[1][2] = A21;
	T_Inv_trackerToBase.M[2][2] = A22;
	T_Inv_trackerToBase.M[3][2] = 0.f;
	//COLUMN3
	T_Inv_trackerToBase.M[0][3] = -(A00*Px)-(A10*Py)-(A20*Pz);
	T_Inv_trackerToBase.M[1][3] = -(A01*Px)-(A11*Py)-(A21*Pz);
	T_Inv_trackerToBase.M[2][3] = -(A02*Px)-(A12*Py)-(A22*Pz);
	T_Inv_trackerToBase.M[3][3] = 1.f;

	return T_Inv_trackerToBase;
}

FMatrix UTransformationMatrix_ViveTracker::TransformationMatrix_Multiply(FMatrix InTransformationMatrix1, FMatrix InTransformationMatrix2)
{
	/**
		K = I*J;
		//ROW0
		K00 = (I00*J00)+(I01*J10)+(I02*J20)+(I03*J30);
		K01 = (I00*J01)+(I01*J11)+(I02*J21)+(I03*J31);
		K02 = (I00*J02)+(I01*J12)+(I02*J22)+(I03*J32);
		K03 = (I00*J03)+(I01*J13)+(I02*J23)+(I03*J33);
		//ROW1
		K10 = (I10*J00)+(I11*J10)+(I12*J20)+(I13*J30);
		K11 = (I10*J01)+(I11*J11)+(I12*J21)+(I13*J31);
		K12 = (I10*J02)+(I11*J12)+(I12*J22)+(I13*J32);
		K13 = (I10*J03)+(I11*J13)+(I12*J23)+(I13*J33);
		//ROW2
		K20 = (I20*J00)+(I21*J10)+(I22*J20)+(I23*J30);
		K21 = (I20*J01)+(I21*J11)+(I22*J21)+(I23*J31);
		K22 = (I20*J02)+(I21*J12)+(I22*J22)+(I23*J32);
		K23 = (I20*J03)+(I21*J13)+(I22*J23)+(I23*J33);
		//ROW3
		K30 = (I30*J00)+(I31*J10)+(I32*J20)+(I33*J30);
		K31 = (I30*J01)+(I31*J11)+(I32*J21)+(I33*J31);
		K32 = (I30*J02)+(I31*J12)+(I32*J22)+(I33*J32);
		K33 = (I30*J03)+(I31*J13)+(I32*J23)+(I33*J33);
	*/
	
	/*Matrix1*/
	//ROW0
	float I00 = InTransformationMatrix1.M[0][0];
	float I01 = InTransformationMatrix1.M[0][1];
	float I02 = InTransformationMatrix1.M[0][2];
	float I03 = InTransformationMatrix1.M[0][3];
	//ROW1
	float I10 = InTransformationMatrix1.M[1][0];
	float I11 = InTransformationMatrix1.M[1][1];
	float I12 = InTransformationMatrix1.M[1][2];
	float I13 = InTransformationMatrix1.M[1][3];
	//ROW2
	float I20 = InTransformationMatrix1.M[2][0];
	float I21 = InTransformationMatrix1.M[2][1];
	float I22 = InTransformationMatrix1.M[2][2];
	float I23 = InTransformationMatrix1.M[2][3];
	//ROW3
	float I30 = InTransformationMatrix1.M[3][0];
	float I31 = InTransformationMatrix1.M[3][1];
	float I32 = InTransformationMatrix1.M[3][2];
	float I33 = InTransformationMatrix1.M[3][3];

	/*Matrix2*/
	//ROW0
	float J00 = InTransformationMatrix2.M[0][0];
	float J01 = InTransformationMatrix2.M[0][1];
	float J02 = InTransformationMatrix2.M[0][2];
	float J03 = InTransformationMatrix2.M[0][3];
	//ROW1
	float J10 = InTransformationMatrix2.M[1][0];
	float J11 = InTransformationMatrix2.M[1][1];
	float J12 = InTransformationMatrix2.M[1][2];
	float J13 = InTransformationMatrix2.M[1][3];
	//ROW2
	float J20 = InTransformationMatrix2.M[2][0];
	float J21 = InTransformationMatrix2.M[2][1];
	float J22 = InTransformationMatrix2.M[2][2];
	float J23 = InTransformationMatrix2.M[2][3];
	//ROW3
	float J30 = InTransformationMatrix2.M[3][0];
	float J31 = InTransformationMatrix2.M[3][1];
	float J32 = InTransformationMatrix2.M[3][2];
	float J33 = InTransformationMatrix2.M[3][3];

	/*Matrix3 = Matrix1*Matrix2 */

	//ROW0
	float K00 = (I00*J00) + (I01*J10) + (I02*J20) + (I03*J30);
	float K01 = (I00*J01) + (I01*J11) + (I02*J21) + (I03*J31);
	float K02 = (I00*J02) + (I01*J12) + (I02*J22) + (I03*J32);
	float K03 = (I00*J03) + (I01*J13) + (I02*J23) + (I03*J33);
	//ROW1
	float K10 = (I10*J00) + (I11*J10) + (I12*J20) + (I13*J30);
	float K11 = (I10*J01) + (I11*J11) + (I12*J21) + (I13*J31);
	float K12 = (I10*J02) + (I11*J12) + (I12*J22) + (I13*J32);
	float K13 = (I10*J03) + (I11*J13) + (I12*J23) + (I13*J33);
	//ROW2
	float K20 = (I20*J00) + (I21*J10) + (I22*J20) + (I23*J30);
	float K21 = (I20*J01) + (I21*J11) + (I22*J21) + (I23*J31);
	float K22 = (I20*J02) + (I21*J12) + (I22*J22) + (I23*J32);
	float K23 = (I20*J03) + (I21*J13) + (I22*J23) + (I23*J33);
	//ROW3
	float K30 = (I30*J00) + (I31*J10) + (I32*J20) + (I33*J30);
	float K31 = (I30*J01) + (I31*J11) + (I32*J21) + (I33*J31);
	float K32 = (I30*J02) + (I31*J12) + (I32*J22) + (I33*J32);
	float K33 = (I30*J03) + (I31*J13) + (I32*J23) + (I33*J33);

	//Assignment of matrix 3 values to out matrix 
	FMatrix TransformationMatrixMultiplied;
	//ROW0
	TransformationMatrixMultiplied.M[0][0] = K00;
	TransformationMatrixMultiplied.M[0][1] = K01;
	TransformationMatrixMultiplied.M[0][2] = K02;
	TransformationMatrixMultiplied.M[0][3] = K03;
	//ROW1
	TransformationMatrixMultiplied.M[1][0] = K10;
	TransformationMatrixMultiplied.M[1][1] = K11;
	TransformationMatrixMultiplied.M[1][2] = K12;
	TransformationMatrixMultiplied.M[1][3] = K13;
	//ROW2
	TransformationMatrixMultiplied.M[2][0] = K20;
	TransformationMatrixMultiplied.M[2][1] = K21;
	TransformationMatrixMultiplied.M[2][2] = K22;
	TransformationMatrixMultiplied.M[2][3] = K23;
	//ROW3
	TransformationMatrixMultiplied.M[3][0] = K30;
	TransformationMatrixMultiplied.M[3][1] = K31;
	TransformationMatrixMultiplied.M[3][2] = K32;
	TransformationMatrixMultiplied.M[3][3] = K33;

	return TransformationMatrixMultiplied;
}

FVector UTransformationMatrix_ViveTracker::TransformationMatrix_PositionExtract(FMatrix InTransformationMatrix)
{
	FVector TransformedPosition;

	TransformedPosition.X = InTransformationMatrix.M[0][3];
	TransformedPosition.Y = InTransformationMatrix.M[1][3];
	TransformedPosition.Z = InTransformationMatrix.M[2][3];

	return TransformedPosition;
}

FRotator UTransformationMatrix_ViveTracker::TransformationMatrix_RotationRPYExtract(FMatrix InTransformationMatrix)
{
	/**
		Tracker Tranformed axis 
		X -> forward
		Y -> left
		Z -> up
		- Right hand rule

		+ A yaw is a counterclockwise rotation around z-axis
		+ A pitch is a counterclockwise rotation around  y-axis
		+ A roll is a counterclockwise rotation around  x-axis
	*/

	/*Rotation Matrix Extraction A[3*3]*/
	//COLUMN0
	float A00 = InTransformationMatrix.M[0][0];
	float A10 = InTransformationMatrix.M[1][0];
	float A20 = InTransformationMatrix.M[2][0];
	//COLUMN1
	float A01 = InTransformationMatrix.M[0][1];
	float A11 = InTransformationMatrix.M[1][1];
	float A21 = InTransformationMatrix.M[2][1];
	//COLUMN2
	float A02 = InTransformationMatrix.M[0][2];
	float A12 = InTransformationMatrix.M[1][2];
	float A22 = InTransformationMatrix.M[2][2];

	// Calculating the euler angle from rotation matrix in radians
	
	/**
		YPR convention - ZYX intrinsic Convention 3D Rotation matrix 
		Intrinsic rotation notion used [ZYX of intrinsic = XYZ of extrinsic]
		// Matlab code for reference // full documentation available in https://github.com/MSIhub
		%% Decompose rotation matrix to euler angle
		% Taking into account gimbal lock and other Euler singularities

		if(A_ZYX(1,3) ~= 1 && A_ZYX(1,3) ~= -1 )
			%Solution 1 -- matches with matlab euler angle
			pitch1 = asin(A_ZYX(1,3));
			roll1 = atan2((-A_ZYX(2,3)/cos(pitch1)),(A_ZYX(3,3)/cos(pitch1)));
			yaw1 = atan2((-1*A_ZYX(1,2)/cos(pitch1)),(A_ZYX(1,1)/cos(pitch1)));
			calculatedEuler = [roll1, pitch1, yaw1]*(180/PI)
		else
			% To overcome gimbal lock and singularities
			% Assign any value to roll, preferably zero
			roll3 = 0;
			if(A_ZYX(1,3) == -1)
				pitch3 = -pi/2;
				yaw3 = -atan2(-A_ZYX(2,1),A_ZYX(2,2));
				calculatedEuler = [roll3, pitch3, yaw3]*(180/PI)
			else
				pitch4 = pi/2;
				yaw4 = atan2(A_ZYX(2,1),A_ZYX(2,2));
				calculatedEuler = [roll3, pitch4, yaw4]*(180/PI)
			end
		end

	*/
	float roll;
	float pitch;
	float yaw;

	if (A02 != 1 && A02 != -1)
	{
		pitch = asinf(A02);
		roll = atan2f((-A12/cosf(pitch)),(A22/cosf(pitch)));
		yaw = atan2f((-A01/cosf(pitch)), (A00/cosf(pitch)));
	}
	else
	{
		//To Overcome gimbal lock and Euler singularities
		roll = 0;
		if (A02 == -1)
		{
			pitch = -PI / 2;
			yaw = -atan2f(-A10,A11);
		}
		else
		{
			pitch = PI / 2;
			yaw = atan2f(A10,A11);
		}
	}

	// Assigning the angles to the rotator converted to degrees
	FRotator TransformedRPY;
	TransformedRPY.Roll = roll * (180/PI) ;
	TransformedRPY.Pitch = pitch * (180 / PI);
	TransformedRPY.Yaw = yaw * (180 / PI);


	return TransformedRPY;
}


