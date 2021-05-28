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


	ColliderLeftOverlap =CreateDefaultSubobject<UBoxComponent>(TEXT("LEft Collider"));
	ColliderLeftOverlap->SetupAttachment(MiddleOfSquareSceneComponent);

	ColliderRightOverlap =CreateDefaultSubobject<UBoxComponent>(TEXT("Right Collider"));
	ColliderRightOverlap->SetupAttachment(MiddleOfSquareSceneComponent);

	ColliderBackWardOverlap =CreateDefaultSubobject<UBoxComponent>(TEXT("Back Collider"));
	ColliderBackWardOverlap->SetupAttachment(MiddleOfSquareSceneComponent);

	ColliderFrontOverlap =CreateDefaultSubobject<UBoxComponent>(TEXT("Front Collider"));
	ColliderFrontOverlap->SetupAttachment(MiddleOfSquareSceneComponent);
} 

// Called when the game starts or when spawned
void AGenerateLevel::BeginPlay(){
	Super::BeginPlay();
	SetActorScale3D(FVector(22.5,22.5,22.5));
	GetWorldTimerManager().SetTimer(CheckRemoveHallwayTimerHandle, this,&AGenerateLevel::CheckToRemoveHallway,2.0f,true);
	GetWorldTimerManager().SetTimer(TimeManage,this, &AGenerateLevel::Wait,0.5,false);
	IsPossibleToSpawnBackward =true; // these 2 variables could get changed to false if we begin overlap
	IsPossibeToSpawnRight =true;
	IsPossibleToSpawnLeft =true;
	ColliderLeftOverlap->OnComponentBeginOverlap.AddDynamic(this,&AGenerateLevel::LeftOnOverlapBegin);
	ColliderRightOverlap->OnComponentBeginOverlap.AddDynamic(this,&AGenerateLevel::RightOnOverlapBegin);
	ColliderBackWardOverlap->OnComponentBeginOverlap.AddDynamic(this,&AGenerateLevel::BackOnOverlapBegin);
	Collider->OnComponentBeginOverlap.AddDynamic(this,&AGenerateLevel::OnOverlapBegin);
	Collider->OnComponentEndOverlap.AddDynamic(this,&AGenerateLevel::OnEndOverllap);

	CheckIfPossibleToSpawnNew();
	if(SpawnType==TypeSpawn::Main){
		SpawnHallwayFrontDoor();
		bHasBeenOverllaped=true;
	} else{
		SpawnHallway();
	}
}

// Called every frame
void AGenerateLevel::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerateLevel::StaticClass(), FoundActors);
	FirstPlayer=Cast<APlayer1>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	Collider->GetOverlappingActors(ColliderActors);
	if(NewObjectSpawned == nullptr){
		UE_LOG(LogTemp,Warning,TEXT("Nullptr"));
	}
}

void AGenerateLevel::SpawnHallway(){
	a:
	if(FoundActors.Num() < MaxNumBuild){
		int RandomNumber=FMath::RandRange(1,3);
		if(RandomNumber ==1 && IsPossibeToSpawnRight ==true)
			SpawnHallwayRightDoor();
		else if(RandomNumber ==2 && IsPossibleToSpawnLeft ==true)
			SpawnHallwayLeftDoor();
		else if(RandomNumber ==3 && IsPossibleToSpawnBackward ==true)
			SpawnHallwayBackDoor();
		else
		goto a;
	}
}
void AGenerateLevel::SpawnHallwayFrontDoor(){
	FVector SpawnLocation(FrontDoorSide->GetComponentLocation());
	FRotator  RotationSpawn(0,0,0);
	FActorSpawnParameters SpawnParam;
	NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);
	 // makes owner null
	if(NewObjectSpawned != nullptr){
		AHallway*Hallway=Cast<AHallway>(NewObjectSpawned);
		HallwayFront = Hallway;
		if(Hallway != nullptr){
			FRotator GG(90,0,0);
			Hallway->SceneComponent2->SetRelativeRotation(GG);
			Hallway->AttachToComponent(FrontDoorSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			Hallway->SetOwner(this);
			FrontDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			FrontDoorSide->SetHiddenInGame(true);
		}
		IsPossibleToSpawnFoward=false;
		SpawnedFront = true;
	}
}
void AGenerateLevel::SpawnHallwayBackDoor(){
	FVector SpawnLocation(BackDoorSide->GetComponentLocation().X,BackDoorSide->GetComponentLocation().Y,BackDoorSide->GetComponentLocation().Z);
	FRotator  RotationSpawn(0,0,0);
	FActorSpawnParameters SpawnParam;
	NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);	
	if(NewObjectSpawned != nullptr){  
		AHallway*Hallway=Cast<AHallway>(NewObjectSpawned);
		HallwayBack = Hallway;
		if(Hallway != nullptr){
			FRotator GG(-90,180,0);
			Hallway->SceneComponent2->SetRelativeRotation(GG);
			Hallway->AttachToComponent(BackDoorSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			Hallway->SetOwner(this);
			BackDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			BackDoorSide->SetHiddenInGame(true);
		}
		IsPossibleToSpawnBackward =false;
		SpawnedBack = false;
	}
}

