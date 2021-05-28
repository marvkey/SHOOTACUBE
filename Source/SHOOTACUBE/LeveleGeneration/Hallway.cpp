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
	GetWorldTimerManager().SetTimer(TImerManager,this,&AHallway::DoDestroy,2.0f,true);
}

void AHallway::DoDestroy(){
	
	AGenerateLevel* GenereateLevelOwner = Cast<AGenerateLevel>(GetOwner());
	if(NewObjectSpawned == nullptr){
		UE_LOG(LogTemp,Warning,TEXT("Actor is null %s"),*this->GetName());
		if(GetOwner() == nullptr){
			this->Destroy();
			UE_LOG(LogTemp,Warning,TEXT("has no owner so it is destroyed %s"),*this->GetName());
			return;
		}
		if(GenereateLevelOwner->GetSpawnType() == TypeSpawn::Main){
			UE_LOG(LogTemp,Warning,TEXT("has spawn type of main %s"),*this->GetName());
			return;
		}
		this->Destroy();
		UE_LOG(LogTemp,Warning,TEXT("Destroyed Hallway %s"),*this->GetName());
		
	}
	
}

// Called every frame
void AHallway::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	GetAllChildActors(ChildActors);
	//DoDestroy();
}

void AHallway::SpawnGenerateLevel(){
	if(FoundActors.Num() <=8){
		FVector SpawnLocation(FrontSide->GetComponentLocation());
		FRotator  RotationSpawn(0,0,0);
		FActorSpawnParameters SpawnParam;
		NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);
		if(NewObjectSpawned != nullptr){
			AGenerateLevel* Level=Cast<AGenerateLevel>(NewObjectSpawned);
			LevelGenerate =Level;
			if(Level != nullptr){
				FRotator GG(90,0,0);
				LevelGenerate->HallwaySpawnSceneComponent3->SetRelativeRotation(GG);
				LevelGenerate->AttachToComponent(FrontSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				LevelGenerate->FrontDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				LevelGenerate->FrontDoorSide->SetHiddenInGame(true);
				LevelGenerate->SetOwner(this);
			}
		}
	}
}
