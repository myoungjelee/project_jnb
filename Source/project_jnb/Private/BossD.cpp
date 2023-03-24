// Fill out your copyright notice in the Description page of Project Settings.


#include "BossD.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include "MainPlayer.h"
#include <Kismet/KismetMathLibrary.h>
#include "BossC.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Animation/AnimInstance.h>
#include <Sound/SoundBase.h>
#include <Materials/MaterialParameterCollection.h>
#include <Kismet/KismetMaterialLibrary.h>
#include <Components/CapsuleComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/AudioComponent.h>
#include <Particles/ParticleSystem.h>



// Sets default values
ABossD::ABossD()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BossPreset"));
	GetCapsuleComponent()->SetCapsuleHalfHeight(110);
	GetCapsuleComponent()->SetCapsuleRadius(110);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -150));
	GetMesh()->SetRelativeRotation(FRotator(0, 135, 0));
	GetMesh()->SetRelativeScale3D(FVector(3));

	GetCharacterMovement()->GravityScale = 0;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("SkeletalMesh'/Game/Assets/Boss/BossD/source/Beedrill_Animation.Beedrill_Animation'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
	}

	ConstructorHelpers::FClassFinder<ABossC> tempBoss(TEXT("Blueprint'/Game/Blueprints/BP_BossC.BP_BossC_C'"));
	if (tempBoss.Succeeded())
	{
		BossC = tempBoss.Class;
	}

	ConstructorHelpers::FClassFinder<UAnimInstance> tempAnim(TEXT("AnimBlueprint'/Game/ABP/ABP_BossD.ABP_BossD_C'"));
	if (tempAnim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(tempAnim.Class);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempStart(TEXT("SoundWave'/Game/Audio/BossD_Start.BossD_Start'"));
	if (tempStart.Succeeded())
	{
		startSound = tempStart.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound1(TEXT("SoundWave'/Game/Audio/BossD-Attack1.BossD-Attack1'"));
	if (tempSound1.Succeeded())
	{
		attack1Sound = tempSound1.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound2(TEXT("SoundWave'/Game/Audio/BossD_Attack2.BossD_Attack2'"));
	if (tempSound2.Succeeded())
	{
		attack2Sound = tempSound2.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempSound3(TEXT("SoundWave'/Game/Audio/BossD_Die.BossD_Die'"));
	if (tempSound3.Succeeded())
	{
		dieSound = tempSound3.Object;
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
void ABossD::BeginPlay()
{
	Super::BeginPlay();

	Hp = InitalHp;

	mainPlayer = Cast<AMainPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainPlayer::StaticClass()));

	UGameplayStatics::PlaySound2D(GetWorld(), startSound);
}

// Called every frame
void ABossD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	CurrTIme += DeltaTime;

	if (Hp >= 0)
	{
		if (State == 0)
		{
			if (CurrTIme > 3)
			{
				State++;
				CurrTIme = 0;
			}
		}
		else if (State == 1)
		{
			MoveL();
			if (GetActorLocation().Y <= -16300)
			{
				State++;
				CurrTIme = 0;
			}
		}
		else if (State == 2)
		{
			MoveR();
			if (GetActorLocation().Y >= -14700)
			{
				State++;
				CurrTIme = 0;
			}
		}
		else if (State == 3)
		{
			if (AttackCount < 1)
			{
				Attack();
			}
			else
			{
				State++;
				CurrTIme = 0;
				AttackCount = 0;
			}

		}
		else if (State == 4)
		{
			MoveL();
			if (GetActorLocation().Y <= -16300)
			{
				State++;
				CurrTIme = 0;
			}
		}
		else if (State == 5)
		{
			if (AttackCount < 2)
			{
				Attack();
			}

			else
			{
				State++;
				CurrTIme = 0;
				AttackCount = 0;
			}
		}
		else if (State == 6)
		{
			MoveL();
			if (GetActorLocation().Y <= -16300)
			{
				State++;
				CurrTIme = 0;
			}
		}
		else if (State == 7)
		{
			GetMesh()->SetRelativeRotation(FRotator(0, 45, 0));

			if (GetActorLocation().Z > 220)
			{
				FVector p0 = GetActorLocation();
				FVector vt = -GetActorUpVector() * 2000 * GetWorld()->GetDeltaSeconds();
				SetActorLocation(p0 + vt);
			}
			else if (GetActorLocation().Z <= 220 && GetActorLocation().Y <= -14700)
			{
				if (sounState == 0)
				{
					sound = UGameplayStatics::SpawnSound2D(GetWorld(), attack2Sound);
				}
				sounState++;
				FVector p0 = GetActorLocation();
				FVector vt = GetActorRightVector() * 1000 * GetWorld()->GetDeltaSeconds();
				SetActorLocation(p0 + vt);
			}
			else
			{
				State++;
				CurrTIme = 0;
				sounState = 0;
			}
		}
		else if (State == 8)
		{
			if (GetActorLocation().Z <= 800 && GetActorLocation().Y >= -14700)
			{
				FVector p0 = GetActorLocation();
				FVector vt = GetActorUpVector() * 2000 * GetWorld()->GetDeltaSeconds();
				SetActorLocation(p0 + vt);
			}
			else
			{
				State++;
				CurrTIme = 0;
			}
		}
		else if (State == 9)
		{
			if (AttackCount < 2)
			{
				Attack();
			}

			else
			{
				State++;
				CurrTIme = 0;
				AttackCount = 0;
			}
		}
		else if (State == 10)
		{
			MoveR();
			if (GetActorLocation().Y >= -14700)
			{
				State++;
				CurrTIme = 0;
			}
		}
		else if (State == 11)
		{
			GetMesh()->SetRelativeRotation(FRotator(0, 135, 0));

			if (GetActorLocation().Z > 220)
			{
				FVector p0 = GetActorLocation();
				FVector vt = -GetActorUpVector() * 2000 * GetWorld()->GetDeltaSeconds();
				SetActorLocation(p0 + vt);
			}
			else if (GetActorLocation().Z <= 220 && GetActorLocation().Y >= -16300)
			{
				if (sounState == 0)
				{
					sound = UGameplayStatics::SpawnSound2D(GetWorld(), attack2Sound);
				}
				sounState++;
				FVector p0 = GetActorLocation();
				FVector vt = -GetActorRightVector() * 1000 * GetWorld()->GetDeltaSeconds();
				SetActorLocation(p0 + vt);
			}
			else
			{
				State++;
				CurrTIme = 0;
				sounState = 0;
			}
		}
		else if (State == 12)
		{
			if (GetActorLocation().Z <= 800 && GetActorLocation().Y <= -16300)
			{
				FVector p0 = GetActorLocation();
				FVector vt = GetActorUpVector() * 2000 * GetWorld()->GetDeltaSeconds();
				SetActorLocation(p0 + vt);
			}
			else
			{
				State = 5;
				CurrTIme = 0;
			}
		}
	}
	else
	{
		sound->Stop();

		if (sounState2 == 0)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), dieSound);
		}
		sounState2++;

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FVector p0 = GetActorLocation();
		FVector vt = (GetActorUpVector() + GetActorRightVector()) * 2000 * GetWorld()->GetDeltaSeconds();
		SetActorLocation(p0 + vt);

		dieTime += DeltaTime;
		if (dieTime > 3)
		{
			Destroy();

			GetWorld()->SpawnActor<ABossC>(BossC, FVector(-40, -15470, 900), FRotator(0));
		}
	}
}

