// Fill out your copyright notice in the Description page of Project Settings.


#include "guidedBullet.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>

// Sets default values
AguidedBullet::AguidedBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	gSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	gSphereComp->SetCollisionProfileName(TEXT("BulletPreset"));
	gSphereComp->SetSphereRadius(13);
	RootComponent = gSphereComp;

	
	gMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	gMeshComp->SetupAttachment(gSphereComp);
	gMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	gMeshComp->SetRelativeScale3D(FVector(0.26f));
	gMeshComp->SetRelativeLocation(FVector(0, 0, -13));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (tempMesh.Succeeded())
	{
		gMeshComp->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AguidedBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AguidedBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AguidedBullet::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->GetName().Contains(TEXT("BossC"))) //bossC 라고 하면 보스랑 미사일이랑 둘 다 포함아닌가? 왜 안맞지
	{
		//crashEvent();
		OtherActor->Destroy();
	}
	else
	{
		//crashEvent();
		OtherActor->Destroy();
	}
}