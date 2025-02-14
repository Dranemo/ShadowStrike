#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Bullet.generated.h"

UCLASS()
class SHADOWSTRIKE_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletProperties", meta = (AllowPrivateAccess = "true"))
	float LifeTime = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BulletProperties", meta = (AllowPrivateAccess = "true"))
	FVector ImpulseForce = FVector(20000, 20000, 20000);
	
	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* NiagaraEffect;
	FTimerHandle NiagaraHandle;
	
	FTimerHandle BulletTimerHandle;
	void DestroyBullet();

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