void ABossD::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->GetName().Contains(TEXT("critical")))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), exSound);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), criEffect, GetActorLocation() + FVector(-100, 0, 0), GetActorRotation(), FVector3d(5));
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

void ABossD::MoveL()
{
	GetMesh()->SetRelativeRotation(FRotator(0, 135, 0));

	if (GetActorLocation().Y >= -16500)
	{
		FVector p0 = GetActorLocation();
		FVector vt = -GetActorRightVector() * 2000 * GetWorld()->GetDeltaSeconds();
		SetActorLocation(p0 + vt);
	}
}
void ABossD::MoveR()
{
	GetMesh()->SetRelativeRotation(FRotator(0, 45, 0));

	if (GetActorLocation().Y <= -14500)
	{
		FVector p0 = GetActorLocation();
		FVector vt = GetActorRightVector() * 2000 * GetWorld()->GetDeltaSeconds();
		SetActorLocation(p0 + vt);
	}
}
void ABossD::Attack()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();


	if (mainPlayer != nullptr)
	{
		if (mainPlayer->GetActorLocation().Y > GetActorLocation().Y)
		{
			GetMesh()->SetRelativeRotation(FRotator(0, 45, 0));
		}
		else
		{
			GetMesh()->SetRelativeRotation(FRotator(0, 135, 0));
		}
		if (CurrentTime < 2)
		{
			if (CurrentTime > 1)
			{
				if (sounState == 0)
				{
					UGameplayStatics::PlaySound2D(GetWorld(), attack1Sound);
				}
				sounState++;
				dir = mainPlayer->GetActorLocation() - GetActorLocation();
				dir.Normalize();

				FVector p0 = GetActorLocation();
				FVector vt = dir * 1000 * GetWorld()->GetDeltaSeconds();
				SetActorLocation(p0 + vt);
			}
		}
		else if (CurrentTime < 3)
		{
			if (GetActorLocation().Z < 800)
			{
				FVector p0 = GetActorLocation();
				FVector vt = GetActorUpVector() * 2000 * GetWorld()->GetDeltaSeconds();
				SetActorLocation(p0 + vt);
			}
		}
		else
		{
			CurrentTime = 0;
			AttackCount++;
			sounState = 0;
		}
	}
	else
	{
		if (GetActorLocation().Z <= 800)
		{
			FVector p0 = GetActorLocation();
			FVector vt = GetActorUpVector() * 500 * GetWorld()->GetDeltaSeconds();
			SetActorLocation(p0 + vt);
		}
	}

}

void ABossD::OnHitEvent()
{
	Hp--;

	if (mainPlayer->Mp < 250)
	{
		mainPlayer->Mp++;
	}
}

void ABossD::CriticalEvent()
{
	Hp -= 30;
}

void ABossD::Damaged()
{
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), collection, TEXT("emi"), 0.1);
	FTimerHandle WaitHandle;
	float WaitTime = 0.1; //시간을 설정하고
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
		{
			UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), collection, TEXT("emi"), 0);
		}), WaitTime, false);
}