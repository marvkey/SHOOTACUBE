// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hallway.generated.h"
UENUM()
enum class HallwaySpawn{
	FrontDoorSpawn,
	NotFrontDoorSpawn
};
UCLASS()
class SHOOTACUBE_API AHallway : public AActor{
	GENERATED_BODY()
public:	
	AHallway();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category="Component")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, Category="Component")
	USceneComponent* SceneComponent2;
	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* LeftSide;

	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* RightSide;

	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* FrontSide;

	UPROPERTY(EditAnywhere, Category ="Component")
	UStaticMeshComponent* BackWall;

	UPROPERTY(EditAnywhere, Category ="Component")
	UStaticMeshComponent* Floor;
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere,Category="Spawning Objects")
	TSubclassOf<class AActor>OurSpawningObjects;
	class AActor* NewObjectSpawned=nullptr;
	void SpawnGenerateLevel();
	TArray<AActor*> FoundActors;
};
