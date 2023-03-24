// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyZone2.generated.h"

UCLASS()
class PROJECT_JNB_API ADestroyZone2 : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestroyZone2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
		class UBoxComponent* compBox;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
