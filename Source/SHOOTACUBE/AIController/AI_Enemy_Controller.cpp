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
    GetWorldTimerManager().SetTimer(ShootTimerHandle,this,&AAI_Enemy_Controller::Shoot,3,true);   
    GetWorldTimerManager().SetTimer(CallWaitBeginPlay,this,&AAI_Enemy_Controller::WaitBeginePlay,3,false);
    if(GetPawn() != nullptr){
       StartLocation =GetPawn()->GetActorLocation();
    }
}

void AAI_Enemy_Controller::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    if(UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == nullptr){return;}
    if(GetPawn()== nullptr){return;}
    FirstPlayer=UGameplayStatics::GetPlayerPawn(GetWorld(),0);
    if(FirstPlayer != nullptr){
        MoveToPlayer();
        GunShootRate = FMath::RandRange(0.5,5);
    }else{
        AAIController::ClearFocus(EAIFocusPriority::Gameplay);
        //AAIController::MoveToLocation(StartLocation);
    }
}

void AAI_Enemy_Controller::Shoot(){
    if(FirstPlayer == nullptr || GetPawn() == nullptr){return;}
   APlayer1* Control=Cast<APlayer1>(GetPawn());
    if(FirstPlayer != nullptr){
        if(Control != nullptr){
            if(LineOfSightTo(FirstPlayer)){
                Control->ShootWeapon();
            }
        }
    }
}

void AAI_Enemy_Controller::MoveToPlayer(){
    if(FirstPlayer != nullptr){
        if(LineOfSightTo(FirstPlayer)){
            AAIController::SetFocus(FirstPlayer,EAIFocusPriority::Gameplay);
            AAIController::MoveToActor(FirstPlayer, AcceptableRadius);
                   
        }else{
            AAIController::ClearFocus(EAIFocusPriority::Gameplay);
           // ConstantMoving();
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
    if(FirstPlayer == nullptr){return;}
    if(DistaneFromPlayer()<AcceptableRadius){
        AAIController::MoveToLocation(GetControlledcharacter->GetActorLocation()-20);
    }
}
void AAI_Enemy_Controller::ConstantMoving(){
    if(GetControlledcharacter== nullptr){return;}
    FVector Blocked(0,0,0);
    int MoveASide = FMath::RandRange(0,1000);
    if(MoveASide > 500){
        int MoveXAxis = FMath::RandRange(0,1000);
        if(MoveXAxis >=500){
            int PlusDirection = FMath::RandRange(0,1);
            if(PlusDirection == 1){
                float Size = FMath::RandRange(MinMovement,MaxMovement);
                LoctionToMove = {GetControlledcharacter->GetActorLocation().X+Size, GetControlledcharacter->GetActorLocation().Y,GetControlledcharacter->GetActorLocation().Z};
                 AAIController::MoveToLocation(FVector(GetControlledcharacter->GetActorLocation().X+Size, GetControlledcharacter->GetActorLocation().Y,GetControlledcharacter->GetActorLocation().Z));
            }else{
                float Size = FMath::RandRange(MinMovement,MaxMovement);
                LoctionToMove = {GetControlledcharacter->GetActorLocation().X-Size, GetControlledcharacter->GetActorLocation().Y,GetControlledcharacter->GetActorLocation().Z};
                 AAIController::MoveToLocation(FVector(GetControlledcharacter->GetActorLocation().X-Size, GetControlledcharacter->GetActorLocation().Y,GetControlledcharacter->GetActorLocation().Z));
            }
            return;
        }
        // Move in the Y Axis 
        int PlusDirection = FMath::RandRange(0,1);
        if(PlusDirection == 1){
            float Size = FMath::RandRange(MinMovement,MaxMovement);
            LoctionToMove = {GetControlledcharacter->GetActorLocation().X, GetControlledcharacter->GetActorLocation().Y+Size,GetControlledcharacter->GetActorLocation().Z};
             AAIController::MoveToLocation(FVector(GetControlledcharacter->GetActorLocation().X, GetControlledcharacter->GetActorLocation().Y+Size,GetControlledcharacter->GetActorLocation().Z));
        }else{
            float Size = FMath::RandRange(MinMovement,MaxMovement);
            LoctionToMove = {GetControlledcharacter->GetActorLocation().X, GetControlledcharacter->GetActorLocation().Y-Size,GetControlledcharacter->GetActorLocation().Z};
            AAIController::MoveToLocation(FVector(GetControlledcharacter->GetActorLocation().X, GetControlledcharacter->GetActorLocation().Y-Size,GetControlledcharacter->GetActorLocation().Z));
        }
    }
}

void AAI_Enemy_Controller::WaitBeginePlay(){
    
}
