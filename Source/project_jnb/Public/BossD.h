// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossD.generated.h"

UCLASS()
class PROJECT_JNB_API ABossD : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:

	UPROPERTY(EditAnywhere)
		int32 InitalHp = 100;
	UPROPERTY(EditAnywhere)
		int32 Hp;
	UPROPERTY()
		class AMainPlayer* mainPlayer;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABossC> BossC;
	UPROPERTY(EditAnywhere)
		class USoundBase* startSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* attack1Sound;
	UPROPERTY(EditAnywhere)
		class USoundBase* attack2Sound;
	UPROPERTY(EditAnywhere)
		class USoundBase* dieSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* hitSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* exSound;
	UPROPERTY(EditAnywhere)
		class UMaterialParameterCollection* collection;
	UPROPERTY()
		class UAudioComponent* sound;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* criEffect;

public:

	float CurrentTime = 0;
	float CurrTIme = 0;
	float AttackTime = 3;
	float HitTime = 2;
	float dieTime = 0;
	float AttackCount = 0;
	int32 sounState = 0;
	int32 sounState2 = 0;
	int32 State = 0;
	FVector dir;
	FRotator rot;

public:

	void MoveL();
	void MoveR();
	void Attack();
	void OnHitEvent();
	void CriticalEvent();
	void Damaged();
};
