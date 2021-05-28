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
	void ConstantMoving();
	UPROPERTY(EditAnywhere,Category="AI Functions")
	float AcceptableRadius=1000;
	
	class APawn* FirstPlayer;
	FTimerHandle UpdateSeconds;
	class APlayer1* GetControlledcharacter;
	FVector StartLocation;
	UPROPERTY(EditAnywhere,Category="AI Functions")
	float MinMovement =3000;
	UPROPERTY(EditAnywhere,Category="AI Functions")
	float MaxMovement =6000;
	FVector LoctionToMove ={1,1,0};
	int beginMove =0 ;

	FTimerHandle ShootTimerHandle;
	FTimerHandle CallWaitBeginPlay;
	float GunShootRate;
	void WaitBeginePlay();
};
