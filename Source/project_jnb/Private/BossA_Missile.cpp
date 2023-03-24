// Fill out your copyright notice in the Description page of Project Settings.

#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "BossA_Missile.h"
#include <Components/SkeletalMeshComponent.h>

// Sets default values
ABossA_Missile::ABossA_Missile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject <UBoxComponent>(TEXT("Box"));
	SetRootComponent(compBox);
	compBox->SetBoxExtent(FVector(30, 50, 30));
	compBox->SetCollisionProfileName(TEXT("MissilePreset"));
	compBox->SetRelativeScale3D(FVector(1.5));

	compMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(compBox);
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));
	compMesh->SetRelativeLocation(FVector(0, 45, 0));
	compMesh->SetRelativeRotation(FRotator(0, 180, 0));
	compMesh->SetRelativeScale3D(FVector(150));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Boss/BossA/BossA_Missile/source/BossA_Missile.BossA_Missile'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetSkeletalMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ABossA_Missile::BeginPlay()
{
	Super::BeginPlay();
	dir = -GetActorRightVector();
}

// Called every frame
void ABossA_Missile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector p0 = GetActorLocation();
	FVector vt = dir * speed * DeltaTime;
	SetActorLocation(p0 + vt); //this->SetActorLocation(p0 + vt);
}

