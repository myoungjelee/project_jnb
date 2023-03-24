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


	//2. Mesh ������Ʈ�� ��ġ�� ȸ�� ���� �����ϰ� �ʹ�.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, 0));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerPreset"));

	//1. ���̷�Ż�޽ø� �������� �ʹ�.
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

	//2�� ����
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

	//�ǰ� �� �ݸ��� ���� no collision
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
				//���࿡ direction �� ���̰� 0�̸�
				if (direction.Length() <= 0)
				{
					rot = GetActorRotation();
				}
				//�׷��� ������ 
				else
				{
					//���࿡ direction.Z �� 0���� ũ�� direction.Y �� 0���� ũ��
					if (direction.Z != 0 && direction.Y != 0)
					{
						direction = direction.RotateAngleAxis(-25 * direction.Y, FVector::ForwardVector);
					}
					//direction �������� ������ �Ѵ�
					rot = UKismetMathLibrary::MakeRotFromY(direction);
				}

				//FTransform skFire = GetMesh()->GetChildComponent(0)->GetComponentTransform();
				//GetWorld()->SpawnActor<ABullet_gn>(BulletFactory, skFire);

				//�Ѿ˻��� (��ġ, ȸ�� ����)
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
				//���࿡ direction �� ���̰� 0�̸�
				if (direction.Length() <= 0)
				{
					rot = GetActorRotation();
				}
				//�׷��� ������ 
				else
				{
					//���࿡ direction.Z �� 0���� ũ�� direction.Y �� 0���� ũ��
					if (direction.Z != 0 && direction.Y != 0)
					{
						direction = direction.RotateAngleAxis(-25 * direction.Y, FVector::ForwardVector);
					}
					//direction �������� ������ �Ѵ�
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

	//���࿡ �ε��� ���� �̸��� 1.boss 2.missile �� �����ϰ� �ִٸ� 
	//Hp �� 1��´� => crashEvent();

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

	//�¿��Է�, �����Է� �̺�Ʈó���Լ� ���ε�
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
	DECLARE_DELEGATE_OneParam(FFire, bool) //����
	PlayerInputComponent->BindAction<FFire>(TEXT("Fire"), IE_Pressed, this, &AMainPlayer::InputFire, true); //C key
	PlayerInputComponent->BindAction<FFire>(TEXT("Fire"), IE_Released, this, &AMainPlayer::InputFire, false);
	PlayerInputComponent->BindAction(TEXT("Fire2"), IE_Pressed, this, &AMainPlayer::InputFire2); //true �� �� z Ű ��� ����(��� �� mp = 0 ���� set. �ٽ� ��Ȱ��ȭ)
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
// 	//movement -> �� �� //GetActorRightVector()?? Y ���� +100,200 ���� �ش�
// 	if (dir.Y == 1)
// 	{
// 		GetActorLocation() + FVector(0, 100, 0);
// 	}
// 	//movement <- �� �� -GetActorRightVector() //Y ���� -100,200 ���� �ش�
// 	if (dir.Y == -1)
// 	{
// 		GetActorLocation() + FVector(0, -100, 0);
// 	}
}

void AMainPlayer::InputFire(bool value) //�⺻ �߻� C key
{
	bFire = value;
}


void AMainPlayer::InputFire2() //�ʻ�� Z key
{
	//mp��100 �� �Ǹ� ��� ����
	if (Mp >= 50)
	{
		FRotator rot;
		//���࿡ direction �� ���̰� 0�̸�
		if (direction.Length() <= 0)
		{
			rot = GetActorRotation();
		}
		//�׷��� ������ 
		else
		{
			//���࿡ direction.Z �� 0���� ũ�� direction.Y �� 0���� ũ��
			if (direction.Z != 0 && direction.Y != 0)
			{
				direction = direction.RotateAngleAxis(-25 * direction.Y, FVector::ForwardVector);
			}
			//direction �������� ������ �Ѵ�
			rot = UKismetMathLibrary::MakeRotFromY(direction);
		}
		GetWorld()->SpawnActor<AcriticalBullet>(BulletFactory3, GetActorLocation(), rot);
		Mp -= 50;
	}
	 
}

//hp ���� - �̻���, ������ �浹 ��Ȳ ���� �Լ�
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
	//�÷��̾� �̵� ó��
	//P(��� ��ġ) = P0(���� ��ġ) + vt(�ӵ�*�ð�)

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