// Fill out your copyright notice in the Description page of Project Settings.
#include "AI_Enemy_Controller.h"
#include "Kismet/GameplayStatics.h"
#include  "SHOOTACUBE/Player/Player1.h"
AAI_Enemy_Controller::AAI_Enemy_Controller(){
    PrimaryActorTick.bCanEverTick = true;
}

void  AAI_Enemy_Controller::BeginPlay(){
    Super::BeginPlay();
    GetControlledcharacter=Cast<APlayer1>(GetPawn());
    FirstPlayer=Cast<APlayer1>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
    GetWorldTimerManager().SetTimer(UpdateSeconds,this,&AAI_Enemy_Controller::MoveToPlayer,2,true);
}

void AAI_Enemy_Controller::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    if(FirstPlayer ==nullptr){
        return;
    }
    if(GetControlledcharacter != nullptr){
        //MoveToLocation(StartLocation);
    }
}

void AAI_Enemy_Controller::Shoot(){
    if(GetControlledcharacter != nullptr){
        GetControlledcharacter->ShootWeapon();
    }
}

void AAI_Enemy_Controller::MoveToPlayer(){
    if(FirstPlayer != nullptr){
        if(LineOfSightTo(FirstPlayer)){
            AAIController::SetFocus(FirstPlayer,EAIFocusPriority::Gameplay);
            AAIController::MoveToActor(FirstPlayer, AcceptableRadius);
            int RandomNumber=FMath::RandRange(0,1000);
            PlayerIsTooClose();
            if(RandomNumber >970){
                Shoot();
            }
        }
    }
}

float AAI_Enemy_Controller::DistaneFromPlayer(){
    if(FirstPlayer==nullptr){
        return 0.0f;
    }
    return FVector::Dist(GetControlledcharacter->GetActorLocation(), FirstPlayer->GetActorLocation());
}
void AAI_Enemy_Controller::PlayerIsTooClose(){
    if(DistaneFromPlayer()<AcceptableRadius){
        MoveToLocation(GetControlledcharacter->GetActorLocation()-20);
    }
}
 