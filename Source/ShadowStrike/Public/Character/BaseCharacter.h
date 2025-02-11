#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCapsuleComponent;

UCLASS()
class SHADOWSTRIKE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	
	virtual void Die();
	
protected:
	virtual void BeginPlay() override;
	virtual void Fire();
	virtual void Rotate(FVector LookAtTarget);

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 10.0f;

private:

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	float InterpSpeed = 2.f;
};
