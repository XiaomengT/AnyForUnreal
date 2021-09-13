// Fill out your copyright notice in the Description page of Project Settings.

#include "MeshTestActor.h"


// Sets default values
AMeshTestActor::AMeshTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<URayBasicComponent>(TEXT("Rayline"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AMeshTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeshTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

