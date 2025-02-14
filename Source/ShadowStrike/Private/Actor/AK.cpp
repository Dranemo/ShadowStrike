// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AK.h"

#include "Actor/Bullet.h"
#include "Kismet/GameplayStatics.h"

AAK::AAK()
{
	BulletSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPoint"));
	BulletSpawnLocation->SetupAttachment(WeaponMesh);
	
}


void AAK::Fire()
{
	Super::Fire();

	if(Bullet && BulletSpawnLocation)
	{
		FTransform TransformBullet(this->GetActorRotation() + FRotator(90, 0, 0), BulletSpawnLocation->GetComponentLocation(), BulletScale);
		
		ABullet* InstanciatedBullet = GetWorld()->SpawnActor<ABullet>(Bullet, TransformBullet);
		InstanciatedBullet->SetOwner(this);

		UGameplayStatics::PlaySoundAtLocation(this, ShootSound, TransformBullet.GetLocation());
	}
}