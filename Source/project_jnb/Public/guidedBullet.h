// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "guidedBullet.generated.h"

UCLASS()
class PROJECT_JNB_API AguidedBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AguidedBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//충돌 컴포넌트
	UPROPERTY(EditAnywhere)
	class USphereComponent* gSphereComp;
	//외관 컴포넌트
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* gMeshComp;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