void AGenerateLevel::SpawnHallwayRightDoor(){
	FVector SpawnLocation(RightDoorSide->GetComponentLocation().X,RightDoorSide->GetComponentLocation().Y,RightDoorSide->GetComponentLocation().Z);
    FRotator  RotationSpawn(0,0,0);
    FActorSpawnParameters  SpawnParam;
    NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);
    if(NewObjectSpawned != nullptr){
		 AHallway*Hallway=Cast<AHallway>(NewObjectSpawned);
    	HallwayRight =Hallway;
        if(Hallway != nullptr){
           	FRotator GG(90,90,0);
           	Hallway->SceneComponent2->SetRelativeRotation(GG);
           	Hallway->AttachToComponent(RightDoorSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        	Hallway->SetOwner(this);
           	RightDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
           	RightDoorSide->SetHiddenInGame(true);
        }
    	IsPossibeToSpawnRight =false;
    	SpawnedRight =false;
    }
}

void AGenerateLevel::SpawnHallwayLeftDoor(){
	FVector SpawnLocation(LeftDoorSide->GetComponentLocation().X,LeftDoorSide->GetComponentLocation().Y,LeftDoorSide->GetComponentLocation().Z);
	FRotator  RotationSpawn(0,0,0);
	FActorSpawnParameters SpawnParam;
	NewObjectSpawned=GetWorld()->SpawnActor<AActor>(OurSpawningObjects,SpawnLocation,RotationSpawn,SpawnParam);    
	if(NewObjectSpawned != nullptr){
		AHallway*Hallway=Cast<AHallway>(NewObjectSpawned);
		HallwayLeft = Hallway;
		if(Hallway != nullptr){
			FRotator GG(90,90,180);
			Hallway->SceneComponent2->SetRelativeRotation(GG);
			Hallway->AttachToComponent(LeftDoorSide, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			Hallway->SetOwner(this);
			LeftDoorSide->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			LeftDoorSide->SetHiddenInGame(true);
		}
		IsPossibleToSpawnLeft =false;
		SpawnedLeft =true;
	}
}

void AGenerateLevel::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor->IsA(AGenerateLevel::StaticClass())){
		AGenerateLevel* Check = static_cast<AGenerateLevel*>(OtherActor);
		if( this == Check){
			//UE_LOG(LogTemp,Warning,TEXT("the check is equal to this CHECK G"));
		}else if(Check->SpawnType == TypeSpawn::NotMain){
			OtherActor->GetOwner()->Destroy();
			OtherActor->Destroy(); 
			UE_LOG(LogTemp,Warning,TEXT("Overlapp has been destroyed with check"));
		}else if(Check->SpawnType == TypeSpawn::Main){
			this->GetOwner()->Destroy();
			this->Destroy(); 
			UE_LOG(LogTemp,Warning,TEXT("Destroyed ouself cause we where overlapping main"));

		}
	}
	SpawnAi(OtherActor);
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

