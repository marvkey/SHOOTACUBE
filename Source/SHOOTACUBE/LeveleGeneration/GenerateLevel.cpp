// Fill out your copyright notice in the Description page of Project Settings.
#include "GenerateLevel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Hallway.h"
AGenerateLevel::AGenerateLevel(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("scene Component"));
	RootComponent=SceneComponent;
	HallwaySpawnSceneComponent3=CreateDefaultSubobject<USceneComponent>(TEXT("HallwaySpawn"));
	HallwaySpawnSceneComponent3->SetupAttachment(RootComponent);
	
	MiddleOfSquareSceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("scene Component2"));
	MiddleOfSquareSceneComponent->SetupAttachment(HallwaySpawnSceneComponent3);

	LeftSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSide"));
	LeftSide->SetupAttachment(MiddleOfSquareSceneComponent);

	LeftSideSecondHalf=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSideSecondHalf"));
	LeftSideSecondHalf->SetupAttachment(MiddleOfSquareSceneComponent);

	LeftDoorSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoorSide"));
	LeftDoorSide->SetupAttachment(MiddleOfSquareSceneComponent);

	RightSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSide"));
	RightSide->SetupAttachment(MiddleOfSquareSceneComponent);

	RightSideSecondHalf=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSideSecondHalf"));
	RightSideSecondHalf->SetupAttachment(MiddleOfSquareSceneComponent);

	RightDoorSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoorSide"));
	RightDoorSide->SetupAttachment(MiddleOfSquareSceneComponent);

	FrontSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontSide"));
	FrontSide->SetupAttachment(MiddleOfSquareSceneComponent);

	FrontDoorSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontDoorSide"));
	FrontDoorSide->SetupAttachment(MiddleOfSquareSceneComponent);

	FrontSideSecondHalf=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrontSideSecondHalf"));
	FrontSideSecondHalf->SetupAttachment(MiddleOfSquareSceneComponent);

	BackWall=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bckwall"));
	BackWall->SetupAttachment(MiddleOfSquareSceneComponent);

	BackDoorSide=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackDoorSide"));
	BackDoorSide->SetupAttachment(MiddleOfSquareSceneComponent);

	BackSideSecondHalf=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back door side second half"));
	BackSideSecondHalf->SetupAttachment(MiddleOfSquareSceneComponent);

	Floor=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));
	Floor->SetupAttachment(MiddleOfSquareSceneComponent);

	Collider=CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	Collider->SetupAttachment(MiddleOfSquareSceneComponent);
} 

// Called when the game starts or when spawned
void AGenerateLevel::BeginPlay(){
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerateLevel::StaticClass(), FoundActors);
	SetActorScale3D(FVector(22.5,22.5	,22.5));
	if(SpawnType==TypeSpawn::Main){
		SpawnHallwayFrontDoor();
		bHasBeenOverllaped=true;
	} else{
		SpawnHallway();
	}
	Collider->OnComponentBeginOverlap.AddDynamic(this,&AGenerateLevel::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this,&AGenerateLevel::OnEndOverllap);
}

// Called every frame
void AGenerateLevel::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	FirstPlayer=Cast<APlayer1>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
}


void AGenerateLevel::SpawnHallway(){
	if(FoundActors.Num() < 8){
		int RandomNumber=FMath::RandRange(1,3);
		if(RandomNumber ==1){
			SpawnHallwayRightDoor();
		} else if(RandomNumber ==2){
			SpawnHallwayBackDoor();
		} else if(RandomNumber ==3){
			SpawnHallwayLeftDoor();
		}
	}
}
void AGenerateLevel::SpawnHallwayFrontDoor(){
	FVector SpawnLocation(FrontDoorSide->GetComponentLocation().X,FrontDoorSide->GetComponentLocation().Y,FrontDoorSide->GetComponentLocation().Z);
	FRotator  RotationSpawn(0,0,0);
	FActorSpawnParameters SpawnParam;
	NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);

	if(NewObjectSpawned != nullptr){
		AHallway*Hallway=Cast<AHallway>(NewObjectSpawned);
		if(Hallway != nullptr){
			FRotator GG(90,0,0);
			Hallway->SceneComponent2->SetRelativeRotation(GG);
			Hallway->AttachToComponent(FrontDoorSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			FrontDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FrontDoorSide->SetHiddenInGame(true);
		}
	}
}
void AGenerateLevel::SpawnHallwayBackDoor(){
	FVector SpawnLocation(BackDoorSide->GetComponentLocation().X,BackDoorSide->GetComponentLocation().Y,BackDoorSide->GetComponentLocation().Z);
	FRotator  RotationSpawn(0,0,0);
	FActorSpawnParameters SpawnParam;
	NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);

	if(NewObjectSpawned != nullptr){
		AHallway*Hallway=Cast<AHallway>(NewObjectSpawned);
		if(Hallway != nullptr){
			FRotator GG(-90,180,0);
			Hallway->SceneComponent2->SetRelativeRotation(GG);
			Hallway->AttachToComponent(BackDoorSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			BackDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			BackDoorSide->SetHiddenInGame(true);
		}
	}
}

