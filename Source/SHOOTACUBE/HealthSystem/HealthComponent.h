// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTACUBE_API UHealthComponent : public UActorComponent{
	 GENERATED_BODY()
public:	
	UHealthComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, Category="Health")
	bool UseShields =true;
	
	UPROPERTY(EditAnywhere, Category="Health")
	float Health;

	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth=100;
	
	UPROPERTY(EditAnywhere, Category="Health",meta=(EditCondition="UseShields"))
	float ShieldMaxHealth=200;
	
	UPROPERTY(EditAnywhere, Category="Health",meta=(EditCondition="UseShields"))
	float ShieldHealth;
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
    void TakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
private:
	void KillOwner();
	void TakeDamageShield(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);
};