void AGenerateLevel::LeftOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	//UE_LOG(LogTemp,Warning,TEXT("Overlapp on left"));
	if(OtherActor!= this && OtherActor!= GetOwner()){
		if(OtherActor->IsA(AGenerateLevel::StaticClass()) || OtherActor->IsA(AHallway::StaticClass()))
			IsPossibleToSpawnLeft =false;
	}else
		IsPossibleToSpawnLeft = true;
}

void AGenerateLevel::RightOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
//	UE_LOG(LogTemp,Warning,TEXT("Overlapp on Right"));
	if(OtherActor!= this && OtherActor!= GetOwner()){
		if(OtherActor->IsA(AGenerateLevel::StaticClass()) || OtherActor->IsA(AHallway::StaticClass()))
			IsPossibeToSpawnRight =false;
	}else
		IsPossibeToSpawnRight = true;
}

void AGenerateLevel::BackOnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	//UE_LOG(LogTemp,Warning,TEXT("Overlapp on IsPossibleToSpawnBackward"));
	if(OtherActor!= this && OtherActor!= GetOwner()){
		if (OtherActor->IsA(AGenerateLevel::StaticClass()) || OtherActor->IsA(AHallway::StaticClass()))
			IsPossibleToSpawnBackward=false;
	}else
		IsPossibleToSpawnBackward = true;
}

void AGenerateLevel::CheckIfPossibleToSpawnNew(){
	CheckPossibleToSpawnLeft();
	CheckPossibleToSpawnRight();
	CheckPossibleToSpawnBackward();
	CheckPossibleToSpawnFoward();
}

void AGenerateLevel::CheckPossibleToSpawnLeft(){
	ColliderLeftOverlap->GetOverlappingActors(OverlappedActorLeft);
	if(OverlappedActorLeft.Num() > 0){
		for(AActor* Overlapped: OverlappedActorLeft){
			if(Overlapped!=nullptr && Overlapped !=this){
				if(Overlapped->IsA(AGenerateLevel::StaticClass())){
					IsPossibleToSpawnLeft =false;
				//	UE_LOG(LogTemp,Warning,TEXT("Overlapp on left"));
					break;
				}
				if(Overlapped != this->GetOwner()){ // the owner is the hallway wich spawns it
					if(Overlapped->IsA(AHallway::StaticClass())){
						IsPossibleToSpawnLeft =false;
						//UE_LOG(LogTemp,Warning,TEXT("Overlapp on left"));
						break;
					}
				}
			}
		}
		return;
	}
	IsPossibleToSpawnLeft =true;
}
void AGenerateLevel::CheckPossibleToSpawnRight(){
	ColliderRightOverlap->GetOverlappingActors(OverlappedActorRight);
	if(OverlappedActorRight.Num() > 0){
		for(AActor* Overlapped: OverlappedActorRight){
			if(Overlapped!=nullptr && Overlapped!=this){
				if(Overlapped->IsA(AGenerateLevel::StaticClass())){
					IsPossibeToSpawnRight =false;
					//UE_LOG(LogTemp,Warning,TEXT("Overlapp on Right"));
					break;
				}
				if(Overlapped != this->GetOwner()){ // the owner is the hallway wich spawns it
					if(Overlapped->IsA(AHallway::StaticClass())){
						IsPossibeToSpawnRight =false;
						//UE_LOG(LogTemp,Warning,TEXT("Overlapp on Right"));
						break;
					}
				}
			}
		}
		return;
	}
	IsPossibeToSpawnRight =true;
	
}
void AGenerateLevel::CheckPossibleToSpawnFoward(){
    ColliderFrontOverlap->GetOverlappingActors(OverlappedActorFoward);
    if(OverlappedActorFoward.Num() > 0){
    	for(AActor* Overlapped: OverlappedActorFoward){
    		if(Overlapped!=nullptr && Overlapped!=this){
    			if(Overlapped->IsA(AGenerateLevel::StaticClass())){
    				IsPossibleToSpawnFoward =false;
    				//UE_LOG(LogTemp,Warning,TEXT("Overlapp on front"));
    				break;
    			}
    			if(Overlapped != this->GetOwner()){ // the owner is the hallway wich spawns it
    				if(Overlapped->IsA(AHallway::StaticClass())){
    					IsPossibleToSpawnFoward =false;
    					//UE_LOG(LogTemp,Warning,TEXT("Overlapp on front"));
    					break;
    				}
    			}
    		}
    	}
    	return;
    }
    IsPossibleToSpawnFoward =true;
}

