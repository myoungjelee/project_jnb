// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyZone2.h"
#include <Components/BoxComponent.h>

// Sets default values
ADestroyZone2::ADestroyZone2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject <UBoxComponent>(TEXT("Box"));
	SetRootComponent(compBox);
	compBox->SetBoxExtent(FVector(50, 50, 50));
	//compBox->SetBoxExtent(FVector(50, 2000, 50));
	compBox->SetCollisionProfileName(TEXT("DestroyZonePreset"));
}

// Called when the game starts or when spawned
void ADestroyZone2::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADestroyZone2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestroyZone2::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//¸¸¾à¿¡ ºÎµúÈù ³ðÀÌ Bullet ÀÌ¸é
	if (OtherActor->GetName().Contains(TEXT("Bullet")))
	{
		OtherActor->Destroy();
	}
	//±×·¸Áö ¾ÊÀ¸¸é
	else if (OtherActor->GetName().Contains(TEXT("Missile")))
	{
		//ºÎµúÈù ³ðÀ» ÆÄ±«ÇÏÀÚ
		OtherActor->Destroy();
	}
}