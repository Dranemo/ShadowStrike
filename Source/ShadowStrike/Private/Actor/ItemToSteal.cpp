// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ItemToSteal.h"

#include "MyGameInstance.h"
#include "Components/BoxComponent.h"

// Sets default values
AItemToSteal::AItemToSteal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = Box;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
	Mesh->SetupAttachment(Box);

}

void AItemToSteal::StealItem()
{
	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GI->StealItem();
	}
	Destroy();
}

void AItemToSteal::BeginPlay()
{
	Super::BeginPlay();

	if (UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance()))
	{
		GI->NbrItemToSteal ++;
	}
}

