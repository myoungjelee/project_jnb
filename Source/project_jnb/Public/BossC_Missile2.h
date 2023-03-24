// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossC_Missile2.generated.h"

UCLASS()
class PROJECT_JNB_API ABossC_Missile2 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossC_Missile2();

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

	UPROPERTY(EditAnywhere)
		float speed = 600;

	FVector dir;

	FRotator rot;

	UPROPERTY()
		class AMainPlayer* mainPlayer;


};
