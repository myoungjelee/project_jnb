// Fill out your copyright notice in the Description page of Project Settings.											


#include "BossB_Missile.h"											
#include <Components/BoxComponent.h>											
#include <Components/StaticMeshComponent.h>											
#include <Kismet/GameplayStatics.h>
#include "MainPlayer.h"
#include <Components/SkeletalMeshComponent.h>

// Sets default values											
ABossB_Missile::ABossB_Missile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.										
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject <UBoxComponent>(TEXT("Box"));
	SetRootComponent(compBox);
	compBox->SetBoxExtent(FVector(25, 40, 40));
	compBox->SetCollisionProfileName(TEXT("MissilePreset"));

	compMesh = CreateDefaultSubobject <USkeletalMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(compBox);
	compMesh->SetRelativeScale3D(FVector(3));
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));
	compMesh->SetRelativeRotation(FRotator(0, 90, 90));
	compMesh->SetRelativeLocation(FVector(0, 0, 0));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Boss/BossB/BossB_Missile/source/BossB_Missile.BossB_Missile'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetSkeletalMesh(tempMesh.Object);
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("AnimBlueprint'/Game/ABP/ABP_BossB_Missile.ABP_BossB_Missile_C'"));
	if (tempAnim.Succeeded())
	{
		compMesh->SetAnimInstanceClass(tempAnim.Class);
	}

	InitialLifeSpan = 5;
}

// Called when the game starts or when spawned											
void ABossB_Missile::BeginPlay()
{
	Super::BeginPlay();

	int32 rand = FMath::RandRange(1, 100);

	if (rand > 50)
	{
		compMesh->SetVisibility(false);
		Destroy();
	}
	else
	{
		dir = -GetActorUpVector();
	}

}

// Called every frame											
void ABossB_Missile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector p0 = GetActorLocation();
	FVector vt = dir * speed * DeltaTime;
	SetActorLocation(p0 + vt);
}


