// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnGenerateLevel.generated.h"
UCLASS()
class SHOOTACUBE_API ASpawnGenerateLevel : public AActor{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	ASpawnGenerateLevel();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere,Category="Spawning Objects")
	TSubclassOf<class AGenerateLevel>OurSpawningObjects;
	class AGenerateLevel* NewObjectSpawned;

	UPROPERTY(EditAnywhere, Category ="Value")
	float Pitch;

	UPROPERTY(EditAnywhere, Category ="Value")
	float Yaw;

	UPROPERTY(EditAnywhere, Category ="Value")
	float Roll;
};
