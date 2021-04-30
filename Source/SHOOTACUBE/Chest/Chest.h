// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SHOOTACUBE/Accessories/Gun/Gun.h"
#include "Chest.generated.h"
UCLASS()
class SHOOTACUBE_API AChest : public AActor{
	GENERATED_BODY()
public:	
	AChest();
	UPROPERTY(EditAnywhere, Category="Collider")
	class UBoxComponent*Collider;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category="Collider")
	class USkeletalMeshComponent* SkeletalMesh;

protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(EditAnywhere, Category="Spawn")
	TArray<TSubclassOf<class AGun>>SpawningGuns;
	UPROPERTY(EditAnywhere, Category="Spawn")
	TArray<TSubclassOf<class AAmmo>>SpawnAmmo;
	int RandomNumber;
	class AGun* Gun=nullptr;
};
