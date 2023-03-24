// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_gn.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Engine/StaticMesh.h>

// Sets default values
ABullet_gn::ABullet_gn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	sphereComp->SetCollisionProfileName(TEXT("BulletPreset"));
	sphereComp->SetSphereRadius(13);
	RootComponent = sphereComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(sphereComp);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComp->SetRelativeScale3D(FVector(2.6f, 2.0f, 2.6f));
	MeshComp->SetRelativeLocation(FVector(0, 0, -1.3));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/BulletPistol.BulletPistol'"));
	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}

}

// Called when the game starts or when spawned
void ABullet_gn::BeginPlay()
{
	Super::BeginPlay();

	dir = FVector::RightVector;
}

// Called every frame
void ABullet_gn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float speed = 1000;
	//bool bFire;

	//if (bFire == 1) //이거는 bullet 말고 총알을 생성하는 mainplayer 에서 구현했어야 함
	//{
	//	//1. < 키가 눌려있고 bFire이 작동할 때 
	//	if (direction.Y == -1)
	//	{
	//		dir = -GetActorRightVector();
	//	}
	//	//2.
	//	if (direction.Y == -1 && direction.Z == 1)
	//	{
	//		dir = -GetActorRightVector()+ GetActorUpVector();
	//	}
	//	//3.
	//	if (direction.Z == 1)
	//	{
	//		dir = GetActorUpVector();
	//	}
	//	//4.
	//	if (direction.Z == 1 && direction.Y == 1)
	//	{
	//		dir = GetActorUpVector() + GetActorRightVector();
	//	}
	//	//5.
	//	if (direction.Y == 1)
	//	{
	//		dir = GetActorRightVector();
	//	}
	//}

	FVector p0 = GetActorLocation();
	FVector vt = speed * DeltaTime * GetActorRightVector();
		
	SetActorLocation(p0 + vt);

}

void ABullet_gn::NotifyActorBeginOverlap(AActor* OtherActor)
{
}



void ABullet_gn::crashEvent()
{
}