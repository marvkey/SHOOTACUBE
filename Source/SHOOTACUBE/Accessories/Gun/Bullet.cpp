// Fill out your copyright notice in the Description page of Project Settings.
#include "Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SHOOTACUBE/Player/Player1.h"
#include "Gun.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

ABullet::ABullet(){
    ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
    ParticleTrail = CreateDefaultSubobject< UParticleSystemComponent>(TEXT("Particle system"));
    ParticleTrail->SetupAttachment(SkeletalMesh);
}
void ABullet::BeginPlay(){
    Super::BeginPlay();
    Collider->OnComponentHit.AddDynamic(this,&ABullet::OnHit);
    StartLocation=GetActorLocation();
    UGameplayStatics::PlaySoundAtLocation(this,LunchSound,GetActorLocation());
}

void  ABullet::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    if(GetOwner() != nullptr){
        MyOwner=GetOwner();
        if(GetOwner()->GetOwner() != nullptr){
            MyOwnersOwner=GetOwner()->GetOwner();
        }
    }
}
void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
    GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("We hit an actor")));
    UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
    UGameplayStatics::SpawnEmitterAtLocation(this,HitParticle, GetActorLocation());
    
    if(OtherActor && (OtherActor->IsA(APlayer1::StaticClass())) && (OtherActor != MyOwner) && (OtherActor !=MyOwnersOwner)){
        APlayer1* DamagedPlayer=Cast<APlayer1>(OtherActor);
        AGun* CurrentGun=Cast<AGun>(GetOwner());
        if(CurrentGun->GetAmmoOfGun()==AmmoType::SmallAmmo || CurrentGun->GetAmmoOfGun()==AmmoType::MediumAmmo){
            UGameplayStatics::ApplyDamage(DamagedPlayer,CurrentGun->GunDamage,MyOwnersOwner->GetInstigatorController(),MyOwnersOwner,DamageType);
            GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,FString::Printf(TEXT("We Hit a Player we Gave Damage %F and the Player has a Health of %F "),CurrentGun->GunDamage,DamagedPlayer->GetHealthRemaining()));
        }else if(CurrentGun->GetAmmoOfGun() ==AmmoType::RocketLuncherAmmo){
            DamageSize=CurrentGun->GunDamage;
            UGameplayStatics::ApplyRadialDamage(GetWorld(),CurrentGun->GunDamage,GetActorLocation(), CurrentGun->DamageRadiusRokcetLuncher,DamageType,TArray<AActor*>(),MyOwnersOwner,MyOwnersOwner->GetInstigatorController());
        }
    }
}