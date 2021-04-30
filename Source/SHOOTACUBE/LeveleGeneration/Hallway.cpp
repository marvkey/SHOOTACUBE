// Fill out your copyright notice in the Description page of Project Settings.
#include "Hallway.h"
#include "GenerateLevel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
// Sets default values
AHallway::AHallway(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
    SceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("scene Component"));
	RootComponent=SceneComponent;

	SceneComponent2=CreateDefaultSubobject<USceneComponent>(TEXT("scene Component2"));
	SceneComponent2->SetupAttachment(RootComponent);
	
	LeftSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSide"));
	LeftSide->SetupAttachment(SceneComponent2);

	RightSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSide"));
	RightSide->SetupAttachment(SceneComponent2);

	FrontSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontSide"));
	FrontSide->SetupAttachment(SceneComponent2);

	BackWall=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bckwall"));
	BackWall->SetupAttachment(SceneComponent2);

	Floor=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(SceneComponent2);
}

void AHallway::BeginPlay(){
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerateLevel::StaticClass(), FoundActors);
	SetActorScale3D(FVector(6,6,6));
	SpawnGenerateLevel();
}

// Called every frame
void AHallway::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	if(NewObjectSpawned ==nullptr){
	}
}

void AHallway::SpawnGenerateLevel(){
	if(FoundActors.Num() <10){
		FVector SpawnLocation(FrontSide->GetComponentLocation().X,FrontSide->GetComponentLocation().Y,FrontSide->GetComponentLocation().Z);
		FRotator  RotationSpawn(0,0,0);
		FActorSpawnParameters SpawnParam;
		NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);
		if(NewObjectSpawned != nullptr){
			AGenerateLevel* Level=Cast<AGenerateLevel>(NewObjectSpawned);
			if(Level != nullptr){
				FRotator GG(90,0,0);
				Level->HallwaySpawnSceneComponent3->SetRelativeRotation(GG);
				Level->AttachToComponent(FrontSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				Level->FrontDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				Level->FrontDoorSide->SetHiddenInGame(true);
			}
		}
	}
}