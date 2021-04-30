// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Accessories.generated.h"
class UTexture2D;
UCLASS()
class SHOOTACUBE_API AAccessories : public AActor{
	GENERATED_BODY()
public:	
	AAccessories();
	UPROPERTY(EditAnywhere)
	FString Name="Pistol";
	UPROPERTY(EditAnywhere, Category="Image")
	UTexture2D*Image;

protected:
	UPROPERTY(EditAnywhere, Category="Collider")
	class UBoxComponent*Collider;
	
	UPROPERTY(EditAnywhere, Category="Collider")
	class USkeletalMeshComponent* SkeletalMesh;
};
