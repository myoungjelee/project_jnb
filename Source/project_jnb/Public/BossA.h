// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossA.generated.h"

UCLASS()
class PROJECT_JNB_API ABossA : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this Actor's properties
	ABossA();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABossA_Missile> MissileFactory;
	UPROPERTY(EditAnywhere)
		int32 InitalHp = 125;
	UPROPERTY(EditAnywhere)
		int32 Hp;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* Montage;
	UPROPERTY(EditAnywhere)
		class USoundBase* attackSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* hitSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* exSound;
	UPROPERTY(EditAnywhere)
		class AMainPlayer* mainPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 attackState = 0;
	UPROPERTY(EditAnywhere)
		class UMaterialParameterCollection* emi;
	UPROPERTY(EditAnywhere)
		class UAudioComponent* sound;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABossB> BossB;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* criEffect;



public:
	float dieTime = 0;
	int32 dieState = 0;
	int32 idleState = 0;
	float CurrentTime = 0;
	int32 MissileCount = 0;
	float damTIme = 0;
	float visible;
public:

	void OnHitEvent();
	UFUNCTION(BlueprintCallable)
		void FirFire(float DeltaTime);
	void SecFire(float DeltaTime);
	void ThiFire(float DeltaTime);
	void MakeMissile(FVector pos, FRotator rot);
	void CriticalEvent();
	void Damaged();
	void Die();
};