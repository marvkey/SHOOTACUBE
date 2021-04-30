// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Player1.h"
#include "Player1Controleer.generated.h"
UCLASS()
class SHOOTACUBE_API APlayer1Controller : public APlayerController{
	GENERATED_BODY()
	APlayer1Controller();
	
public:
	void AddNextLevelScreen();
	void RemoveNextLevelScreenFromViewPort();
	
protected:
	virtual void BeginPlay() override;
	virtual  void Tick(float DeltaSeconds) override;
	
private:
	void RemoveViewport();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> PlayerHud;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> GunHud;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget>NextLevelScreen;
	
	UPROPERTY()
	UUserWidget* HUD;
	
	UPROPERTY()
	UUserWidget* PlayerGunHUD;
	
	UPROPERTY()
	UUserWidget* GoToNextLevel;
	APlayer1 *FirstPlayer;
};
