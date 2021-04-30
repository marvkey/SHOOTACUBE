// Fill out your copyright notice in the Description page of Project Settings.
#include "Player1.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SHOOTACUBE/Accessories/Gun/Gun.h"
#include "SHOOTACUBE/HealthSystem/HealthComponent.h"
#include "SHOOTACUBE/Accessories/Accessories.h"
#include "Engine/Texture2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
APlayer1::APlayer1(){
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(GetMesh());
	Camera= CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	HealthComponent=CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay(){
	Super::BeginPlay();
	/*
	FirstPlayer=Cast<APlayer1>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if(bIsAi ==true){
		RandomNumber=FMath::RandRange(0,SpawningGuns.Num()-1);
		AIGun=GetWorld()->SpawnActor<AGun>(SpawningGuns[RandomNumber]);
		AIGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightArm"));
	}
	*/
}
// Called every frame
void APlayer1::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	GetAttachedActors(ChildActors,true);
		/*
		if(bIsAi ==true){
			if(AIGun != nullptr){
				FRotator GunRotation(-this->GetControlRotation().Pitch,AIGun->GetActorRotation().Yaw,AIGun->GetActorRotation().Roll);
        		AIGun->SetActorRotation(GunRotation);
				if(AIGun->MagazineSizeMinused != AIGun->MagazineSize){
					AIGun->MagazineSizeMinused=AIGun->MagazineSize;
				}
			}
		}
		*/
	if(CurrentGun != nullptr){
     	FRotator GunRotation(-this->GetControlRotation().Pitch,CurrentGun->GetActorRotation().Yaw,CurrentGun->GetActorRotation().Roll);
		CurrentGun->SetActorRotation(GunRotation);
		CurrentGun->SetActorHiddenInGame(false);
	}
}

// Called to bind functionality to input
void APlayer1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveX"), this, &APlayer1::MoveX);
	PlayerInputComponent->BindAxis(TEXT("MoveY"), this, &APlayer1::MoveY);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PickUpGun"),EInputEvent::IE_Pressed,this, &APlayer1::PickUpWeapon);
	PlayerInputComponent->BindAction(TEXT("ShootGun"),EInputEvent::IE_Pressed,this,&APlayer1::ShootWeapon);
	PlayerInputComponent->BindAction(TEXT("RelodeGun"),EInputEvent::IE_Pressed,this,&APlayer1::RelodeWeapon);
	PlayerInputComponent->BindAction(TEXT("Print Inventory"),EInputEvent::IE_Pressed,this,&APlayer1::PrintInventory);
	PlayerInputComponent->BindAction(TEXT("GoToInventory1"),EInputEvent::IE_Pressed,this,&APlayer1::SwitchWeaponItemSlot1);
	PlayerInputComponent->BindAction(TEXT("GoToInventory2"),EInputEvent::IE_Pressed,this,&APlayer1::SwitchWeaponItemSlot2);
	PlayerInputComponent->BindAction(TEXT("GoToInventory3"),EInputEvent::IE_Pressed,this,&APlayer1::SwitchWeaponItemSlot3);
	PlayerInputComponent->BindAction(TEXT("RemoveGun"), EInputEvent::IE_Pressed,this,&APlayer1::RemoveGun);
	// PC
	PlayerInputComponent->BindAxis(TEXT("PCRotateX"), this, &APlayer1::PCRotateX);
	PlayerInputComponent->BindAxis(TEXT("PCRotateY"), this, &APlayer1::PCRotateY);
	// Controller
	PlayerInputComponent->BindAxis(TEXT("ControllerRotateX"), this, &APlayer1::ControllerRotateX);
	PlayerInputComponent->BindAxis(TEXT("ControllerRotateY"), this, &APlayer1::ControllerRotateY);
}

void APlayer1::SetPlayerHealth(bool KilledPlayer){
	if(KilledPlayer==false){
		HealthComponent->Health+=10;
		HealthComponent->ShieldHealth+=20;
	}
}
void APlayer1::AddToInventory(AAccessories* Actor){
	Inventory.Add(Actor);
}
void APlayer1::PrintInventory(){
	FString SInventory ="";
	for (AAccessories* Actor :Inventory){
		SInventory.Append(Actor->Name);
		SInventory.Append(" | ");
	}
	GEngine->AddOnScreenDebugMessage(1,5,FColor::Blue,*SInventory);
}

void APlayer1::EndPlay(const EEndPlayReason::Type EndPlayReason){
	if(bIsAi == true){
		//FirstPlayer->SetPlayerHealth(false);
	}
}

