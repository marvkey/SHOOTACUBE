// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePlayerGameMode_Base.h"

#include "SHOOTACUBE/LeveleGeneration/Hallway.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SHOOTACUBE/LeveleGeneration/GenerateLevel.h"
#include "SHOOTACUBE/Player/Player1Controleer.h"
#include "SHOOTACUBE/LeveleGeneration/SpawnGenerateLevel.h"
#include "GameFramework/CharacterMovementComponent.h"
ASinglePlayerGameMode_Base::ASinglePlayerGameMode_Base(){
    PrimaryActorTick.bCanEverTick = true;
}

void ASinglePlayerGameMode_Base::Tick(float DeltaSeconds){
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

void ASinglePlayerGameMode_Base::BeginPlay(){
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(UpdateSeconds,this,&ASinglePlayerGameMode_Base::UpdateTime,0.001,true);
   AShootacubeBaseGameMode::SpawnedControllActor = GetWorld()->SpawnActor<ASpawnGenerateLevel>(AShootacubeBaseGameMode::GenerateLevelControl,FVector(0,0,0),FRotator(0,0,0));
    
}
void ASinglePlayerGameMode_Base::UpdateTime(){
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

void ASinglePlayerGameMode_Base::CheckToGoToNextLevel(){
   
    for(AActor *Actor:FoundActors){
        AGenerateLevel* LevelGenerate=Cast<AGenerateLevel>(Actor);
        if(LevelGenerate->bHasBeenOverllaped==true&& LevelGenerate->bHasBeenLoopedOver==false ){
            ReachedMaxNumOFTrue++;
            LevelGenerate->bHasBeenLoopedOver=true;
        }
    }
    
}
void ASinglePlayerGameMode_Base::GoToNextLevel(){
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
        GetWorldTimerManager().SetTimer(NextLeavelTimer,this,&ASinglePlayerGameMode_Base::TheRealGoToNextLevel,3,false);
    }
}

void ASinglePlayerGameMode_Base::TheRealGoToNextLevel(){
    GetWorld()->GetTimerManager().UnPauseTimer(UpdateSeconds);
    FirstPlayer->GetCharacterMovement()->GravityScale=1;
   // GetWorld()->SpawnActor<ASpawnGenerateLevel>(GenerateLevelControl,FVector(0,0,0),FRotator(0,0,0));
    Level++;
    FirstPlayer->SetActorLocation(FVector(0,0,0));
    PlayerController->RemoveNextLevelScreenFromViewPort();
    bIsCheck=false;
    }