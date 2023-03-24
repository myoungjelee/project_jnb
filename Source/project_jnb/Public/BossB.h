// Fill out your copyright notice in the Description page of Project Settings.			

#pragma once			

#include "CoreMinimal.h"			
#include "GameFramework/Character.h"			
#include "BossB.generated.h"			

UCLASS()
class PROJECT_JNB_API ABossB : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties		
	ABossB();

protected:
	// Called when the game starts or when spawned		
	virtual void BeginPlay() override;

public:
	// Called every frame		
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* compBox;
	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* compSkel;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABossB_Missile> MissileFactory;
	UPROPERTY(EditAnywhere)
		float MissileCount = 4;
	UPROPERTY(EditAnywhere)
		float MissileGap = 200;
	UPROPERTY()
		class AMainPlayer* mainPlayer;
	UPROPERTY(EditAnywhere)
		int32 InitalHp = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Hp;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABossD> BossD;
	UPROPERTY(EditAnywhere)
		class USoundBase* attackSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* startSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* hitSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* exSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 attackState = 0;
	UPROPERTY(EditAnywhere)
		class UMaterialParameterCollection* collection;
	UPROPERTY(EditAnywhere)
		class UAudioComponent* sound;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* effect;
	UPROPERTY(EditAnywhere)
		class UAnimMontage* montage;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* criEffect;


public:

	float CurrentTime = 0;
	float dieTIme = 0;

public:

	void OnHitEvent();
	void MakeMissile(FVector pos, FRotator rot);
	void Fire();
	void CriticalEvent();
	void Damaged();
};
