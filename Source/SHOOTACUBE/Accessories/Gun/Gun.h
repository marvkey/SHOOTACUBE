// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "SHOOTACUBE/Accessories/Accessories.h"
#include "SHOOTACUBE/Player/Player1.h"
#include "Gun.generated.h"
UENUM()
enum class AmmoType{
	SmallAmmo,
	MediumAmmo,
	RocketLuncherAmmo
};
UENUM()
enum class GunType{
	Sniper,
	ShotGun,
	Pistol,
	AssaultRifle,
	RocketLuncher
};
UCLASS()
class SHOOTACUBE_API AGun : public AAccessories
{
	GENERATED_BODY()
	public:
	virtual  void Tick(float DeltaSeconds) override;
	AGun();
	void Shoot();
	AmmoType GetAmmoOfGun();
	GunType GetGunType(){return TypeOfGun;}
	void RelodeGun(AGun* GunToRelode,int32 &SmallBulletsAmmo, int32 &MediumBulletAmmo, int32 &RocketLuncherAmmo);
	UPROPERTY(EditAnywhere, Category="GunStats")
    float GunDamage=10;
	
	UPROPERTY(EditAnywhere, Category="GunStats") float DamageRadiusRokcetLuncher;
	UPROPERTY(EditAnywhere, Category="GunStats") int32 MagazineSize=20;
	UPROPERTY(EditAnywhere, Category="GunStats") int32 MagazineSizeMinused;
	UPROPERTY(EditAnywhere,Category="Relode Time") float RelodeTime=2;
protected:
	virtual void BeginPlay() override;
private:
	void RelodeSmallBulletGun(AGun* GunToRelode, int32 &SmallBulletAmmo);
	void RelodeMediumBulletGun(AGun* GunToRelode, int32 &MediumBulletAmmo);
	void RelodeRocketLuncherGun(AGun* GunToRelode, int32 &RocketLuncherAmmo);
	void OnAttachToPlayer();
	UPROPERTY(EditAnywhere, Category="GunStats") float GunFireRate=2.0f;
	UPROPERTY(EditAnywhere, Category="Animation") class UAnimationAsset* RelodeAnimation;
	
	UPROPERTY(EditAnywhere, Category="GunStats") AmmoType AmmoOfGun=AmmoType::SmallAmmo;
	
	UPROPERTY(EditAnywhere, Category ="GunStats") GunType TypeOfGun = GunType::Pistol;
	
	UPROPERTY(EditAnywhere, Category="Ammo") USceneComponent* Bullet;
	UPROPERTY(EditAnywhere, Category="Ammo")TSubclassOf<class ABullet>BulletSpawned;
};