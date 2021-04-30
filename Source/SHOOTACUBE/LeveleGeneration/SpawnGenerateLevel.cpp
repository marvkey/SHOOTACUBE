// Fill out your copyright notice in the Description page of Project Settings.
#include "SpawnGenerateLevel.h"
#include "GenerateLevel.h"
// Sets default values
ASpawnGenerateLevel::ASpawnGenerateLevel(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnGenerateLevel::BeginPlay(){
	Super::BeginPlay();
	FVector SpawnLocation(0,0,0);
    FRotator  RotationSpawn(Pitch,Yaw,Roll);
    FActorSpawnParameters SpawnParam;
    NewObjectSpawned=GetWorld()->SpawnActor<AGenerateLevel>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);
}

void ASpawnGenerateLevel::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}