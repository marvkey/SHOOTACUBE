// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent(){
	PrimaryComponentTick.bCanEverTick = true;
	Health=MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay(){
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
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
		GetOwner()->Destroy();
	}
}

void UHealthComponent::TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser){
	if(UseShields ==true){
		TakeDamageShield(DamageActor,Damage,DamageType,InstigatedBy,DamageCauser);
	}else{
		Health-=Damage;
	}
	GEngine->AddOnScreenDebugMessage(-1,3,FColor::Blue,FString::Printf(TEXT("Health is now %f"),Health));
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

