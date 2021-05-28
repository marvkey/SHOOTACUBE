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
    UGameplayStatics::PlaySoundAtLocation(this,LunchSound,GetActorLocation());
}

void ABullet::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer1::StaticClass(), AllPlayer);
}
void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
    /* removign Health will be taken care of in the Health system Component */
    if(GetOwner()== nullptr){
        return;
    }
    ParticleTrail->SetHiddenInGame(true);
    
    if(OtherActor && (OtherActor !=GetOwner()->GetOwner())){
        if(GetOwner() != nullptr){
            AGun* CurrentGun=Cast<AGun>(GetOwner());
            if(CurrentGun->GetGunType() != GunType::RocketLuncher){
                UGameplayStatics::ApplyDamage(
                    OtherActor,
                    CurrentGun->GunDamage,
                    GetOwner()->GetInstigatorController(),
                    GetOwner()->GetOwner(),
                    DamageType);
                UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
                UGameplayStatics::SpawnEmitterAtLocation(this,HitParticle, GetActorLocation());
            }else{
                // UGameplayStatics::ApplyRadialDamage(GetWorld(),CurrentGun->GunDamage,GetActorLocation(), CurrentGun->DamageRadiusRokcetLuncher,DamageType,Ignore,GetOwner()->GetOwner(),GetOwner()->GetOwner()->GetInstigatorController(),true);
                for(AActor* Players: AllPlayer){
                    if(Players->IsA(APlayer1::StaticClass())){
                        if(Players == GetOwner()->GetOwner()){
                            continue;
                        }
                        if(FVector::Dist(this->GetActorLocation(),Players->GetActorLocation())  <=CurrentGun->DamageRadiusRokcetLuncher){
                            UGameplayStatics::ApplyDamage(Players,CurrentGun->GunDamage,GetOwner()->GetOwner()->GetInstigatorController(),this,DamageType);
                        }
                    }
                }
            }
        }
    }
    this->Destroy();
}