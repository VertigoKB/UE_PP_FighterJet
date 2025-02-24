// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtificalHorizon.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PlayerHUD.h"

void UArtificalHorizon::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UArtificalHorizon::NativeConstruct()
{
	Super::NativeConstruct();

	if (!CachAndInitialize())
	{
		RemoveFromParent();
		return;
	}

	HorizonImage->SetBrushFromMaterial(HorizonMaterial);
}

void UArtificalHorizon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentPitch = OwnerHud->PitchValue;
	CurrentRoll = OwnerHud->RollValue;
	PitchOffset = CurrentPitch / 90.f;

	//Elevate or descend by Pitch	(HorizonMaterial is UMaterialInstanceDynamic)
	HorizonMaterial->SetScalarParameterValue(FName(TEXT("PitchOffset")), PitchOffset);

	//Dynamic move by pitch & roll	(HorizonImage is UImage)
	HorizonImage->SetRenderTransformAngle(CurrentRoll);

	float MultiflierToX = CurrentRoll * 3.f;
	float MultiflierToY = CurrentPitch * -3.f;
	FVector2d Target2D = FVector2D(MultiflierToX, MultiflierToY);
	HorizonImage->SetRenderTranslation(Target2D);

}

bool UArtificalHorizon::CachAndInitialize()
{
	if (!OwnerHud)
		OwnerHud = Cast<APlayerHUD>(GetOwningPlayer()->GetHUD());
	if (!OwnerHud)
		return false;

	if (!ParentMaterial)
	{
		UE_LOG(LogTemp, Warning, TEXT("ParentMaterial is null / UArtificalHorizon::CachAndInitialize"));
		return false;
	}

	HorizonMaterial = UMaterialInstanceDynamic::Create(ParentMaterial, this);
	if (!HorizonMaterial)
		return false;

	return true;
}
