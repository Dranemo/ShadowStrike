#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Pawn.h"
#include "EnemyPawn.generated.h"

UCLASS()
class SHADOWSTRIKE_API AEnemyPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	AEnemyPawn();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "AI Properties")
	TArray<FVector> ControlPoint;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLightComponent;
};
