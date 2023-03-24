// Fill out your copyright notice in the Description page of Project Settings.


#include "stPlayer_Pawn.h"
#include <Components/BoxComponent.h>
#include <Components/SkeletalMeshComponent.h>

// Sets default values
AstPlayer_Pawn::AstPlayer_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	compBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(compBox);

	compBox->SetBoxExtent(FVector(30));
	compBox->SetCollisionProfileName(TEXT("PlayerPreset")); //프리셋 이름 넣어주기
	

	compSkel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal"));
	compSkel->SetupAttachment(compBox);
	compSkel->SetRelativeScale3D(FVector(0.6));
	compSkel->SetRelativeRotation(FRotator(0, 90, -90));
	compSkel->SetRelativeLocation(FVector(-40, 0, -10));
	compSkel->SetCollisionEnabled(ECollisionEnabled::NoCollision); //콜리젼 맞게 넣어주기

	ConstructorHelpers::FObjectFinder<USkeletalMesh> spMesh(TEXT("SkeletalMesh'/Game/Blueprints/MPAnim/Shooting.Shooting'"));
	if (spMesh.Succeeded())
	{
		compSkel->SetSkeletalMesh(spMesh.Object);
	}
}

// Called when the game starts or when spawned
void AstPlayer_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(FVector(-80, 60, 40));
	SetActorRotation(FRotator(0,-180,0));
}

// Called every frame
void AstPlayer_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	speed = 500;
	
	//p = p0 +vt
	FVector p0 = GetActorLocation();
	FVector vt = speed * DeltaTime * (GetActorRightVector()*h + GetActorForwardVector()*v);
	FVector p = p0 + vt;
	SetActorLocation(p, true);
}

// Called to bind functionality to input
void AstPlayer_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//AD key
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &AstPlayer_Pawn::spHorizontal);
	//WS key
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &AstPlayer_Pawn::spVertical);
}

void AstPlayer_Pawn::spHorizontal(float value)
{
	h = value;
}

void AstPlayer_Pawn::spVertical(float value)
{
	v = value;
}


