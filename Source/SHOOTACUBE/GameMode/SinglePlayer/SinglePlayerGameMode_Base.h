// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SHOOTACUBE/GameMode/ShootacubeBaseGameMode.h"
#include "SinglePlayerGameMode_Base.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTACUBE_API ASinglePlayerGameMode_Base : public AShootacubeBaseGameMode{
	GENERATED_BODY()
	public:
	virtual void Tick(float DeltaSeconds) override;
	void GoToNextLevel();
	void UpdateTime();
	void CheckToGoToNextLevel();
	void TheRealGoToNextLevel();
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Time")
	int32 HourTime;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Time")
	int32 MinuteTime;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Time")
	int32 SecondTime;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Time")
	int32 MillisecondTime;
	
	FTimerHandle UpdateSeconds;
	FTimerHandle  NextLeavelTimer;
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Time")
	int32 Level=1;
	
	TArray<AActor*> FoundActors;
	TArray<AActor*> FoundActorsPlayer;
	TArray<AActor*>Hallway;
	TArray<AActor*>SpawnControll;

	class APlayer1 *FirstPlayer;
	int32 ReachedMaxNumOFTrue;

	protected:
	virtual void BeginPlay() override;
	
	private:
	class APlayer1Controller* PlayerController ;
	bool bIsCheck=false; // cheks if function is already on the tick
};
