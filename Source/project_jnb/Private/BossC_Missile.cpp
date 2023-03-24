// Fill out your copyright notice in the Description page of Project Settings.


#include "BossC_Missile.h"
#include <Components/StaticMeshComponent.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include "MainPlayer.h"
#include <Kismet/KismetMathLibrary.h>
#include <Sound/SoundBase.h>

// Sets default values
ABossC_Missile::ABossC_Missile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compCapsule = CreateDefaultSubobject <UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(compCapsule);
	compCapsule->SetCollisionProfileName(TEXT("Missile2Preset"));
	compCapsule->SetRelativeScale3D(FVector(1.5f));

	compMesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh"));
	compMesh->SetupAttachment(compCapsule);
	compMesh->SetCollisionProfileName(TEXT("NoCollision"));
	compMesh->SetRelativeRotation(FRotator(180, 45, 90));
	compMesh->SetRelativeScale3D(FVector(0.8));
	compMesh->SetRelativeLocation(FVector(10, -12, -15));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Game/Assets/Boss/BossC/BossC_Missile/source/BossC_Missile.BossC_Missile'"));
	if (tempMesh.Succeeded())
	{
		compMesh->SetStaticMesh(tempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/Audio/Missile.Missile'"));
	if (tempSound.Succeeded())
	{
		destroySound = tempSound.Object;
	}
}

// Called when the game starts or when spawned
void ABossC_Missile::BeginPlay()
{
	Super::BeginPlay();

	Hp = InitalHp;

	dir = -GetActorUpVector();
}

// Called every frame
void ABossC_Missile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Hp > 0)
	{
		AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass());
		mainPlayer = Cast<AMainPlayer>(player);

		if (player != nullptr)
		{
			dir = player->GetActorLocation() - GetActorLocation();
			dir.Normalize();

			FVector p0 = GetActorLocation();
			FVector vt = dir * 300 * DeltaTime;
			SetActorLocation(p0 + vt);

			rot = UKismetMathLibrary::MakeRotFromXZ(FVector::ForwardVector, dir);
			if (dir.Y > 0)
			{
				SetActorRotation(-1 * rot);
			}
			SetActorRotation(rot);
		}
		else
		{
			FVector p0 = GetActorLocation();
			FVector vt = dir * 200 * DeltaTime;
			SetActorLocation(p0 + vt);
		}
	}
	else
	{
	UGameplayStatics::PlaySound2D(GetWorld(),destroySound);
		Destroy();
	}
}

void ABossC_Missile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->GetName().Contains(TEXT("Bullet")))
	{
		OnHitEvent();
		OtherActor->Destroy();
	}
}

void ABossC_Missile::OnHitEvent()
{
	Hp--;
}