// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShootacubeBaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTACUBE_API AShootacubeBaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category="Time")
	TSubclassOf<class ASpawnGenerateLevel>GenerateLevelControl;
	class ASpawnGenerateLevel* SpawnedControllActor;
	protected:
	virtual  void BeginPlay() override;
};
