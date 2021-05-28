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
#include "SHOOTACUBE/Chest/Chest.h"
#include "Components/BoxComponent.h"
#include <vector>
// Sets default values
APlayer1::APlayer1(){
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(GetMesh());
	Camera= CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	PlayerPerspective = CreateDefaultSubobject<UBoxComponent>(TEXT("Player View Perspective"));
	PlayerPerspective->SetupAttachment(SpringArm);
	HealthComponent=CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay(){
	Super::BeginPlay();
	FirstPlayer=Cast<APlayer1>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if(bIsAi ==true){
		RandomNumber=FMath::RandRange(0,SpawningGuns.Num()-1);
		AIGun=GetWorld()->SpawnActor<AGun>(SpawningGuns[RandomNumber]);
		AIGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightArm"));
		this->GetController()->SetActorTickEnabled(true);
	}
}
// Called every frame
void APlayer1::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	PlayerPerspective->GetOverlappingActors(OverllapingActors);
	GetAttachedActors(ChildActors,true);
	if(Slot1 == nullptr && Slot2 ==nullptr && Slot3 == nullptr ){
		CurrentWeaponSlot =0;
	}
	/* THIS IS IT */
	if(bIsAi ==true){
		if(AIGun != nullptr){
			FRotator GunRotation(-this->GetControlRotation().Pitch,AIGun->GetActorRotation().Yaw,AIGun->GetActorRotation().Roll);
        	AIGun->SetActorRotation(GunRotation);
			if(AIGun->MagazineSizeMinused != AIGun->MagazineSize){
				AIGun->MagazineSizeMinused=AIGun->MagazineSize;
			}
		}
	}
	
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
	PlayerInputComponent->BindAction(TEXT("PickUpGun"),EInputEvent::IE_Pressed,this, &APlayer1::PickUpItem);
	PlayerInputComponent->BindAction(TEXT("ShootGun"),EInputEvent::IE_Pressed,this,&APlayer1::ShootWeapon);
	PlayerInputComponent->BindAction(TEXT("RelodeGun"),EInputEvent::IE_Pressed,this,&APlayer1::RelodeWeapon);
	PlayerInputComponent->BindAction(TEXT("GoToInventory1"),EInputEvent::IE_Pressed,this,&APlayer1::SwitchWeaponItemSlot1);
	PlayerInputComponent->BindAction(TEXT("GoToInventory2"),EInputEvent::IE_Pressed,this,&APlayer1::SwitchWeaponItemSlot2);
	PlayerInputComponent->BindAction(TEXT("GoToInventory3"),EInputEvent::IE_Pressed,this,&APlayer1::SwitchWeaponItemSlot3);
	PlayerInputComponent->BindAction(TEXT("IncreaseInventorySlot"),EInputEvent::IE_Pressed,this,&APlayer1::IncreaseWeaponSlot);
	PlayerInputComponent->BindAction(TEXT("DecreaseInventorySlot"),EInputEvent::IE_Pressed,this,&APlayer1::DecreaseWeaponSlot);
	PlayerInputComponent->BindAction(TEXT("OpenChest"),EInputEvent::IE_Pressed,this,&APlayer1::OpenChest);

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

void APlayer1::EndPlay(const EEndPlayReason::Type EndPlayReason){
	Super::EndPlay(EndPlayReason);
}

void APlayer1::MoveX(float AxisValue) {
	APawn::AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayer1::MoveY(float AxisValue) {
	APawn::AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayer1::PCRotateX(float AxisValue) {
	APawn::AddControllerYawInput(AxisValue * PcRotateX * GetWorld()->GetDeltaSeconds());
}

void APlayer1::PCRotateY(float AxisValue) {
	APawn::AddControllerPitchInput(AxisValue * PcRotateY * GetWorld()->GetDeltaSeconds());
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
	if(CurrentGun != nullptr)
		return CurrentGun->GunDamage;
	return 0;
}

int32 APlayer1::GetGunRelodeRate(){
	if(CurrentGun != nullptr)
		return CurrentGun->RelodeTime;
	return 0;
}

UTexture2D* APlayer1::FirstSlot(){
	if(Slot1 != nullptr){
		AGun * GunCast=Cast<AGun>(Slot1);
		return GunCast->Image;
	}
	return EmptyInventory;
}

UTexture2D* APlayer1::SecondSlot(){
	if(Slot2 != nullptr){
		AGun * GunCast=Cast<AGun>(Slot2);
		return GunCast->Image;
	}
	return EmptyInventory;
}

UTexture2D* APlayer1::ThirdSlot(){
	if(Slot3 !=nullptr ){
		AGun * GunCast=Cast<AGun>(Slot3);
		return GunCast->Image;
	}
	return EmptyInventory;
}

void APlayer1::ControllerRotateX(float AxisValue) {
	APawn::AddControllerYawInput(AxisValue * PlayerControllerRotateX * GetWorld()->GetDeltaSeconds());
}

void APlayer1::ControllerRotateY(float AxisValue) {
	APawn::AddControllerPitchInput(AxisValue *PlayerControllerRotateY * GetWorld()->GetDeltaSeconds());
}

void APlayer1::PickUpItem(){
	for(AActor* GunOverllap:OverllapingActors){
		if(GunOverllap->IsA(AChest::StaticClass())){ // we do not want to pick up gun when overlapping a chest
			break;
		}
		if(GunOverllap->IsA(AAccessories::StaticClass()) && CheckIfChildActor(GunOverllap)==false){
			if(GunOverllap->GetOwner() != nullptr){return;}
			if(Slot1 == nullptr){
				Slot1=Cast<AAccessories>(GunOverllap);
				Slot1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightArm"));
				Slot1->SetOwner(this);
				Slot1->SetActorHiddenInGame(true);
				return;
			}
			if(Slot2 == nullptr){
				Slot2 =Cast<AAccessories>(GunOverllap);
				Slot2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightArm"));
				Slot2->SetOwner(this);
				Slot2->SetActorHiddenInGame(true);
				return;
			}
			if(Slot3 == nullptr){
				Slot3 = Cast<AAccessories>(GunOverllap);
				Slot3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightArm"));
				Slot3->SetOwner(this);
				Slot3->SetActorHiddenInGame(true);
				break;
			}else{
				UE_LOG(LogTemp,Warning,TEXT("all weapon slots are full"));
				return;
			}
		} 
	}
}

void APlayer1::ShootWeapon(){
	if(CurrentGun != nullptr){
		CurrentGun->Shoot();
		return;
	}
	if(bIsAi == true && AIGun!=nullptr){
		AIGun->Shoot();
		return;
	}
}

void APlayer1::RelodeWeapon(){
	if(CurrentGun != nullptr)
		CurrentGun->RelodeGun(CurrentGun,SmallBulletsAmmo,MediumBulletAmmo,RocketLuncherBulletAmmo);
}

void APlayer1::SwitchWeaponItemSlot1(){
	if(Slot1 != nullptr && CurrentWeaponSlot !=1){
		CurrentWeaponSlot= 1;
		CurrentGun = Cast<AGun>(Slot1);
		CurrentGun->SetActorHiddenInGame(false);
		ActiveSlot =Slot1;
		if(Slot2 != nullptr)
			Slot2->SetActorHiddenInGame(true);
		if(Slot3 != nullptr)
			Slot3->SetActorHiddenInGame(true);
	}
	
}
void APlayer1::SwitchWeaponItemSlot2(){
	if(Slot2 != nullptr && CurrentWeaponSlot != 2){
		CurrentWeaponSlot =2;
		CurrentGun = Cast<AGun>(Slot2);
		ActiveSlot =Slot2;
		CurrentGun->SetActorHiddenInGame(false);
		if(Slot1 != nullptr)
			Slot1->SetActorHiddenInGame(true);
		if(Slot3 != nullptr)
			Slot3->SetActorHiddenInGame(true);
	}
}
	
void APlayer1::SwitchWeaponItemSlot3(){
	if(Slot3 != nullptr && CurrentWeaponSlot != 3){
		CurrentWeaponSlot =3;
		CurrentGun = Cast<AGun>(Slot3);
		ActiveSlot =Slot3;
		CurrentGun->SetActorHiddenInGame(false);
		if(Slot1 != nullptr)
			Slot1->SetActorHiddenInGame(true);
		if(Slot2 != nullptr)
			Slot2->SetActorHiddenInGame(true);
	}
}

void APlayer1::RemoveGun(){
	if(CurrentGun != nullptr){
		CurrentGun->SetOwner(nullptr);
		CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentGun=nullptr;
		if(ActiveSlot ==Slot1)
			Slot1 =nullptr;
		else if(ActiveSlot == Slot2)
			Slot2 =nullptr;
		else if(ActiveSlot == Slot3)
			Slot3 =nullptr;
		CurrentWeaponSlot =0;  // we need to implement go to a slot when we remove a weapon 
	}
}

void APlayer1::IncreaseWeaponSlot(){
	if(Slot1 != nullptr && (Slot1->IsA(AGun::StaticClass()) &&  (CurrentWeaponSlot ==3 || CurrentWeaponSlot ==0) )){ // so we can go abck if hte weapon slot is to bigg
		if(Slot1 != nullptr){
			CurrentWeaponSlot= 1;
			CurrentGun = Cast<AGun>(Slot1);
			CurrentGun->SetActorHiddenInGame(false);
			ActiveSlot =Slot1;
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}

		else if(Slot2 != nullptr){
			CurrentWeaponSlot =2;
			CurrentGun = Cast<AGun>(Slot2);
			ActiveSlot =Slot2;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}

		else if(Slot3 != nullptr){
			CurrentWeaponSlot =3;
			CurrentGun = Cast<AGun>(Slot3);
			ActiveSlot =Slot3;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			return;
		}
	}
	else if(Slot2 != nullptr && Slot2->IsA(AGun::StaticClass()) && CurrentWeaponSlot==1){
		if(Slot2 != nullptr){
			CurrentWeaponSlot =2;
			CurrentGun = Cast<AGun>(Slot2);
			ActiveSlot =Slot2;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}

		else if(Slot3 != nullptr){
			CurrentWeaponSlot =3;
			CurrentGun = Cast<AGun>(Slot3);
			ActiveSlot =Slot3;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			return;
		}

		else if(Slot1 != nullptr){
			CurrentWeaponSlot= 1;
			CurrentGun = Cast<AGun>(Slot1);
			ActiveSlot =Slot1;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}
	}

	else if(Slot3 != nullptr && (Slot3->IsA(AGun::StaticClass()) )){
		if(Slot3 != nullptr){
			CurrentWeaponSlot =3;
			CurrentGun = Cast<AGun>(Slot3);
			ActiveSlot =Slot3;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			return;
		}
		else if(Slot1 != nullptr){
			CurrentWeaponSlot= 1;
			CurrentGun = Cast<AGun>(Slot1);
			ActiveSlot =Slot1;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}

		else if(Slot2 != nullptr){
			CurrentWeaponSlot =3;
			CurrentGun = Cast<AGun>(Slot3);
			ActiveSlot =Slot2;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}
	}
}

void APlayer1::DecreaseWeaponSlot(){
	if(Slot3 != nullptr && Slot3->IsA(AGun::StaticClass()) && (CurrentWeaponSlot ==3 || CurrentWeaponSlot ==0)){ // 2 and 3 because if we set it to 3 so if positive applied it points to 1
		// then when it is 3 we can say that here you go point to the first
		if(Slot2 != nullptr){
			CurrentWeaponSlot =2;
			CurrentGun = Cast<AGun>(Slot2);
			ActiveSlot =Slot2;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}
		
		else if(Slot1 != nullptr){
			CurrentWeaponSlot =1;
			CurrentGun = Cast<AGun>(Slot1);
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}
		return;
	}
	
	else if(Slot2 != nullptr && Slot2->IsA(AGun::StaticClass()) && CurrentWeaponSlot ==2){
		if(Slot1 != nullptr){
			CurrentWeaponSlot =1;
			CurrentGun = Cast<AGun>(Slot1);
			ActiveSlot =Slot1;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}
		
		else if(Slot3 != nullptr){
			CurrentWeaponSlot =3;
			CurrentGun = Cast<AGun>(Slot3);
			ActiveSlot =Slot3;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			return;
		}
		return;
	}
	
	else if(Slot1 != nullptr && Slot1->IsA(AGun::StaticClass()) && CurrentWeaponSlot ==1){
		if(Slot3 != nullptr){
			CurrentWeaponSlot =3;
			CurrentGun = Cast<AGun>(Slot3);
			ActiveSlot =Slot3;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot2 != nullptr)
				Slot2->SetActorHiddenInGame(true);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			return;
		}

		else if(Slot2 != nullptr){
			CurrentWeaponSlot =2;
			CurrentGun = Cast<AGun>(Slot2);
			ActiveSlot =Slot2;
			CurrentGun->SetActorHiddenInGame(false);
			if(Slot1 != nullptr)
				Slot1->SetActorHiddenInGame(true);
			if(Slot3 != nullptr)
				Slot3->SetActorHiddenInGame(true);
			return;
		}
		return;
	}
	

}

void APlayer1::OpenChest(){
	for(AActor* Overlapped:OverllapingActors){
		if(Overlapped->IsA(AGun::StaticClass()) && CheckIfChildActor(Overlapped) ==false){
			break;
		}
		if(Overlapped->IsA(AChest::StaticClass())){ // we do not want to pick up gun when overlapping a chest
			AChest* Chest = Cast<AChest>(Overlapped);
			Chest->Open();			
			break;
		}
	}
}

bool APlayer1::CheckIfChildActor(AActor* Actor){
	if(Actor == nullptr)return false;
	for(AActor* AllActor: ChildActors){
		if(AllActor == Actor){
			return true;
		}
	}
	return false;
}

void APlayer1::SpawnOnDestroy(){
	UE_LOG(LogTemp,Warning,TEXT("actor destoyed"));
	if(bIsAi == true){
		if(bIsAi == true){
			RandomNumber=FMath::RandRange(0,SpawningGuns.Num()-1);
			GetWorld()->SpawnActor<AActor>(ItemSPawnWhenDead[RandomNumber],GetActorLocation(),{0.0f,0.0f,0.0f});
		}
		AIGun->Destroy();
	}
}
