// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Knife.h"

#include "Components/BoxComponent.h"
#include "Pawn/BasePawn.h"

AKnife::AKnife()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(WeaponMesh);
}

void AKnife::Fire()
{
	Super::Fire();

	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors);

	for (auto OverlappingActor : OverlappingActors)
	{
		if(ABasePawn* pawnActor = Cast<ABasePawn>(OverlappingActor))
		{
			pawnActor->Die();
		}
	}
}
