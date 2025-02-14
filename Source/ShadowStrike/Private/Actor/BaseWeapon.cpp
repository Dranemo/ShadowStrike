// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseWeapon.h"

#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon  mesh"));
	WeaponMesh->SetupAttachment(BoxComponent);

	LightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	LightComponent->SetupAttachment(BoxComponent);
	

}

void ABaseWeapon::Fire()
{
}

void ABaseWeapon::DestroyWeapon()
{
	Destroy();
}

void ABaseWeapon::SetLight(bool _bool)
{
	if(!_bool)
		LightComponent->SetIntensity(0);

	else
		LightComponent->SetIntensity(LightIntensity);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	SetLight(false);
}

