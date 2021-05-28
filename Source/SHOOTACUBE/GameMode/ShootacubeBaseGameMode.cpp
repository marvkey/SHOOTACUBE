// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootacubeBaseGameMode.h"

#include "SHOOTACUBE/LeveleGeneration/GenerateLevel.h"
#include "GameFramework/CharacterMovementComponent.h"
AShootacubeBaseGameMode::AShootacubeBaseGameMode(){
    PrimaryActorTick.bCanEverTick = false;
}
void AShootacubeBaseGameMode::BeginPlay(){
    Super::BeginPlay();
}
