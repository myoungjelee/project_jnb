// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossC_Missile.generated.h"

UCLASS()
class PROJECT_JNB_API ABossC_Missile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossC_Missile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* compCapsule;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* compMesh;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void OnHitEvent();

	UPROPERTY(EditAnywhere)
		int32 InitalHp = 2;
	UPROPERTY(EditAnywhere)
		int32 Hp;

	FVector dir;

	FRotator rot;

	UPROPERTY()
		class AMainPlayer* mainPlayer;
	UPROPERTY(EditAnywhere)
		class USoundBase* destroySound;
};