void AGenerateLevel::SpawnHallwayRightDoor(){
		FVector SpawnLocation(RightDoorSide->GetComponentLocation().X,RightDoorSide->GetComponentLocation().Y,RightDoorSide->GetComponentLocation().Z);
    	FRotator  RotationSpawn(0,0,0);
    	FActorSpawnParameters  SpawnParam;
    	NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);
	
    	if(NewObjectSpawned != nullptr){
    		AHallway*Hallway=Cast<AHallway>(NewObjectSpawned);
    		if(Hallway != nullptr){
    			FRotator GG(90,90,0);
    			Hallway->SceneComponent2->SetRelativeRotation(GG);
    			Hallway->AttachToComponent(RightDoorSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    			RightDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    			RightDoorSide->SetHiddenInGame(true);
    		}
    	}
}

void AGenerateLevel::SpawnHallwayLeftDoor(){
	FVector SpawnLocation(LeftDoorSide->GetComponentLocation().X,LeftDoorSide->GetComponentLocation().Y,LeftDoorSide->GetComponentLocation().Z);
	FRotator  RotationSpawn(0,0,0);
	FActorSpawnParameters SpawnParam;
	NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);
    
	if(NewObjectSpawned != nullptr){
		AHallway*Hallway=Cast<AHallway>(NewObjectSpawned);
		if(Hallway != nullptr){
			FRotator GG(90,90,180);
			Hallway->SceneComponent2->SetRelativeRotation(GG);
			Hallway->AttachToComponent(LeftDoorSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			LeftDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			LeftDoorSide->SetHiddenInGame(true);
		}
	}
}

void AGenerateLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	/*
	if(SpawnType ==TypeSpawn::NotMain){
		if(OtherActor!= nullptr && OtherActor->IsA(APlayer1::StaticClass())){
			APlayer1* Player=Cast<APlayer1>(OtherActor);
			FActorSpawnParameters SpawnParam;
			if(Player->bIsAi ==false){
				this->bHasBeenOverllaped=true;
				int32 Randint=FMath::RandRange(1,10);
				if(Randint <= 8){
					int32 TimeToRepeat=FMath::RandRange(1,5);
					for(int i=0; i<TimeToRepeat; i++){
						int32 RandFromDistanceX=FMath::RandRange(-2000,3000);
						int32 RandFromDistanceY=FMath::RandRange(-2000,3000);
						FVector  ComponentLocation(MiddleOfSquareSceneComponent->GetComponentLocation().X+RandFromDistanceX,MiddleOfSquareSceneComponent->GetComponentLocation().Y+RandFromDistanceY,FirstPlayer->GetActorLocation().Z);
						GetWorld()->SpawnActor<APlayer1>(AI,ComponentLocation,FRotator(0,0,0),SpawnParam);
					}
				}else{
					int32 TimeToRepeat=FMath::RandRange(1,3);
					for(int i=0; i<TimeToRepeat; i++){
						int32 RandFromDistanceX=FMath::RandRange(-2000,3000);
						int32 RandFromDistanceY=FMath::RandRange(-2000,3000);
						FVector  ComponentLocation(MiddleOfSquareSceneComponent->GetComponentLocation().X+ RandFromDistanceX,MiddleOfSquareSceneComponent->GetComponentLocation().Y+RandFromDistanceY,FirstPlayer->GetActorLocation().Z);
						GetWorld()->SpawnActor<AChest>(Chest,ComponentLocation,FRotator(0,0,0),SpawnParam);
					}
				}
		
			}
		}
	}
	*/
}

void AGenerateLevel::OnEndOverllap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex){
	if(bHasBeenOverllaped ==false){
		if(OtherActor->IsA(APlayer1::StaticClass())){
			APlayer1* Player=Cast<APlayer1>(OtherActor);
			if(Player->bIsAi ==false){
				this->bHasBeenOverllaped=true;
			}
		}
	}
}