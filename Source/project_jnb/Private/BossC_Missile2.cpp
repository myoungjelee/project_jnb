// Fill out your copyright notice in the Description page of Project Settings.


#include "BossC_Missile2.h"
#include <Components/StaticMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include "MainPlayer.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ABossC_Missile2::ABossC_Missile2()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	compCapsule = CreateDefaultSubobject <UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(compCapsule);
	compCapsule->SetCollisionProfileName(TEXT("MissilePreset"));
	compCapsule->SetRelativeScale3D(FVector(4));

	compMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(compCapsule);
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));
	compMesh->SetRelativeLocation(FVector(-47, 20, -70));
	compMesh->SetRelativeRotation(FRotator(0, 90, 0));
	compMesh->SetRelativeScale3D(FVector(0.05));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/Assets/Boss/BossC/BossC_Missile2/source/BossC_Missile2.BossC_Missile2'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ABossC_Missile2::BeginPlay()
{
	Super::BeginPlay();

	AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass());
	mainPlayer = Cast<AMainPlayer>(player);

	if (player != nullptr)
	{
		dir = player->GetActorLocation() - GetActorLocation();
		dir.Normalize();
		rot = UKismetMathLibrary::MakeRotFromXZ(FVector::ForwardVector, dir);
		SetActorRotation(rot);
	}
	else
	{
		dir = -GetActorUpVector();
	}
}

// Called every frame
void ABossC_Missile2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector p0 = GetActorLocation();
	FVector vt = dir * speed * DeltaTime;

	SetActorLocation(p0 + vt);
}