void APlayer1::MoveX(float AxisValue) {
	APawn::AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayer1::MoveY(float AxisValue) {
	APawn::AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayer1::PCRotateX(float AxisValue) {
	APawn::AddControllerYawInput(AxisValue * PcRotateX * GetWorld()->GetDeltaSeconds()*10);
}

void APlayer1::PCRotateY(float AxisValue) {
	APawn::AddControllerPitchInput(AxisValue * PcRotateY * GetWorld()->GetDeltaSeconds()*10);
}

float APlayer1::GetHealthRemaining(){
	return HealthComponent->Health;
}

float APlayer1::GetHealthPercent(){
	return HealthComponent->Health/HealthComponent->MaxHealth;
}

float APlayer1::GetShieldRemaining(){
	if( HealthComponent->UseShields ==true)
		return HealthComponent->ShieldHealth;
	return  0;
}

float APlayer1::GetShieldPercent(){
	if( HealthComponent->UseShields ==true)
		return HealthComponent->ShieldHealth/HealthComponent->ShieldMaxHealth;
	return 0;
}

int32 APlayer1::GetMagazieSizeUnchaged(){
	if(CurrentGun != nullptr)
		return CurrentGun->MagazineSize;
	return 0;
}

int32 APlayer1::GetMagazineSizeChange(){
	if(CurrentGun != nullptr)
		return CurrentGun->MagazineSizeMinused;
	return  0;
}

int32 APlayer1::GetGunDamage(){
	return CurrentGun->GunDamage;
}

int32 APlayer1::GetGunRelodeRate(){
	return CurrentGun->RelodeTime;
}

UTexture2D* APlayer1::FirstSlot(){
	if(ChildActors.Num() >=1){
		AGun * GunCast=Cast<AGun>(ChildActors[0]);
		return GunCast->Image;
	}
	return nullptr;
}

UTexture2D* APlayer1::SecondSlot(){
	if(ChildActors.Num() >= 2){
		AGun * GunCast=Cast<AGun>(ChildActors[1]);
		return GunCast->Image;
	}
	return nullptr;
}

UTexture2D* APlayer1::ThirdSlot(){
	if(ChildActors.Num() >= 3){
		AGun * GunCast=Cast<AGun>(ChildActors[2]);
		return GunCast->Image;
	}
	return nullptr;
}

void APlayer1::ControllerRotateX(float AxisValue) {
	APawn::AddControllerYawInput(AxisValue * PlayerControllerRotateX * GetWorld()->GetDeltaSeconds());
}

void APlayer1::ControllerRotateY(float AxisValue) {
	APawn::AddControllerPitchInput(AxisValue *PlayerControllerRotateY * GetWorld()->GetDeltaSeconds());
}

void APlayer1::PickUpWeapon(){
	GetOverlappingActors(OverllapingActors);
	for(AActor* GunOverllap:OverllapingActors){
		if(GunOverllap->IsA(AGun::StaticClass())){
			NotCurrentGun=Cast<AGun>(GunOverllap);
			NotCurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightArm"));
			NotCurrentGun->SetOwner(this);
			NotCurrentGun->SetActorHiddenInGame(true);
		} 
	}
}

void APlayer1::ShootWeapon(){
	if(CurrentGun != nullptr){
		if(bIsAi==true)
			AIGun->Shoot();
		else
			CurrentGun->Shoot();
	}
}

void APlayer1::RelodeWeapon(){
	if(CurrentGun != nullptr)
		CurrentGun->RelodeGun(CurrentGun,SmallBulletsAmmo,MediumBulletAmmo,RocketLuncherBulletAmmo);
}

void APlayer1::SwitchWeaponItemSlot1(){
	if(ChildActors.Num() >=1){
		CurrentGun=Cast<AGun>(ChildActors[0]);
		NotCurrentGun=nullptr;
		if(ChildActors.Num() >=2){
			if(ChildActors[1] != nullptr)
				ChildActors[1]->SetActorHiddenInGame(true);
			if(ChildActors.Num() >= 3){
				if(ChildActors[2] != nullptr)
					ChildActors[2]->SetActorHiddenInGame(true);
			}
		}
	}
}
void APlayer1::SwitchWeaponItemSlot2(){
	if(ChildActors.Num() >= 2){
		CurrentGun=Cast<AGun>(ChildActors[1]);
		NotCurrentGun=nullptr;
		if(ChildActors[0] != nullptr){
			ChildActors[0]->SetActorHiddenInGame(true);
		}
		if(ChildActors.Num() >= 3){
			if(ChildActors[2] != nullptr){
				ChildActors[2]->SetActorHiddenInGame(true);
			}
		}
	}
}
	
void APlayer1::SwitchWeaponItemSlot3(){
	if(ChildActors.Num() >= 3){
		CurrentGun=Cast<AGun>(ChildActors[2]);
		NotCurrentGun=nullptr;
		if(ChildActors[0] != nullptr){
			ChildActors[0]->SetActorHiddenInGame(true);
		}
		if(ChildActors[1] != nullptr){
			ChildActors[1]->SetActorHiddenInGame(true);
		}
	}
}

void APlayer1::RemoveGun(){
	if(CurrentGun != nullptr){
		CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentGun=nullptr;
	}
}
		





