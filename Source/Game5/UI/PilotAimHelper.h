// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PilotAimHelper.generated.h"

/**
 * 
 */
UCLASS()
class GAME5_API UPilotAimHelper : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetDigitFromFloat(float Value);

	UFUNCTION()
	void InitArrays();

	UFUNCTION(BlueprintCallable)
	void RefelectDataToImage();

protected:
	UPROPERTY()
	TObjectPtr<class APlayerHUD> CachedHud;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RawTurbineFloat = 0.f;					// Please Update this var at player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RawAltitudeFloat = 0.f;					// Please Update this var at player
protected:
	UPROPERTY()
	TArray<int32> TurbineDigitData;
	UPROPERTY()
	TArray<int32> AltitudeDigitData;
protected:
	static const int8 TurbineArrayLength = 3;

	UPROPERTY()
	TObjectPtr<class UImage> TurbineUnits[TurbineArrayLength];
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TurbineSpeedUnitOne;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TurbineSpeedUnitTen;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TurbineSpeedUnitHund;

	int8 GreaterLength = 0;
	static const int8 AltitudeArrayLength = 4;
	UPROPERTY()
	TObjectPtr<class UImage> AltitudeUnits[AltitudeArrayLength];
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AltitudeUnitOne;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AltitudeUnitTen;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AltitudeUnitHund;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AltitudeUnitThou;

	


	FTimerHandle DigitUpdater;
protected:
	UPROPERTY()
	TObjectPtr<class UTexture2D> PreLoadedNumbers[10];
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum0;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum1;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum2;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum3;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum4;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum5;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum6;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum7;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum8;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedNum9;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UTexture2D> PreLoadedBlank;

};
