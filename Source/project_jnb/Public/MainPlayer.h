// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <UObject/NoExportTypes.h>
#include "MainPlayer.generated.h"

//��ǥ:������� �¿��Է��� �޾� �̵��ϰ� �ʹ�.
//�ʿ�Ӽ�: �̵��ӵ�, �̵�����
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
		int32 InitialHp = 3; //�����ϱ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 InitialMp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) //���� Mp
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


	//�̵��ӵ� 
	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 600;
	//�̵����� 
	FVector direction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector dir;

	//�Ѿ˰���
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet_gn> BulletFactory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet3> BulletFactory2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AcriticalBullet> BulletFactory3;

	//�Ѿ��� �¿� ����
	UPROPERTY(EditAnywhere)
	float bulletGap = 100;



	UFUNCTION()
	void InputHorizontal(float value);
	void InputVertical(float value);
	void InputJump();
	void InputDuck(bool value); //bool value
	void InputDash(bool value);
	void InputFire(bool value); //fire1
	void InputFire2(); //fire2 �ʻ��
	UFUNCTION()
	void crashEvent(bool value); //hp ���� - �̻���, ������ �浹 ��Ȳ ���� �Լ�
	void dyingEvent();
	void Movement(); //�÷��̾� �̵�����
	void changeWeapon();
	void ChangeHP();

};


