// Copyright Epic Games, Inc. All Rights Reserved.
#include "SHOOTACUBEGameModeBase.h"
#include "LeveleGeneration/Hallway.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SHOOTACUBE/LeveleGeneration/GenerateLevel.h"
#include "SHOOTACUBE/Player/Player1Controleer.h"
#include "SHOOTACUBE/LeveleGeneration/SpawnGenerateLevel.h"
#include "GameFramework/CharacterMovementComponent.h"
ASHOOTACUBEGameModeBase::ASHOOTACUBEGameModeBase(){
    PrimaryActorTick.bCanEverTick = true;
}

void ASHOOTACUBEGameModeBase::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    FirstPlayer=Cast<APlayer1>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenerateLevel::StaticClass(), FoundActors);
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer1::StaticClass(), FoundActorsPlayer);
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHallway::StaticClass(), Hallway);
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnGenerateLevel::StaticClass(), SpawnControll);
    //CheckToGoToNextLevel();
    //GoToNextLevel();
    PlayerController = Cast<APlayer1Controller >(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void ASHOOTACUBEGameModeBase::BeginPlay(){
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(UpdateSeconds,this,&ASHOOTACUBEGameModeBase::UpdateTime,0.001,true);
}
void ASHOOTACUBEGameModeBase::UpdateTime(){
    MillisecondTime++;
    if(MillisecondTime==1000){
        SecondTime++;
        MillisecondTime=0;
    }
    if(SecondTime ==60){
        MinuteTime++;
        SecondTime=0;
    }
    if(MinuteTime ==60)
        HourTime++;
}

void ASHOOTACUBEGameModeBase::CheckToGoToNextLevel(){
    
    for(AActor *Actor:FoundActors){
        AGenerateLevel* LevelGenerate=Cast<AGenerateLevel>(Actor);
        if(LevelGenerate->bHasBeenOverllaped==true&& LevelGenerate->bHasBeenLoopedOver==false ){
            ReachedMaxNumOFTrue++;
            LevelGenerate->bHasBeenLoopedOver=true;
        }
    }
    
}
void ASHOOTACUBEGameModeBase::GoToNextLevel(){
    
    if(ReachedMaxNumOFTrue == FoundActors.Num()&& FoundActorsPlayer.Num()==1 && bIsCheck==false){
        GetWorld()->GetTimerManager().PauseTimer(UpdateSeconds);
        PlayerController->AddNextLevelScreen();
        for(AActor *Actor:FoundActors){
            AGenerateLevel* LevelGenerate=Cast<AGenerateLevel>(Actor);
            LevelGenerate->Destroy();
        }
        for(AActor *Actor:Hallway){
           AHallway* LevelGenerate=Cast<AHallway>(Actor);
            LevelGenerate->Destroy();
        }
        for(AActor *Actor:SpawnControll){
            ASpawnGenerateLevel* LevelGenerate=Cast<ASpawnGenerateLevel>(Actor);
            LevelGenerate->Destroy();
        }
        Level++;
        bIsCheck=true;
        FirstPlayer->GetCharacterMovement()->GravityScale=0;
        GetWorldTimerManager().SetTimer(NextLeavelTimer,this,&ASHOOTACUBEGameModeBase::TheRealGoToNextLevel,3,false);
    }
}

void ASHOOTACUBEGameModeBase::TheRealGoToNextLevel(){
    GetWorld()->GetTimerManager().UnPauseTimer(UpdateSeconds);
    FirstPlayer->GetCharacterMovement()->GravityScale=1;
    GetWorld()->SpawnActor<ASpawnGenerateLevel>(GenerateLevelControl,FVector(0,0,0),FRotator(0,0,0));
    Level++;
    FirstPlayer->SetActorLocation(FVector(0,0,0));
    PlayerController->RemoveNextLevelScreenFromViewPort();
    bIsCheck=false;
}
