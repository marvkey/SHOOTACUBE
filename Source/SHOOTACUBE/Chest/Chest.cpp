// Fill out your copyright notice in the Description page of Project Settings.
#include "Chest.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SHOOTACUBE/Accessories/Gun/Ammo.h"
#include "SHOOTACUBE/Player/Player1.h"

// Sets default values
AChest::AChest(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider =CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent=Collider;
	SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AChest::BeginPlay(){
	Super::BeginPlay();
	//Collider->OnComponentBeginOverlap.AddDynamic(this,&AChest::OnOverlapBegin);
}

// Called every frame
void AChest::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}
void AChest::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	if(OtherActor!= nullptr && OtherActor->IsA(APlayer1::StaticClass())){
		APlayer1* Player=Cast<APlayer1>(OtherActor);
		if(Player->bIsAi==false){
			FVector SpawnLocation(this->GetActorLocation().X,this->GetActorLocation().Y,this->GetActorLocation().Z);
			FRotator  RotationSpawn(0,0,0);
			FActorSpawnParameters SpawnParam;
			RandomNumber=FMath::RandRange(0,SpawningGuns.Num()-1);
			Gun=GetWorld()->SpawnActor<AGun>(SpawningGuns[RandomNumber],SpawnLocation,RotationSpawn,SpawnParam);
			Gun->SetActorLocation(this->GetActorLocation());
			if(Gun->GetAmmoOfGun() ==AmmoType::SmallAmmo){
				GetWorld()->SpawnActor<AAmmo>(SpawnAmmo[0], SpawnLocation, RotationSpawn, SpawnParam);
			}
			else if(Gun->GetAmmoOfGun() ==AmmoType::MediumAmmo){
				GetWorld()->SpawnActor<AAmmo>(SpawnAmmo[1],SpawnLocation,RotationSpawn,SpawnParam);
			
			}else if(Gun->GetAmmoOfGun()==AmmoType::RocketLuncherAmmo){
				GetWorld()->SpawnActor<AAmmo>(SpawnAmmo[2],SpawnLocation,RotationSpawn,SpawnParam);
			}
		}
		this->Destroy();
	}
}

void AChest::Open(){
	FVector SpawnLocation(this->GetActorLocation().X,this->GetActorLocation().Y,this->GetActorLocation().Z);
	FRotator  RotationSpawn(0,0,0);
	FActorSpawnParameters SpawnParam;
	RandomNumber=FMath::RandRange(0,SpawningGuns.Num()-1);
	Gun=GetWorld()->SpawnActor<AGun>(SpawningGuns[RandomNumber],SpawnLocation,RotationSpawn,SpawnParam);
	Gun->SetActorLocation(this->GetActorLocation());
	if(Gun->GetAmmoOfGun() ==AmmoType::SmallAmmo){
		GetWorld()->SpawnActor<AAmmo>(SpawnAmmo[0], SpawnLocation, RotationSpawn, SpawnParam);
	}
	else if(Gun->GetAmmoOfGun() ==AmmoType::MediumAmmo){
		GetWorld()->SpawnActor<AAmmo>(SpawnAmmo[1],SpawnLocation,RotationSpawn,SpawnParam);
			
	}else if(Gun->GetAmmoOfGun()==AmmoType::RocketLuncherAmmo){
		GetWorld()->SpawnActor<AAmmo>(SpawnAmmo[2],SpawnLocation,RotationSpawn,SpawnParam);
	}
	bHasOpened =true;
	this->Destroy();
}

