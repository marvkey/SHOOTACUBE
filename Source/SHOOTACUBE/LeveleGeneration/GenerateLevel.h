// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "SpawnGenerateLevel.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "SHOOTACUBE/Player/Player1.h"
#include "SHOOTACUBE/Chest/Chest.h"
#include "Engine/EngineTypes.h"
#include "GenerateLevel.generated.h"
UENUM()
enum class TypeSpawn{
	Main,
	NotMain
};
class AHallway;
UCLASS()
class SHOOTACUBE_API AGenerateLevel : public AActor{
	GENERATED_BODY()
public:	
	AGenerateLevel();
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category="Component")USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, Category="Component")USceneComponent* MiddleOfSquareSceneComponent;
	
	UPROPERTY(EditAnywhere, Category="Component")USceneComponent* HallwaySpawnSceneComponent3;
	
	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* LeftSide;
	
	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* LeftSideSecondHalf;
	
	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* LeftDoorSide;

	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* RightSide;

	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* RightSideSecondHalf;
	
	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* RightDoorSide;
	
	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* FrontSide;

	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* FrontSideSecondHalf;

	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent* FrontDoorSide;

	UPROPERTY(EditAnywhere, Category ="Component")UStaticMeshComponent* BackWall;

	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent*BackSideSecondHalf;
	
	UPROPERTY(EditAnywhere, Category="Component")UStaticMeshComponent*BackDoorSide;
	
	UPROPERTY(EditAnywhere, Category ="Component")UStaticMeshComponent* Floor; 
	
	UPROPERTY(EditAnywhere, Category ="Component")UBoxComponent* Collider;

	UPROPERTY(EditAnywhere, Category ="Component")UBoxComponent* ColliderLeftOverlap;

	UPROPERTY(EditAnywhere, Category ="Component")UBoxComponent* ColliderRightOverlap;

	UPROPERTY(EditAnywhere, Category ="Component")UBoxComponent* ColliderBackWardOverlap;

	UPROPERTY(EditAnywhere, Category ="Component")UBoxComponent* ColliderFrontOverlap;
	
	bool bHasBeenOverllaped=false;
	bool bHasBeenLoopedOver=false;
	void Wait();
	TypeSpawn GetSpawnType(){return SpawnType;}
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

	UFUNCTION()
	void LeftOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void RightOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void BackOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	TArray<AActor*> FoundActors;
	
	TArray<AActor*>OverlappedActorLeft;
	TArray<AActor*>OverlappedActorRight;
	TArray<AActor*>OverlappedActorBackward;
	TArray<AActor*>OverlappedActorFoward;

	UPROPERTY(EditAnywhere,Category="Spawning Objects")
	TypeSpawn SpawnType= TypeSpawn::NotMain;

	UPROPERTY(EditAnywhere,Category="Spawning Objects")
	TSubclassOf<class AActor>OurSpawningObjects;
	APlayer1* FirstPlayer=nullptr;
	class AActor* NewObjectSpawned;
	UPROPERTY(EditAnywhere,Category="Spawning Objects on componet Overllap")
	TSubclassOf<APlayer1>AI;
	
	UPROPERTY(EditAnywhere,Category="Spawning Objects on componet Overllap")
	TSubclassOf<AChest>Chest;
	
	UPROPERTY(EditAnywhere,Category="Spawning Objects")
	int MaxNumBuild=8;
	bool IsPossibleToSpawnLeft = false;
	bool IsPossibeToSpawnRight = false;
	bool IsPossibleToSpawnFoward = false;
	bool IsPossibleToSpawnBackward = false;
	
	void CheckIfPossibleToSpawnNew();
	void CheckPossibleToSpawnLeft();
	void CheckPossibleToSpawnRight();
	void CheckPossibleToSpawnFoward();
	void CheckPossibleToSpawnBackward();

	UPROPERTY(EditAnywhere, Category="Spawning Objects")
	float RayDistance =3000.0f;

	UPROPERTY(EditAnywhere, Category="Spawning Objects")
	float ZValue = 30;
	FTimerHandle TimeManage;
	bool DestroyOther =false;
	AActor* ActorToDestroy =false;
	TArray<AActor*>ColliderActors;

	AHallway* HallwayFront = nullptr; bool SpawnedFront =false;
	AHallway* HallwayBack = nullptr;  bool SpawnedBack =false;
	AHallway* HallwayLeft = nullptr;  bool SpawnedLeft =false;
	AHallway* HallwayRight = nullptr; bool SpawnedRight = false;
	bool bHasScrolledUp = false;

	void CheckToRemoveHallway();
	FTimerHandle CheckRemoveHallwayTimerHandle;
	void SpawnAi(AActor* OtherActor);
};
