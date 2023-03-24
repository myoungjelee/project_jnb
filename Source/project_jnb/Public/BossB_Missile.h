// Fill out your copyright notice in the Description page of Project Settings.		

#pragma once		

#include "CoreMinimal.h"		
#include "GameFramework/Actor.h"		
#include "BossB_Missile.generated.h"		

UCLASS()
class PROJECT_JNB_API ABossB_Missile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties	
	ABossB_Missile();

protected:
	// Called when the game starts or when spawned	
	virtual void BeginPlay() override;

public:
	// Called every frame	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* compMesh;

	UPROPERTY(EditAnywhere)
		float speed = 500;

	FVector dir;



};
