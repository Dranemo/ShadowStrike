// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Bullet.h"
#include "Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraComponent* NiagaraComp =UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),           
		NiagaraEffect,          
		GetActorLocation(),     
		GetActorLocation().ForwardVector.Rotation()  
	);

	GetWorldTimerManager().SetTimer(NiagaraHandle,  FTimerDelegate::CreateLambda([this, NiagaraComp]()
	{
		DestroyNiagara(NiagaraComp);
	}), 0.1, false);
	
	GetWorld()->GetTimerManager().SetTimer(BulletTimerHandle, this, &ABullet::DestroyBullet, LifeTime, false);
	BaseMesh->AddImpulse(ImpulseForce * BaseMesh->GetUpVector());
}
void ABullet::DestroyNiagara(UNiagaraComponent* NiagaraComp)
{
	NiagaraComp->Deactivate();
}

void ABullet::DestroyBullet()
{
	Destroy();
}

void ABullet::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
	FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);


	

	ABaseCharacter* otherPawn = Cast<ABaseCharacter>(Other);
	
	if(Other != GetOwner()->GetOwner() && otherPawn)
	{
		otherPawn->Die();
		DestroyBullet();
	}
}



