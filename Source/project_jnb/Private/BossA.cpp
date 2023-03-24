
// Fill out your copyright notice in the Description page of Project Settings.
#include "BossA.h"
#include "BossA_Missile.h"
#include "BossB.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SkeletalMeshComponent.h>
#include "MainPlayer.h"
#include <Components/StaticMeshComponent.h>
#include <Animation/AnimInstance.h>
#include <Sound/SoundBase.h>
#include <Materials/MaterialParameterCollection.h>
#include <Kismet/KismetMaterialLibrary.h>
#include <Components/CapsuleComponent.h>
#include <Components/AudioComponent.h>
#include <Particles/ParticleSystem.h>



// Sets default values
ABossA::ABossA()
{
	// Set this Actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	GetCapsuleComponent()->SetRelativeScale3D(FVector(4));
	GetCapsuleComponent()->SetCapsuleHalfHeight(100);
	GetCapsuleComponent()->SetCapsuleRadius(40);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -95));
	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	GetMesh()->SetRelativeScale3D(FVector(2.5));


	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Boss/BossA/BossA/BossA.BossA'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	ConstructorHelpers::FClassFinder<ABossA_Missile> tempMis(TEXT("Blueprint'/Game/Blueprints/BP_BossA_Missile.BP_BossA_Missile_C'"));
	if (tempMis.Succeeded())
	{
		MissileFactory = tempMis.Class;
	}

	ConstructorHelpers::FClassFinder<ABossB> tempBoss(TEXT("Blueprint'/Game/Blueprints/BP_BossB.BP_BossB_C'"));
	if (tempBoss.Succeeded())
	{
		BossB = tempBoss.Class;
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("AnimBlueprint'/Game/ABP/ABP_BossA.ABP_BossA_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempMont(TEXT("AnimMontage'/Game/ABP/AM_BossA.AM_BossA'"));
	if (tempMont.Succeeded())
	{
		Montage = tempMont.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound(TEXT("SoundWave'/Game/Audio/BossA_Attack.BossA_Attack'"));
	if (tempSound.Succeeded())
	{
		attackSound = tempSound.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> tempColl(TEXT("MaterialParameterCollection'/Game/Assets/Boss/Boss_Emi.Boss_Emi'"));
	if (tempColl.Succeeded())
	{
		emi = tempColl.Object;
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
void ABossA::BeginPlay()
{
	Super::BeginPlay();

	Hp = InitalHp;

	mainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));
}

// Called every frame
void ABossA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Hp 값이 0이 되거나 작아지면
	if (Hp > 0)
	{
		if (attackState == 0)
		{

			CurrentTime += DeltaTime;
			if (CurrentTime > 4)
			{
				GetCapsuleComponent()->SetCollisionProfileName(TEXT("BossPreset"));
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
			if (idleState == 0)
			{
				PlayAnimMontage(Montage, 1.5f, TEXT("Idle"));
			}
			idleState++;
			CurrentTime += DeltaTime;
			if (CurrentTime > 3)
			{
				attackState++;
				MissileCount = 0;
				CurrentTime = 0;
				idleState = 0;
			}
		}
		else if (attackState == 3)
		{
			SecFire(DeltaTime);
		}
		else if (attackState == 4)
		{
			if (idleState == 0)
			{
				PlayAnimMontage(Montage, 1.5f, TEXT("Idle"));
			}
			idleState++;
			CurrentTime += DeltaTime;
			if (CurrentTime > 3)
			{
				attackState++;
				MissileCount = 0;
				CurrentTime = 0;
				idleState = 0;
			}
		}
		else if (attackState == 5)
		{
			ThiFire(DeltaTime);
		}
	}
	else
	{
		Die();
		dieTime += DeltaTime;
		if (dieTime > 3)
		{
			Destroy();
			GetWorld()->SpawnActor<ABossB>(BossB, FVector(-40, -15470, 510), FRotator(0));

		}
	}
}


void ABossA::NotifyActorBeginOverlap(AActor* OtherActor)
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
	//만약에 부딪힌 놈의 이름이 Bullet 을 포함하고 있다면 
	if (OtherActor->GetName().Contains(TEXT("Bullet")))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), hitSound);
		Damaged();
		//Hp 를 1깎는다
		OnHitEvent();
		//OtherActor(Bullet)파괴
		OtherActor->Destroy();
	}

}

