// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Knife.h"

#include "Components/BoxComponent.h"
#include "Character/BaseCharacter.h"

AKnife::AKnife()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(BoxComponent);
}

void AKnife::Fire()
{
	Super::Fire();

	TArray<AActor*> OverlappingActors;
	TriggerBox->GetOverlappingActors(OverlappingActors);

	for (auto OverlappingActor : OverlappingActors)
	{
		if(OverlappingActor != Owner)
		{
			
			if(ABaseCharacter* pawnActor = Cast<ABaseCharacter>(OverlappingActor))
			{
				pawnActor->Die();
			}

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue,  OverlappingActor->GetName());
		}
	}
}
