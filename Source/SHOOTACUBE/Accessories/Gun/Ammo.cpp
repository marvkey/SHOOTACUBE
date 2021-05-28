// Fill out your copyright notice in the Description page of Project Settings.
#include "Ammo.h"
#include "SHOOTACUBE/Player/Player1.h"
#include "Components/BoxComponent.h"

AAmmo::AAmmo(){}
void AAmmo::BeginPlay(){
    Super::BeginPlay();
    Collider->OnComponentBeginOverlap.AddDynamic(this,&AAmmo::OnOverlapBegin);
    if(AmmoTypeToBeSpawned==AmmoType::SmallAmmo){
        AmmoToBeSpawned =FMath::RandRange(10,50);
    } else if(AmmoTypeToBeSpawned==AmmoType::MediumAmmo){
        AmmoToBeSpawned =FMath::RandRange(10,50);
    } else{
        AmmoToBeSpawned =FMath::RandRange(1,3);
    }
}

void AAmmo::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
}

AmmoType AAmmo::GetAmmoTypeToBeSpawned(){
    return AmmoTypeToBeSpawned;
}

void AAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
    if(OtherActor && OtherActor != this){
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap Begin"));
        if(OtherActor->IsA(APlayer1::StaticClass())){
            APlayer1 * FirstPlayer =Cast<APlayer1>(OtherActor);
            if(FirstPlayer->bIsAi == true){return;}
            if(this->AmmoTypeToBeSpawned ==AmmoType::SmallAmmo){
                FirstPlayer->SmallBulletsAmmo+=AmmoToBeSpawned;
            }else if(AmmoTypeToBeSpawned ==AmmoType::MediumAmmo){

                FirstPlayer->MediumBulletAmmo+=AmmoToBeSpawned;
            }else{
                FirstPlayer->RocketLuncherBulletAmmo+=AmmoToBeSpawned;
            }
            this->Destroy();
        }
    }
}