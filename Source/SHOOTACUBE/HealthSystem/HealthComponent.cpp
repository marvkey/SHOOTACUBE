// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "SHOOTACUBE/Accessories/Gun/Bullet.h"
#include "SHOOTACUBE/Player/Player1.h"
// Sets default values for this component's properties
UHealthComponent::UHealthComponent(){
	PrimaryComponentTick.bCanEverTick = true;
	Health=MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay(){
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	//GetOwner()->OnTakeRadialDamage.AddDynamic(this,&UHealthComponent::TakeRadialDamage);
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	KillOwner();
	if(ShieldHealth >ShieldMaxHealth){
		 ShieldHealth=ShieldMaxHealth;
	}
	if(Health > MaxHealth){
		Health=MaxHealth;
	}
}

void UHealthComponent::KillOwner(){
	if(Health <=0){
		GetOwner()->GetWorld()->DestroyActor(GetOwner());
	}
}

void UHealthComponent::TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser){
	if(Damage >= ShieldHealth+Health){
		if(DamageCauser!=nullptr){
			if(DamageCauser->IsA(APlayer1::StaticClass())){
				APlayer1* PlayerSentDamage = Cast<APlayer1>(DamageCauser);
				PlayerSentDamage->SetKills(PlayerSentDamage->GetKills()+1);
			}
		}
		Health =0;
		ShieldHealth =0;
		if(GetOwner()->IsA(APlayer1::StaticClass())){
			APlayer1* Player =Cast<APlayer1>(GetOwner());
			Player->SpawnOnDestroy();
		}
		GetOwner()->GetWorld()->DestroyActor(GetOwner());
		return;
	}
	if(UseShields ==true){
		TakeDamageShield(DamageActor,Damage,DamageType,InstigatedBy,DamageCauser);
	}else{
		Health-=Damage;
	}
}

void UHealthComponent::TakeDamageShield(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser){
	if(UseShields ==true){
		if(ShieldHealth ==0){
			Health-=Damage;
		} else if(ShieldHealth >=  Damage){
			ShieldHealth-=Damage;
		} else if(ShieldHealth <Damage){
			float DifferenceShieldDamage= Damage-ShieldHealth;
			DifferenceShieldDamage-=ShieldHealth;
			ShieldHealth=0;
			Health-=DifferenceShieldDamage;
		}
	}
}

