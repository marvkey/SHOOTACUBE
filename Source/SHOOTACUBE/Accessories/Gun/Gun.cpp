// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SceneComponent.h"
#include "SHOOTACUBE/Player/Player1.h"
#include "Bullet.h"
AGun::AGun(){
    Bullet=CreateDefaultSubobject<USceneComponent>(TEXT("Bullet"));
    Bullet->SetupAttachment(SkeletalMesh);
    MagazineSizeMinused=MagazineSize;
}
void AGun::BeginPlay(){
    Super::BeginPlay();
}

void AGun::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    OnInteract();

    if(Bullet ==nullptr){
    }
}

void AGun::OnInteract(){
    if(GetOwner() != nullptr && num==0){
        if(APlayer1* Player=Cast<APlayer1>(GetOwner())){
            Player->AddToInventory(this);
            num++;
        }
    }
}

void AGun::OnAttachToPlayer(){
}
AmmoType AGun::GetAmmoOfGun(){
    return AmmoOfGun;
}
void AGun::Shoot(){
    if(Bullet ==nullptr){
    }else{
         if(MagazineSizeMinused > 0){
             if(Bullet != nullptr ){
                    FVector SpawnLocation =Bullet->GetComponentLocation();
                    FRotator SpawnRotaion=Bullet->GetComponentRotation();
                    ABullet* SpawnedBullet=GetWorld()->SpawnActor<ABullet>(BulletSpawned,SpawnLocation,SpawnRotaion);
                    SpawnedBullet->SetOwner(this);
                    MagazineSizeMinused--;
             }
        }else{
            GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("No More GunMagazine Relode")));
        }
    }
}

void AGun::RelodeGun(AGun* GunToRelode,int32 &SmallBulletsAmmo, int32 &MediumBulletAmmo, int32 &RocketLuncherAmmo){
    if(GunToRelode != nullptr){
       SkeletalMesh->GlobalAnimRateScale=RelodeTime;
        SkeletalMesh->PlayAnimation( RelodeAnimation,false);
        if(GunToRelode->AmmoOfGun==AmmoType::SmallAmmo){
            RelodeSmallBulletGun(GunToRelode,SmallBulletsAmmo);
        }else if(GunToRelode->AmmoOfGun==AmmoType::MediumAmmo){
            RelodeMediumBulletGun(GunToRelode,MediumBulletAmmo);
        }else if(GunToRelode->AmmoOfGun ==AmmoType::RocketLuncherAmmo){
            RelodeRocketLuncherGun(GunToRelode, RocketLuncherAmmo);
        }
    }else{
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("Gun is a nullptr")));
    }
}

void AGun::RelodeSmallBulletGun(AGun* GunToRelode, int32 &SmallBulletAmmo){
    if(SmallBulletAmmo >0){
        int32 DifferenceMagazine=(GunToRelode->MagazineSize)-(GunToRelode->MagazineSizeMinused);
        if(DifferenceMagazine ==0){
            GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("magazien is still full")));
        }else{
            if(SmallBulletAmmo >= DifferenceMagazine){
                SmallBulletAmmo-=DifferenceMagazine;
                GunToRelode->MagazineSizeMinused+=DifferenceMagazine;
                GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("Small Bullet Ammo is now %i and GunMagaziine Size is now %i"),SmallBulletAmmo,GunToRelode->MagazineSizeMinused));
            }else if(SmallBulletAmmo  < DifferenceMagazine){
    
                GunToRelode->MagazineSizeMinused+=SmallBulletAmmo;
               SmallBulletAmmo=0;
               GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("Small Bullet Ammo is now %i and GunMagaziine Size is now %i"),SmallBulletAmmo,GunToRelode->MagazineSizeMinused));
            }
        }
    
    }else{
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("No Ammo")));
    }
}

void AGun::RelodeMediumBulletGun(AGun* GunToRelode, int32 &MediumBulletAmmo){
    if(MediumBulletAmmo >0){
        int DifferenceMagazine=(this->MagazineSize)-(this->MagazineSizeMinused);
        if(DifferenceMagazine ==0){
            GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("magazien is still full")));
        }else{
            if(MediumBulletAmmo >=DifferenceMagazine){
                MediumBulletAmmo-=DifferenceMagazine;
                GunToRelode->MagazineSizeMinused+=DifferenceMagazine;
                GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("Mediumullet Ammo is now %i and GunMagaziine Size is now %i"),MediumBulletAmmo,GunToRelode->MagazineSizeMinused));
            }else if(MediumBulletAmmo < DifferenceMagazine){
                MagazineSizeMinused+=MediumBulletAmmo;
                MediumBulletAmmo=0;
                GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("Mediumullet Ammo is now %i and GunMagaziine Size is now %i"),MediumBulletAmmo,GunToRelode->MagazineSizeMinused));
            }
        }
    }else{
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("No Ammo")));
    }
}

void AGun::RelodeRocketLuncherGun(AGun* GunToRelode, int32 &RocketLuncherAmmo){
    if(RocketLuncherAmmo >0){
        int DifferenceMagazine=(this->MagazineSize)-(this->MagazineSizeMinused);
        if(DifferenceMagazine ==0){
            GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("magazien is still full")));
        }else{
            if(RocketLuncherAmmo >=DifferenceMagazine){
                RocketLuncherAmmo-=DifferenceMagazine;
                GunToRelode->MagazineSizeMinused+=DifferenceMagazine;
                GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("Rocket Luncher Ammo is now %i and GunMagaziine Size is now %i"),RocketLuncherAmmo,GunToRelode->MagazineSizeMinused));
        
            }else if(RocketLuncherAmmo < DifferenceMagazine){
                GunToRelode->MagazineSizeMinused+=RocketLuncherAmmo;
                RocketLuncherAmmo=0;
                GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("Rocket Luncher Ammo is now %i and GunMagaziine Size is now %i"),RocketLuncherAmmo,GunToRelode->MagazineSizeMinused));
            }
        }
    }else{
        GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("No Ammo")));
    }
}
