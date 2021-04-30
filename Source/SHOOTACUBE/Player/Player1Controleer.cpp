// Fill out your copyright notice in the Description page of Project Settings.
#include "Player1Controleer.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Player1.h"
APlayer1Controller::APlayer1Controller(){}
void  APlayer1Controller::BeginPlay(){
    Super::BeginPlay();
    
    HUD=CreateWidget(this,PlayerHud);
    if(HUD != nullptr){
        HUD->AddToViewport();
    }
    PlayerGunHUD=CreateWidget(this,GunHud);
    GoToNextLevel=CreateWidget(this,NextLevelScreen);
}
void  APlayer1Controller::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    
    FirstPlayer=Cast<APlayer1>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
    if(FirstPlayer== nullptr){
        this->RemoveViewport();
        return;
    }
    if(PlayerGunHUD->IsInViewport()){
        if(FirstPlayer->CurrentGun  == nullptr){
            PlayerGunHUD->RemoveFromViewport();
        }
    }
    if(PlayerGunHUD->IsInViewport()==false){
        if(FirstPlayer->CurrentGun  != nullptr){
            PlayerGunHUD->AddToViewport();
        }   
    }
}
void APlayer1Controller::RemoveViewport(){
    HUD->RemoveFromViewport();
    if(PlayerGunHUD->IsInViewport()){
        PlayerGunHUD->RemoveFromViewport();
    }
}

void APlayer1Controller::AddNextLevelScreen(){
    if(GoToNextLevel->IsInViewport() ==false){
        GoToNextLevel->AddToViewport();
        RemoveViewport();
    }
}

void APlayer1Controller::RemoveNextLevelScreenFromViewPort(){
    if(GoToNextLevel->IsInViewport() ==true){
        GoToNextLevel->RemoveFromViewport();
            HUD->AddToViewport();
            if(FirstPlayer->CurrentGun != nullptr){
               PlayerGunHUD->AddToViewport();
            }
    }
}