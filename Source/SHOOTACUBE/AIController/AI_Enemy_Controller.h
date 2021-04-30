// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "AI_Enemy_Controller.generated.h"
UCLASS()
class SHOOTACUBE_API AAI_Enemy_Controller : public AAIController{
	GENERATED_BODY()
public:
	AAI_Enemy_Controller();
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay()override;
	
private:
	void Shoot();
	void MoveToPlayer();
	void PlayerIsTooClose(); // Basically what hapens if the palyer is to close to us.
	float DistaneFromPlayer();
	UPROPERTY(EditAnywhere,Category="AI Functions")
	float AcceptableRadius=1000;
	
	class APlayer1* FirstPlayer;
	FTimerHandle UpdateSeconds;
	APlayer1* GetControlledcharacter;
	FVector StartLocation;
	
};
