// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHOOTACUBE/Accessories/Accessories.h"
#include "Gun.h"
#include "Ammo.generated.h"

UCLASS()
class SHOOTACUBE_API AAmmo : public AAccessories{
	GENERATED_BODY()
public:
	virtual  void Tick(float DeltaSeconds) override;
	AAmmo();
	AmmoType GetAmmoTypeToBeSpawned();
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere,Category ="Ammo Type")
	AmmoType AmmoTypeToBeSpawned;
	int AmmoToBeSpawned;
	UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
