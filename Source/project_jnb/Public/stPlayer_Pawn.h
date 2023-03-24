// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "stPlayer_Pawn.generated.h"

UCLASS()
class PROJECT_JNB_API AstPlayer_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AstPlayer_Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* compBox;

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* compSkel;

	void spHorizontal(float value);
	void spVertical(float value);

	UPROPERTY(BlueprintReadWrite)
	float h;
	UPROPERTY(BlueprintReadWrite)
	float v;
	float speed;
};
