// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

#include <vector>

#include "GameFramework/Character.h"
#include "Player1.generated.h"
class APlayer1Controller;
class AAccessories;
UCLASS()
class SHOOTACUBE_API APlayer1 : public ACharacter{
	GENERATED_BODY()
public:
	friend class UHealthComponent;
	friend class APlayer1Controller;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShootWeapon();
	APlayer1();
	UFUNCTION(BlueprintPure) float GetHealthRemaining();
	
	UFUNCTION(BlueprintPure) float GetHealthPercent();
	
	UFUNCTION(BlueprintPure) float GetShieldRemaining();
	
	UFUNCTION(BlueprintPure) float GetShieldPercent();
	
	UFUNCTION(BlueprintPure) int32 GetMagazieSizeUnchaged();
	
	UFUNCTION(BlueprintPure) int32 GetMagazineSizeChange();
	
	UFUNCTION(BlueprintPure) int32 GetGunDamage();
	
	UFUNCTION(BlueprintPure) int32 GetGunRelodeRate();
	
	void SetPlayerHealth(bool KilledPlayer);
	
	UFUNCTION(BlueprintPure) int32 GetKills()const {return Kills;}
	void SetKills(int _Kill){Kills =_Kill;}
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite) float DamagaSize=0;
	void DamageSize(float &ToDamageSize){
		this->DamagaSize=ToDamageSize;
	}
	
	UFUNCTION(BlueprintPure) UTexture2D* FirstSlot();
	UFUNCTION(BlueprintPure) UTexture2D* SecondSlot();
	UFUNCTION(BlueprintPure) UTexture2D* ThirdSlot();
	
	UPROPERTY(EditAnywhere, Category="AI") bool bIsAi=false;
	
	class AGun* AIGun =nullptr;
	UPROPERTY(EditAnywhere, Category="AI",meta=(EditCondition="bIsAi")) TArray<TSubclassOf<AGun>>SpawningGuns;
	UPROPERTY(EditAnywhere,Category ="AI",meta=(EditCondition="bIsAi"))TArray<TSubclassOf<AActor>>ItemSPawnWhenDead;
	int RandomNumber;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite) FSlateBrush Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ammo") int SmallBulletsAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ammo") int MediumBulletAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Ammo") int RocketLuncherBulletAmmo;
	
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

	UPROPERTY(EditAnywhere,Category ="Components")
	class UBoxComponent* PlayerPerspective;
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
	void PickUpItem();
	void RelodeWeapon();
	void RemoveGun();
	int CurrentWeaponSlot =0;// not pointig at any item in the inventory
	// PC 
	void PCRotateX(float AxisValue);
	void PCRotateY(float AxisValue);
	void SwitchWeaponItemSlot1();
	void SwitchWeaponItemSlot2();
	void SwitchWeaponItemSlot3();
	//Controller
	void ControllerRotateX(float AxisValue);
	void ControllerRotateY(float AxisValue);
	void IncreaseWeaponSlot();
	void DecreaseWeaponSlot();
	void OpenChest();
	class AGun* CurrentGun=nullptr;
	class AGun* NotCurrentGun=nullptr;
	
	int32 Kills=0;
	
	AAccessories* Slot1;
	AAccessories* Slot2;
	AAccessories* Slot3;
	AAccessories* ActiveSlot;
	
	TArray<AActor*>OverllapingActors;
	TArray<AAccessories*>Inventory;
	TArray<AActor*>ChildActors;
	bool CheckIfChildActor(AActor* Actor);

	UPROPERTY(EditAnywhere, Category="Image")
	UTexture2D*EmptyInventory;
	void SpawnOnDestroy();
};
