// Fill out your copyright notice in the Description page of Project Settings.

#include "Accessories.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
// Sets default values
AAccessories::AAccessories(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collider =CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent=Collider;
	SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
}

