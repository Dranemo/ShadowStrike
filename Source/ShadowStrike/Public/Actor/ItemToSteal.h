// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemToSteal.generated.h"


class UPointLightComponent;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SHADOWSTRIKE_API AItemToSteal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemToSteal();

	void StealItem();



protected:
	
	void BeginPlay() override;

	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Box;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    	UPointLightComponent* LightComponent;
};
