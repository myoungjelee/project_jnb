// Fill out your copyright notice in the Description page of Project Settings.											

#include <Components/BoxComponent.h>																					
#include "BossB.h"											
#include "BossB_Missile.h"
#include <Kismet/GameplayStatics.h>
#include "MainPlayer.h"
#include "BossD.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Animation/AnimInstance.h>
#include <Sound/SoundBase.h>
#include <Materials/MaterialParameterCollection.h>
#include <Kismet/KismetMaterialLibrary.h>
#include <Components/CapsuleComponent.h>
#include <Components/AudioComponent.h>
#include <Particles/ParticleSystem.h>
#include <Animation/AnimMontage.h>

// Sets default values											
ABossB::ABossB()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.										
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BossPreset"));
	GetCapsuleComponent()->SetRelativeScale3D(FVector(4));
	GetCapsuleComponent()->SetCapsuleHalfHeight(100);
	GetCapsuleComponent()->SetCapsuleRadius(40);

	GetMesh()->SetRelativeScale3D(FVector(4, 5, 5.5f));
	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -100));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Boss/BossB/BossB/source/BossB.BossB'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	ConstructorHelpers::FClassFinder<ABossB_Missile> tempMis(TEXT("Blueprint'/Game/Blueprints/BP_BossB_Missile.BP_BossB_Missile_C'"));
	if (tempMis.Succeeded())
	{
		MissileFactory = tempMis.Class;
	}

	ConstructorHelpers::FClassFinder<ABossD> tempBoss(TEXT("Blueprint'/Game/Blueprints/BP_BossD.BP_BossD_C'"));
	if (tempBoss.Succeeded())
	{
		BossD = tempBoss.Class;
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("AnimBlueprint'/Game/ABP/ABP_BossB.ABP_BossB_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound1(TEXT("SoundWave'/Game/Audio/BossB_Attack.BossB_Attack'"));
	if (tempSound1.Succeeded())
	{
		attackSound = tempSound1.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound2(TEXT("SoundWave'/Game/Audio/BossB_Start.BossB_Start'"));
	if (tempSound2.Succeeded())
	{
		startSound = tempSound2.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> tempColl(TEXT("MaterialParameterCollection'/Game/Assets/Boss/Boss_Emi.Boss_Emi'"));
	if (tempColl.Succeeded())
	{
		collection = tempColl.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> tempEffect(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempEffect.Succeeded())
	{
		effect = tempEffect.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMontage(TEXT("AnimMontage'/Game/ABP/AM_BossB.AM_BossB'"));
	if (tempMontage.Succeeded())
	{
		montage = tempMontage.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempHit(TEXT("SoundWave'/Game/Audio/BossHit.BossHit'"));
	if (tempHit.Succeeded())
	{
		hitSound = tempHit.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempEx(TEXT("SoundWave'/Game/Audio/BossHitEx.BossHitEx'"));
	if (tempEx.Succeeded())
	{
		exSound = tempEx.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> tempCri(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (tempCri.Succeeded())
	{
		criEffect = tempCri.Object;
	}
}


// Called when the game starts or when spawned											
void ABossB::BeginPlay()
{
	Super::BeginPlay();

	Hp = InitalHp;

	mainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
	UGameplayStatics::PlaySound2D(GetWorld(), startSound);
}

// Called every frame											
void ABossB::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime += DeltaTime;
	if (Hp >= 0)
	{
		if (attackState == 0)
		{
			if (CurrentTime > 3)
			{
				attackState++;
				CurrentTime = 0;
			}
		}
		else if (attackState == 1)
		{
			Fire();
			if (CurrentTime > 3)
			{
				attackState++;
				CurrentTime = 0;
			}
		}
		else if (attackState == 2)
		{
			if (CurrentTime > 3)
			{
				attackState++;
				CurrentTime = 0;
			}
		}
		else if (attackState == 3)
		{
			Fire();
			if (CurrentTime > 3)
			{
				attackState++;
				CurrentTime = 0;
			}
		}
		else if (attackState == 4)
		{
			Fire();
			if (CurrentTime > 3)
			{
				attackState++;
				CurrentTime = 0;
			}
		}
		else if (attackState == 5)
		{
			if (CurrentTime > 3)
			{
				attackState++;
				CurrentTime = 0;
			}
		}
		else if (attackState == 6)
		{
			Fire();
			if (CurrentTime > 3)
			{
				CurrentTime = 0;
			}
		}
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PlayAnimMontage(montage, 1.0f, FName(TEXT("Die")));
		sound->Stop();
		TArray<AActor*> missiles;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossB_Missile::StaticClass(), missiles);
		for (int32 i = 0; i < missiles.Num(); i++)
		{
			GetWorld()->DestroyActor(missiles[i]);
		}
		dieTIme += DeltaTime;
		if (dieTIme > 1.5)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), effect, GetActorLocation(), GetActorRotation(), FVector3d(5));
		}

		if (dieTIme > 3)
		{
			StopAnimMontage(montage);

			Destroy();

			GetWorld()->SpawnActor<ABossD>(BossD, FVector(-40, -14890.0, 800), FRotator(0));
		}
	}
}

void ABossB::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->GetName().Contains(TEXT("critical")))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), exSound);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), criEffect, GetActorLocation() + FVector(-100, 0, 0), GetActorRotation(), FVector3d(10));
		Damaged();
		//Hp 를 30깎는다
		CriticalEvent();
		//OtherActor(Bullet)파괴
		OtherActor->Destroy();
	}
	if (OtherActor->GetName().Contains(TEXT("Bullet")))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), hitSound);
		Damaged();
		OnHitEvent();
		OtherActor->Destroy();
	}
}


void ABossB::MakeMissile(FVector pos, FRotator rot)
{
	GetWorld()->SpawnActor<ABossB_Missile>(MissileFactory, pos, rot);
}

void ABossB::Fire()
{
	float MakeTime = 3;
	if (CurrentTime > MakeTime)
	{
		AActor* player = UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass());
		mainPlayer = Cast<AMainPlayer>(player);
		sound = UGameplayStatics::SpawnSound2D(GetWorld(), attackSound);
		if (player != nullptr)
		{
			FVector PlayerPos = player->GetActorLocation();
			PlayerPos.Z += 1000;
			MakeMissile(PlayerPos, GetActorRotation());

			FVector MissilePosR;
			FVector MissilePosL;
			for (int32 i = 1; i < MissileCount; i++)
			{
				int32 randR = FMath::RandRange(250, 450);
				int32 randL = FMath::RandRange(-250, -450);
				int32 randZ = FMath::RandRange(800, 1000);

				MissilePosR = GetActorLocation();
				MissilePosL = GetActorLocation();
				MissilePosR.Y += randR * i;
				MissilePosR.Z += randZ;
				MissilePosL.Y += randL * i;
				MissilePosL.Z += randZ;

				MakeMissile(MissilePosR, GetActorRotation());
				MakeMissile(MissilePosL, GetActorRotation());
			}
		}
	}
}

void ABossB::OnHitEvent()
{
	Hp--;

	if (mainPlayer->Mp < 250)
	{
		mainPlayer->Mp++;
	}
}

void ABossB::CriticalEvent()
{
	Hp -= 30;
}

void ABossB::Damaged()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), collection, TEXT("emi"), 0.1);
	FTimerHandle WaitHandle;
	float WaitTime = 0.1; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), collection, TEXT("emi"), 0);
		}), WaitTime, false);
}