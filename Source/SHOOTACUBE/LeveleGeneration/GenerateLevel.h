// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SHOOTACUBE/Player/Player1.h"
#include "SHOOTACUBE/Chest/Chest.h"
#include "GenerateLevel.generated.h"
UENUM()
enum class TypeSpawn{
	Main,
	NotMain
};
UCLASS()
class SHOOTACUBE_API AGenerateLevel : public AActor
{
	GENERATED_BODY()
public:	
	AGenerateLevel();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category="Component")
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, Category="Component")
	USceneComponent* MiddleOfSquareSceneComponent;
	
	UPROPERTY(EditAnywhere, Category="Component")
	USceneComponent* HallwaySpawnSceneComponent3;
	
	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* LeftSide;
	
	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* LeftSideSecondHalf;
	
	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* LeftDoorSide;

	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* RightSide;

	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* RightSideSecondHalf;
	
	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* RightDoorSide;
	
	UPROPERTY(EditAnywhere, Category="Component")
    UStaticMeshComponent* FrontSide;

	UPROPERTY(EditAnywhere, Category="Component")
    UStaticMeshComponent* FrontSideSecondHalf;

	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent* FrontDoorSide;

	UPROPERTY(EditAnywhere, Category ="Component")
	UStaticMeshComponent* BackWall;

	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent*BackSideSecondHalf;
	
	UPROPERTY(EditAnywhere, Category="Component")
	UStaticMeshComponent*BackDoorSide;
	
	UPROPERTY(EditAnywhere, Category ="Component")
	UStaticMeshComponent* Floor; 
	
	UPROPERTY(EditAnywhere, Category ="Component")
	UBoxComponent* Collider;
    
	bool bHasBeenOverllaped=false;
	bool bHasBeenLoopedOver=false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void SpawnHallway();
	void SpawnHallwayFrontDoor();
	void SpawnHallwayBackDoor();
	void SpawnHallwayRightDoor();
	void SpawnHallwayLeftDoor();
	UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
    void OnEndOverllap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	TArray<AActor*> FoundActors;
	UPROPERTY(EditAnywhere,Category="Spawning Objects")
	TypeSpawn SpawnType=TypeSpawn::NotMain;

	UPROPERTY(EditAnywhere,Category="Spawning Objects")
	TSubclassOf<class AActor>OurSpawningObjects;
	APlayer1* FirstPlayer=nullptr;
	class AActor* NewObjectSpawned;
	UPROPERTY(EditAnywhere,Category="Spawning Objects on componet Overllap")
	TSubclassOf<APlayer1>AI;
	
	UPROPERTY(EditAnywhere,Category="Spawning Objects on componet Overllap")
	TSubclassOf<AChest>Chest;
};
