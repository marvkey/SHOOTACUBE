// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "SHOOTACUBE/Accessories/Accessories.h"
#include "Bullet.generated.h"

UCLASS()
class SHOOTACUBE_API ABullet : public AAccessories{
	GENERATED_BODY()
public:
    virtual  void Tick(float DeltaSeconds) override;
	ABullet();
	UPROPERTY(EditAnywhere, Category="Component")
	class UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* LunchSound;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	class USoundBase* HitSound;
			
	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticle;

	UPROPERTY(BlueprintReadWrite)
	float DamageSize;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystemComponent* ParticleTrail;
	
protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	AActor* MyOwner=nullptr;
	AActor* MyOwnersOwner=nullptr;
	UPROPERTY(EditAnywhere, Category="Component")
	TSubclassOf<UDamageType>DamageType;

	FVector StartLocation;

};
