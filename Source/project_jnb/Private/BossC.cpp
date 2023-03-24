// Fill out your copyright notice in the Description page of Project Settings.


#include "BossC.h"
#include <Components/BoxComponent.h>
#include "BossC_Missile.h"
#include "BossC_Missile2.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SkeletalMeshComponent.h>
#include "MainPlayer.h"
#include <Components/StaticMeshComponent.h>
#include <Animation/AnimInstance.h>
#include <Components/AudioComponent.h>
#include <Blueprint/UserWidget.h>
#include <Materials/MaterialParameterCollection.h>
#include <Kismet/KismetMaterialLibrary.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Particles/ParticleSystem.h>



// Sets default values
ABossC::ABossC()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BossPreset"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(100);
	GetCapsuleComponent()->SetCapsuleRadius(40);
	GetCapsuleComponent()->SetRelativeScale3D(FVector(4));

	GetMesh()->SetRelativeLocation(FVector(0, 0, -95));
	GetMesh()->SetWorldRotation(FRotator(0, 90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.2));

	GetCharacterMovement()->GravityScale = 0;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Boss/BossC/BossC/source/Boss_C.Boss_C'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	ConstructorHelpers::FClassFinder<ABossC_Missile> tempMis(TEXT("Blueprint'/Game/Blueprints/BP_BossC_Missile.BP_BossC_Missile_C'"));
	if (tempMis.Succeeded())
	{
		MissileFactory = tempMis.Class;
	}

	ConstructorHelpers::FClassFinder<ABossC_Missile2> tempMis2(TEXT("Blueprint'/Game/Blueprints/BP_BossC_Missile2.BP_BossC_Missile2_C'"));
	if (tempMis2.Succeeded())
	{
		MissileFactory2 = tempMis2.Class;
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("AnimBlueprint'/Game/ABP/ABP_BossC.ABP_BossC_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempStart(TEXT("SoundWave'/Game/Audio/BossC_Start.BossC_Start'"));
	if (tempStart.Succeeded())
	{
		startSound = tempStart.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound1(TEXT("SoundWave'/Game/Audio/BossC_Attack1.BossC_Attack1'"));
	if (tempSound1.Succeeded())
	{
		attack1Sound = tempSound1.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound2(TEXT("SoundWave'/Game/Audio/BossC_Attack2.BossC_Attack2'"));
	if (tempSound2.Succeeded())
	{
		attack2Sound = tempSound2.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound3(TEXT("SoundWave'/Game/Audio/Win.Win'"));
	if (tempSound3.Succeeded())
	{
		endSound = tempSound3.Object;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> tempWid(TEXT("WidgetBlueprint'/Game/Blueprints/BP_Win.BP_Win_C'"));
	if (tempWid.Succeeded())
	{
		winWidget = tempWid.Class;
	}

	ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> tempColl(TEXT("MaterialParameterCollection'/Game/Assets/Boss/Boss_Emi.Boss_Emi'"));
	if (tempColl.Succeeded())
	{
		collection = tempColl.Object;
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
void ABossC::BeginPlay()
{
	Super::BeginPlay();

	Hp = InitalHp;

	mainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));

	UGameplayStatics::PlaySound2D(GetWorld(), startSound);
}

// Called every frame
void ABossC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Hp >= 0)
	{
		if (attackState == 0)
		{
			CurrentTime += DeltaTime;
			if (CurrentTime > 3)
			{
				attackState++;
				CurrentTime = 0;
			}
		}
		else if (attackState == 1)
		{
			FirFire(DeltaTime);
		}
		else if (attackState == 2)
		{
			SecFire(DeltaTime);
		}
		else if (attackState == 3)
		{
			attackState = 1;
		}
	}
	else
	{
		audio1->Stop();
		audio2->Stop();
		TArray<AActor*> missiles;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossC_Missile::StaticClass(), missiles);
		for (int32 i = 0; i < missiles.Num(); i++)
		{
			GetWorld()->DestroyActor(missiles[i]);
		}
		TArray<AActor*> missiles2;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossC_Missile2::StaticClass(), missiles2);
		for (int32 i = 0; i < missiles2.Num(); i++)
		{
			GetWorld()->DestroyActor(missiles2[i]);
		}
		Destroy();
		mainPlayer->bFire = false;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UGameplayStatics::PlaySound2D(GetWorld(), endSound);
		UUserWidget* win = CreateWidget(GetWorld(), winWidget);
		win->AddToViewport();
	}
}


void ABossC::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (OtherActor->GetName().Contains(TEXT("critical")))
	{

		UGameplayStatics::PlaySound2D(GetWorld(), exSound);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), criEffect, GetActorLocation() + FVector(-100, 0, 0), GetActorRotation(), FVector3d(6));
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





void ABossC::MakeMissile(FVector pos, FRotator rot)
{
	GetWorld()->SpawnActor<ABossC_Missile>(MissileFactory, pos, rot);
}

void ABossC::MakeMissile2(FVector pos, FRotator rot)
{
	GetWorld()->SpawnActor<ABossC_Missile2>(MissileFactory2, pos, rot);
}


void ABossC::FirFire(float DeltaTime)
{
	if (sound == 0)
	{
		audio1 = UGameplayStatics::SpawnSound2D(GetWorld(), attack1Sound);
	}
	sound++;
	if (MissileCount < 2)
	{
		float MakeTime = 2.5;
		CurrentTime += DeltaTime;

		if (CurrentTime > MakeTime)
		{
			int32 randR = FMath::RandRange(0, 500);
			int32 randL = FMath::RandRange(-500, 0);
			FVector MissilePosR;
			FVector MissilePosL;

			MissilePosR = GetActorLocation();
			MissilePosR.Y += randR;
			MissilePosR.Z += 150;

			MissilePosL = GetActorLocation();
			MissilePosL.Y += randL;
			MissilePosL.Z += 150;

			MakeMissile(MissilePosR, GetActorRotation());
			MakeMissile(MissilePosL, GetActorRotation());

			MissileCount++;
			CurrentTime = 0;
		}
	}
	else
	{
		attackState++;
		MissileCount = 0;
		sound = 0;
	}

}

void ABossC::SecFire(float DeltaTime)
{
	if (MissileCount < 3)
	{
		float MakeTime = 2.5;
		CurrentTime += DeltaTime;

		if (CurrentTime > MakeTime)
		{
			if (sound == 0)
			{
				audio2 = UGameplayStatics::SpawnSound2D(GetWorld(), attack2Sound);
			}
			sound++;
			MakeMissile2(GetActorLocation(), GetActorRotation());

			MissileCount++;
			CurrentTime = 0;
			sound = 0;
		}
	}
	else
	{
		attackState++;
		MissileCount = 0;
	}
}

void ABossC::OnHitEvent()
{
	Hp--;

	if (mainPlayer->Mp < 250)
	{
		mainPlayer->Mp++;
	}
}

void ABossC::CriticalEvent()
{
	Hp -= 30;
}

void ABossC::Damaged()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), collection, TEXT("emi"), 0.1);
	FTimerHandle WaitHandle;
	float WaitTime = 0.1; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), collection, TEXT("emi"), 0);
		}), WaitTime, false);
}