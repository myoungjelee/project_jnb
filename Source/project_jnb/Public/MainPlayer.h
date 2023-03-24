// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <UObject/NoExportTypes.h>
#include "MainPlayer.generated.h"

//목표:사용자의 좌우입력을 받아 이동하고 싶다.
//필요속성: 이동속도, 이동방향
UCLASS()
class PROJECT_JNB_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	UPROPERTY(EditAnywhere)
		class UMeshComponent* fireMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 InitialHp = 3; //수정하기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 InitialMp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) //현재 Mp
		int32 Mp;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFire2 = false;
	bool use1B = true;
	float currTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCrash = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSit = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDash = false;
	FVector dashStart;
	FVector dashEnd;
	float dashTime = 0;
	bool bDashTest = false;

	int32 dieState = 0;
	float crashTime = 0;
	float dyingTime = 0;


	//이동속도 
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;
	//이동방향 
	FVector direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector dir;

	//총알공장
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet_gn> BulletFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet3> BulletFactory2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AcriticalBullet> BulletFactory3;

	//총알의 좌우 간격
	UPROPERTY(EditAnywhere)
	float bulletGap = 100;



	UFUNCTION()
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void InputDuck(bool value); //bool value
	void InputDash(bool value);
	void InputFire(bool value); //fire1
	void InputFire2(); //fire2 필살기
	UFUNCTION()
	void crashEvent(bool value); //hp 설정 - 미사일, 보스와 충돌 상황 설정 함수
	void dyingEvent();
	void Movement(); //플레이어 이동설정
	void changeWeapon();
	void ChangeHP();

};


