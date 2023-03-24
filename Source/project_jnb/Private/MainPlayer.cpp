// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Math/TransformCalculus2D.h>
#include <UObject/NoExportTypes.h>
#include <Components/MeshComponent.h>
#include "Bullet_gn.h"
#include <GameFramework/Actor.h>
#include <Engine/World.h>
#include <Kismet/KismetMathLibrary.h>
#include "Bullet3.h"
#include "criticalBullet.h"
#include <GameFramework/PlayerController.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>



 // Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//2. Mesh 컴포넌트의 위치와 회전 값을 설정하고 싶다.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerPreset"));

	//1. 스켈레탈메시를 가져오고 싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("SkeletalMesh'/Game/Blueprints/MPAnim/Ducking.Ducking'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> tempAnim(TEXT("AnimMontage'/Game/Blueprints/MPAnim/MT_MP.MT_MP'"));
	if (tempAnim.Succeeded())
	{
		Montage = tempAnim.Object;
	}
	/*ConstructorHelpers::FObjectFinder<UParticleSystem> tempExplo(TEXT(""));
	if (tempExplo.Succeeded())
	{
		exploEffect = tempExplo.Object;
	}*/

	//2단 점프
	//JumpMaxCount = 2;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
	//SetActorLocation(FVector(0)); 

	Hp = InitialHp;
	Mp = InitialMp;
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Hp > 0)
	{
		Movement();
	}
	
	if (Hp <= 0)
	{
		dyingEvent();
		dyingTime += DeltaTime;
		if (dyingTime > 2.4)
			{
			Destroy();
			}
	}

	//피격 시 콜리젼 설정 no collision
	if (bCrash)
	{	
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("crashPreset"));
		crashTime += DeltaTime;
		
		if(crashTime > 2.5)
		{	
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerPreset"));
			bCrash = false;
			crashTime = 0;
		}
	}

	if (use1B == true)
	{

		if (bFire == true)
		{
			currTime += DeltaTime;
			if (currTime > 0.2)
			{
				FRotator rot;
				//만약에 direction 의 길이가 0이면
				if (direction.Length() <= 0)
				{
					rot = GetActorRotation();
				}
				//그렇지 않으면 
				else
				{
					//만약에 direction.Z 가 0보다 크고 direction.Y 가 0보다 크면
					if (direction.Z != 0 && direction.Y != 0)
					{
						direction = direction.RotateAngleAxis(-25 * direction.Y, FVector::ForwardVector);
					}
					//direction 방향으로 나가게 한다
					rot = UKismetMathLibrary::MakeRotFromY(direction);
				}

				//FTransform skFire = GetMesh()->GetChildComponent(0)->GetComponentTransform();
				//GetWorld()->SpawnActor<ABullet_gn>(BulletFactory, skFire);

				//총알생성 (위치, 회전 셋팅)
 				if (GetActorRotation() == FRotator(0,0,0))
 				{
 				GetWorld()->SpawnActor<ABullet_gn>(BulletFactory, GetActorLocation() + FVector(0, 100, 0), rot);
 				}
 				else if (GetActorRotation() == FRotator(0, 180, 0))
 				{
 					GetWorld()->SpawnActor<ABullet_gn>(BulletFactory, GetActorLocation() + FVector(0, -100, 0), rot);
 				}
				currTime = 0;
			}
		}
	}

	if (use1B == false)
	{

		if (bFire == true)
		{
			currTime += DeltaTime;
			if (currTime > 0.2)
			{
				FRotator rot;
				//만약에 direction 의 길이가 0이면
				if (direction.Length() <= 0)
				{
					rot = GetActorRotation();
				}
				//그렇지 않으면 
				else
				{
					//만약에 direction.Z 가 0보다 크고 direction.Y 가 0보다 크면
					if (direction.Z != 0 && direction.Y != 0)
					{
						direction = direction.RotateAngleAxis(-25 * direction.Y, FVector::ForwardVector);
					}
					//direction 방향으로 나가게 한다
					rot = UKismetMathLibrary::MakeRotFromYX(direction, FVector::ForwardVector);
				}

				int32 i;
				for (i = 0; i < 3; i++)
				{
					FRotator rot3 = FRotator(0, 0, 30) * i - FRotator(0, 0, 30);
					
					if (GetActorRotation() == FRotator(0, 0, 0))
					{
						GetWorld()->SpawnActor<ABullet3>(BulletFactory2, GetActorLocation() + FVector(0, 100, 0), rot + rot3);
					}
					else if (GetActorRotation() == FRotator(0, 180, 0))
					{
						GetWorld()->SpawnActor<ABullet3>(BulletFactory2, GetActorLocation() + FVector(0, -100, 0), rot + rot3);
					}
					//GetWorld()->SpawnActor<ABullet3>(BulletFactory2, GetActorLocation(), rot + rot3);
				}
				currTime = 0;

			}
		}
	}

	
}

void AMainPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	//만약에 부딪힌 놈의 이름이 1.boss 2.missile 을 포함하고 있다면 
	//Hp 를 1깎는다 => crashEvent();

	if (OtherActor->GetName().Contains(TEXT("Missile")))
	{
		crashEvent(true);
		OtherActor->Destroy();
	}
	else if (OtherActor->GetName().Contains(TEXT("Boss")))
	{
		crashEvent(true);
	}
}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//좌우입력, 상하입력 이벤트처리함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AMainPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AMainPlayer::InputVertical);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainPlayer::InputJump);
	
	DECLARE_DELEGATE_OneParam(bDuck, bool)
	PlayerInputComponent->BindAction<bDuck>(TEXT("Duck"), IE_Pressed, this, &AMainPlayer::InputDuck, true); //down key
	PlayerInputComponent->BindAction<bDuck>(TEXT("Duck"), IE_Released, this, &AMainPlayer::InputDuck, false);

	DECLARE_DELEGATE_OneParam(FDash, bool)
	PlayerInputComponent->BindAction<FDash>(TEXT("Dash"), IE_Pressed, this, &AMainPlayer::InputDash, true);

	PlayerInputComponent->BindAction(TEXT("ChangeWeapon"), IE_Pressed, this, &AMainPlayer::changeWeapon);
	//inputFire
	DECLARE_DELEGATE_OneParam(FFire, bool) //연사
	PlayerInputComponent->BindAction<FFire>(TEXT("Fire"), IE_Pressed, this, &AMainPlayer::InputFire, true); //C key
	PlayerInputComponent->BindAction<FFire>(TEXT("Fire"), IE_Released, this, &AMainPlayer::InputFire, false);
	PlayerInputComponent->BindAction(TEXT("Fire2"), IE_Pressed, this, &AMainPlayer::InputFire2); //true 일 때 z 키 사용 가능(사용 후 mp = 0 으로 set. 다시 비활성화)
	PlayerInputComponent->BindAction(TEXT("ChangeHP"), IE_Pressed, this, &AMainPlayer::ChangeHP);
}

void AMainPlayer::InputHorizontal(float value)
{
	if (Hp > 0)
	{
		direction.Y = value;
		dir.Y = value;
		if (dir.Y == 1)
		{
			SetActorRotation(FRotator(0, 0, 0));
		}
		else if (dir.Y == -1)
		{
			SetActorRotation(FRotator(0, 180, 0));
		}
	}
}

void AMainPlayer::InputVertical(float value)
{
	direction.Z = value;
}

void AMainPlayer::InputJump()
{
	Jump();
}

void AMainPlayer::InputDuck(bool value)
{
	bSit = value;
}

void AMainPlayer::InputDash(bool value)
{	
	bDash = value;

	dashStart = GetActorLocation();
	dashEnd = GetActorLocation() + GetActorRightVector() * 400;

// 	dir.Y = value;
// 	//movement -> 일 때 //GetActorRightVector()?? Y 값에 +100,200 값을 준다
// 	if (dir.Y == 1)
// 	{
// 		GetActorLocation() + FVector(0, 100, 0);
// 	}
// 	//movement <- 일 때 -GetActorRightVector() //Y 값에 -100,200 값을 준다
// 	if (dir.Y == -1)
// 	{
// 		GetActorLocation() + FVector(0, -100, 0);
// 	}
}

void AMainPlayer::InputFire(bool value) //기본 발사 C key
{
	bFire = value;
}


void AMainPlayer::InputFire2() //필살기 Z key
{
	//mp≥100 이 되면 사용 가능
	if (Mp >= 50)
	{
		FRotator rot;
		//만약에 direction 의 길이가 0이면
		if (direction.Length() <= 0)
		{
			rot = GetActorRotation();
		}
		//그렇지 않으면 
		else
		{
			//만약에 direction.Z 가 0보다 크고 direction.Y 가 0보다 크면
			if (direction.Z != 0 && direction.Y != 0)
			{
				direction = direction.RotateAngleAxis(-25 * direction.Y, FVector::ForwardVector);
			}
			//direction 방향으로 나가게 한다
			rot = UKismetMathLibrary::MakeRotFromY(direction);
		}
		GetWorld()->SpawnActor<AcriticalBullet>(BulletFactory3, GetActorLocation(), rot);
		Mp -= 50;
	}
	 
}

//hp 설정 - 미사일, 보스와 충돌 상황 설정 함수
void AMainPlayer::crashEvent(bool value)
{
	bCrash = value;

	PlayAnimMontage(Montage, 2.0f, FName(TEXT("Damaged")));
	Hp--;
}

void AMainPlayer::dyingEvent()
{
	if (dieState == 0)
	{
		PlayAnimMontage(Montage, 1.0f, TEXT("Dying"));
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("crashPreset"));
	}
	dieState++;
}

void AMainPlayer::Movement()
{
	//플레이어 이동 처리
	//P(결과 위치) = P0(현재 위치) + vt(속도*시간)

	if (bDash == false)
	{
		GetCharacterMovement()->GravityScale = 2;
		FVector d = FTransform(GetControlRotation()).TransformVector(dir);
		d.Z = 0;
		FVector P0 = GetActorLocation();
		FVector vt = dir * walkSpeed * GetWorld()->GetDeltaSeconds();
		FVector P = P0 + vt;
		SetActorLocation(P);
	}
	else
	{
			/*FVector P = FMath::Lerp(GetActorLocation(), dashEnd, GetWorld()->GetDeltaSeconds() * 20);
			SetActorLocation(P);
			if (FVector::Distance(P, dashEnd) < 0.2f)
			{
				bDash = false;
			}*/

			dashTime += GetWorld()->GetDeltaSeconds() * 10;
			FVector P = FMath::Lerp(dashStart, dashEnd, dashTime);
			SetActorLocation(P, true);
			if (dashTime >= 1)
			{

				bDash = false;
				dashTime = 0;
			}
	}
}

void AMainPlayer::changeWeapon()
{
	if (use1B == true)
	{
		use1B = false;
	}
	else
	{
		use1B = true;
	}
}

void AMainPlayer::ChangeHP()
{
	Hp = 10000;
}