void AGenerateLevel::CheckPossibleToSpawnBackward(){
	ColliderBackWardOverlap->GetOverlappingActors(OverlappedActorBackward);
	if(OverlappedActorBackward.Num() > 0){
		for(AActor* Overlapped: OverlappedActorBackward){
			if(Overlapped!=nullptr && Overlapped!=this){
				if(Overlapped->IsA(AGenerateLevel::StaticClass())){
					IsPossibleToSpawnBackward =false;
					//UE_LOG(LogTemp,Warning,TEXT("Overlapp on back"));
					break;
				}
				if(Overlapped != this->GetOwner()){ // the owner is the hallway wich spawns it
					if(Overlapped->IsA(AHallway::StaticClass())){
						IsPossibleToSpawnBackward =false;
						//UE_LOG(LogTemp,Warning,TEXT("Overlapp on back"));
						break;
					}
				}
			}
		}
		return;
	}
	IsPossibleToSpawnBackward =true;
}

void AGenerateLevel::CheckToRemoveHallway(){
	if(HallwayFront == nullptr && SpawnedFront ==true){
		SpawnedFront = false;
		FrontDoorSide->SetHiddenInGame(false);
		FrontDoorSide->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(LogTemp,Warning,TEXT("Collision front fixed"));
	}
	if(HallwayBack == nullptr && SpawnedBack == true){
		SpawnedBack = false;
		BackDoorSide->SetHiddenInGame(false);
		BackDoorSide->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(LogTemp,Warning,TEXT("Collision back fixed"));
	}

	if(HallwayLeft == nullptr && SpawnedLeft == true){
		SpawnedLeft =false;
		LeftDoorSide->SetHiddenInGame(false);
		LeftDoorSide->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(LogTemp,Warning,TEXT("Collision Left fixed"));
	}

	if(HallwayRight == nullptr && SpawnedRight == true){
		SpawnedRight = false;
		RightDoorSide->SetHiddenInGame(false);
		RightDoorSide->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(LogTemp,Warning,TEXT("Collision Right fixed"));
	}
}

void AGenerateLevel::SpawnAi(AActor* OtherActor){
	if(bHasBeenOverllaped == true){return;}
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
				bHasBeenOverllaped = true;
			}
		}
	}
}

void AGenerateLevel::Wait(){
	//UE_LOG(LogTemp,Warning,TEXT("we destroyed an actor"));
	Collider->SetWorldLocation(FVector(Collider->GetComponentLocation().X,Collider->GetComponentLocation().Y,Collider->GetComponentLocation().Z+ZValue));
	ColliderLeftOverlap->SetWorldLocation(FVector(ColliderLeftOverlap->GetComponentLocation().X,ColliderLeftOverlap->GetComponentLocation().Y,ColliderLeftOverlap->GetComponentLocation().Z+ZValue));
	ColliderRightOverlap->SetWorldLocation(FVector(ColliderRightOverlap->GetComponentLocation().X,ColliderRightOverlap->GetComponentLocation().Y,ColliderRightOverlap->GetComponentLocation().Z+ZValue));
	ColliderBackWardOverlap->SetWorldLocation(FVector(ColliderBackWardOverlap->GetComponentLocation().X,ColliderBackWardOverlap->GetComponentLocation().Y,ColliderBackWardOverlap->GetComponentLocation().Z+ZValue));

	//Collider->SetWorldLocation(FVector(Collider->GetComponentLocation().X,Collider->GetComponentLocation().Y,Collider->GetComponentLocation().Z+ZValue));
}
