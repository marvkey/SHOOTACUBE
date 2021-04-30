// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player1.generated.h"
class APlayer1Controller;
class AAccessories;
UCLASS()
class SHOOTACUBE_API APlayer1 : public ACharacter{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	APlayer1();
	UFUNCTION(BlueprintPure)
	float GetHealthRemaining();
	
	UFUNCTION(BlueprintPure)
    float GetHealthPercent();
	
	UFUNCTION(BlueprintPure)
    float GetShieldRemaining();
	
	UFUNCTION(BlueprintPure)
    float GetShieldPercent();
	
	UFUNCTION(BlueprintPure)
    int32 GetMagazieSizeUnchaged();
	
	UFUNCTION(BlueprintPure)
    int32 GetMagazineSizeChange();
	
	UFUNCTION(BlueprintPure)
    int32 GetGunDamage();
	
	UFUNCTION(BlueprintPure)
    int32 GetGunRelodeRate();
	
	void PickUpWeapon();
	void ShootWeapon();
	void RelodeWeapon();
	void SwitchWeaponItemSlot1();
	void SwitchWeaponItemSlot2();
	void SwitchWeaponItemSlot3();
	void RemoveGun();
	void SetPlayerHealth(bool KilledPlayer);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DamagaSize=0;
	void DamageSize(float &ToDamageSize){
		this->DamagaSize=ToDamageSize;
	}

	UFUNCTION(BlueprintPure)
    UTexture2D* FirstSlot();
	UFUNCTION(BlueprintPure)
    UTexture2D* SecondSlot();
	UFUNCTION(BlueprintPure)
    UTexture2D* ThirdSlot();
	
	UFUNCTION(BlueprintCallable)
    void PrintInventory();
	void AddToInventory(AAccessories* Actor);
	
	UPROPERTY(EditAnywhere, Category="AI")
	bool bIsAi=false;
	
	class AGun* AIGun =nullptr;
	UPROPERTY(EditAnywhere, Category="AI",meta=(EditCondition="bIsAi"))
	TArray<TSubclassOf<AGun>>SpawningGuns;
	
	int RandomNumber;
	friend class APlayer1Controller;
	int32 Kills=0;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FSlateBrush Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ammo")
	int SmallBulletsAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ammo")
	int MediumBulletAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ammo")
	int RocketLuncherBulletAmmo;
	int32 Switch =0;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category="PC",meta =(ClampMin="0.0", ClampMax="100.0"))
	float PcRotateX=10.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PC", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float PcRotateY=10.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Controller", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float PlayerControllerRotateX =100.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Controller", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float PlayerControllerRotateY =100.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category ="Components")
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, Category ="Components")
	class UHealthComponent* HealthComponent;
	APlayer1* FirstPlayer =nullptr;

private:
	virtual  void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// All platforms Function
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);
	// PC 
	void PCRotateX(float AxisValue);
	void PCRotateY(float AxisValue);
	//Controller
	void ControllerRotateX(float AxisValue);
	void ControllerRotateY(float AxisValue);
	class AGun* CurrentGun=nullptr;
	class AGun* NotCurrentGun=nullptr;
	
	TArray<AActor*>OverllapingActors;
	TArray<AAccessories*>Inventory;
	TArray<AActor*>ChildActors;
};
