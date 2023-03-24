// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossC.generated.h"

UCLASS()
class PROJECT_JNB_API ABossC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABossC_Missile> MissileFactory;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ABossC_Missile2> MissileFactory2;
	UPROPERTY(EditAnywhere)
		int32 InitalHp = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMainPlayer* mainPlayer;
	UPROPERTY(EditAnywhere)
		class USoundBase* startSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* attack1Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* attack2Sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* endSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* hitSound;
	UPROPERTY(EditAnywhere)
		class USoundBase* exSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 attackState = 0;
	UPROPERTY(EditAnywhere)
		class UMaterialParameterCollection* collection;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> winWidget;
	UPROPERTY(EditAnywhere)
		class UAudioComponent* audio1;
	UPROPERTY(EditAnywhere)
		class UAudioComponent* audio2;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* criEffect;


public:

	int32 sound = 0;
	float CurrentTime = 0;
	int32 MissileCount = 0;

public:

	void OnHitEvent();
	void MakeMissile(FVector pos, FRotator rot);
	void MakeMissile2(FVector pos, FRotator rot);
	void FirFire(float DeltaTime);
	void SecFire(float DeltaTime);
	void CriticalEvent();
	void Damaged();
};