void ABossA::MakeMissile(FVector pos, FRotator rot)
{
	sound = UGameplayStatics::SpawnSound2D(GetWorld(), attackSound);
	GetWorld()->SpawnActor<ABossA_Missile>(MissileFactory, pos, rot);
}

void ABossA::FirFire(float DeltaTime)
{
	if (MissileCount < 4)
	{
		//미사일 생성시간 설정
		float MakeTime = 1.6;
		//시간을 흐르게한다.
		CurrentTime += DeltaTime;
		//흐르는 시간이 미사일 생성시간보다 커지면
		if (CurrentTime > MakeTime)
		{
			//미사일 발사 위치를 설정해줄 벡터값 설정
			FVector MissilePos;
			//미사일 발사 위치를 BossA 위치로 설정
			MissilePos = GetActorLocation();
			//미사일 발사 위치를 z 축으로 100만큼 내린다
			MissilePos.Z = 180;
			//설정한 위치에서 총알을 발사한다.
			MakeMissile(MissilePos, GetActorRotation());
			//발사 카운트를 증가 시킨다.
			MissileCount++;
			//흐르는 시간을 초기화
			CurrentTime = 0;
		}
	}
	else
	{
		attackState++;
	}
}
void ABossA::SecFire(float DeltaTime)
{
	if (MissileCount < 4)
	{
		//미사일 생성시간 설정
		float MakeTime = 1.3;
		//시간을 흐르게한다.
		CurrentTime += DeltaTime;
		//흐르는 시간이 미사일 생성시간보다 커지면
		if (CurrentTime > MakeTime)
		{
			//미사일 발사 위치를 설정해줄 벡터값 설정
			FVector MissilePos;
			//미사일 발사 위치를 BossA 위치로 설정
			MissilePos = GetActorLocation();
			//미사일 발사 위치를 z 축으로 100만큼 내린다
			MissilePos.Z = 180;
			//설정한 위치에서 총알을 발사한다.
			MakeMissile(MissilePos, GetActorRotation());
			//발사 카운트를 증가 시킨다.
			MissileCount++;
			//흐르는 시간을 초기화
			CurrentTime = 0;
		}
	}
	else
	{
		attackState++;
	}
}
void ABossA::ThiFire(float DeltaTime)
{
	if (MissileCount < 4)
	{
		//미사일 생성시간 설정
		float MakeTime = 1.1;
		//시간을 흐르게한다.
		CurrentTime += DeltaTime;
		//흐르는 시간이 미사일 생성시간보다 커지면
		if (CurrentTime > MakeTime)
		{
			//미사일 발사 위치를 설정해줄 벡터값 설정
			FVector MissilePos;
			//미사일 발사 위치를 BossA 위치로 설정
			MissilePos = GetActorLocation();
			//미사일 발사 위치를 z 축으로 100만큼 내린다
			MissilePos.Z = 180;
			//설정한 위치에서 총알을 발사한다.
			MakeMissile(MissilePos, GetActorRotation());
			//발사 카운트를 증가 시킨다.
			MissileCount++;
			//흐르는 시간을 초기화
			CurrentTime = 0;
		}
	}
	else
	{
		CurrentTime += DeltaTime;
		if (CurrentTime > 2)
		{
			attackState = 1;
			MissileCount = 0;
			CurrentTime = 0;
		}
	}
}


void ABossA::OnHitEvent()
{
	Hp--;

	if (mainPlayer->Mp < 250)
	{
		mainPlayer->Mp++;
		UE_LOG(LogTemp, Error, TEXT("%d"), mainPlayer->Mp);
	}
}

void ABossA::CriticalEvent()
{
	Hp -= 30;
}

void ABossA::Damaged()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), emi, TEXT("emi"), 0.1);
	FTimerHandle WaitHandle;
	float WaitTime = 0.1; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), emi, TEXT("emi"), 0);
		}), WaitTime, false);
}

void ABossA::Die()
{
	if (dieState == 0)
	{

		PlayAnimMontage(Montage, 1.0f, TEXT("Dying"));
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	dieState++;

	TArray<AActor*> missiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossA_Missile::StaticClass(), missiles);
	for (int32 i = 0; i < missiles.Num(); i++)
	{
		GetWorld()->DestroyActor(missiles[i]);
	}
}
