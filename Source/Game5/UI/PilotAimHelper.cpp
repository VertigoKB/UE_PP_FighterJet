// Fill out your copyright notice in the Description page of Project Settings.


#include "PilotAimHelper.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "PlayerHUD.h"

void UPilotAimHelper::NativeConstruct()
{
	Super::NativeConstruct();
	
	GreaterLength = TurbineArrayLength < AltitudeArrayLength ? AltitudeArrayLength : TurbineArrayLength;
	TurbineDigitData.SetNum(GreaterLength);
	AltitudeDigitData.SetNum(GreaterLength);

	InitArrays();

	CachedHud = Cast<APlayerHUD>(GetOwningPlayer()->GetHUD());

	GetWorld()->GetTimerManager().SetTimer(DigitUpdater, FTimerDelegate::CreateLambda([&]() {
		if (CachedHud)
		{
			RawTurbineFloat = CachedHud->ThrustValue;
			RawAltitudeFloat = CachedHud->AltitudeValue;
		}
		TurbineDigitData = GetDigitFromFloat(RawTurbineFloat);
		AltitudeDigitData = GetDigitFromFloat(RawAltitudeFloat);
		RefelectDataToImage();
		}), 0.1f, true);
}

void UPilotAimHelper::NativeDestruct()
{
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(DigitUpdater);
	DigitUpdater.Invalidate();
}

TArray<int32> UPilotAimHelper::GetDigitFromFloat(float Value)
{
	TArray<int32> DigitData;

	if (FMath::IsNearlyEqual(Value, 0.f, (1.0E-4)))
	{
		DigitData.Init(0, GreaterLength);
		return DigitData;
	}

	int32 IntValue = static_cast<int32>(Value);

	while (IntValue > 0)
	{
		int32 Digit = static_cast<int32>(IntValue % 10);
		DigitData.Add(Digit);
		IntValue /= 10;
	}

	while (DigitData.Num() < GreaterLength)
	{
		DigitData.Add(0);
	}

	return DigitData;
}

void UPilotAimHelper::InitArrays()
{
	PreLoadedNumbers[0] = PreLoadedNum0;
	PreLoadedNumbers[1] = PreLoadedNum1;
	PreLoadedNumbers[2] = PreLoadedNum2;
	PreLoadedNumbers[3] = PreLoadedNum3;
	PreLoadedNumbers[4] = PreLoadedNum4;
	PreLoadedNumbers[5] = PreLoadedNum5;
	PreLoadedNumbers[6] = PreLoadedNum6;
	PreLoadedNumbers[7] = PreLoadedNum7;
	PreLoadedNumbers[8] = PreLoadedNum8;
	PreLoadedNumbers[9] = PreLoadedNum9;

	TurbineUnits[0] = TurbineSpeedUnitOne;
	TurbineUnits[1] = TurbineSpeedUnitTen;
	TurbineUnits[2] = TurbineSpeedUnitHund;

	AltitudeUnits[0] = AltitudeUnitOne;
	AltitudeUnits[1] = AltitudeUnitTen;
	AltitudeUnits[2] = AltitudeUnitHund;
	AltitudeUnits[3] = AltitudeUnitThou;
}

void UPilotAimHelper::RefelectDataToImage()
{
	for (int8 i = 0; i < TurbineArrayLength; i++)
	{
		int32 DigitValue = TurbineDigitData[i];
		UTexture2D* TargetNumber = PreLoadedNumbers[DigitValue];
		TurbineUnits[i]->SetBrushFromTexture(TargetNumber, true);
	}

	for (int8 i = 0; i < AltitudeArrayLength; i++)
	{
		int32 DigitValue = AltitudeDigitData[i];
		UTexture2D* TargetNumber = PreLoadedNumbers[DigitValue];
		AltitudeUnits[i]->SetBrushFromTexture(TargetNumber, true);
	}
}
