// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet_gn.generated.h"

UCLASS()
class PROJECT_JNB_API ABullet_gn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet_gn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	//�浹 ������Ʈ p.69
	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;
	//�ܰ� ������Ʈ
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY()
	class AMainPlayer* Direction;



public:
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	void crashEvent();

	//�߻�ü�� �̵��� ����� ������Ʈ
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* movementComp;
	
	FVector dir;

};
