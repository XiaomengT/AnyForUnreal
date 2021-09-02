// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PersonalMeshComponent.h"
#include "ProceduralCubeActor.generated.h"

UCLASS()
class NEWVERSION_API AProceduralCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralCubeActor();

	UPROPERTY(VisibleAnywhere, Category = Materials)
		UPersonalMeshComponent* mesh;

	void GenerateCube(const float& InSize, TArray<FPersonalMeshTriangle>& OutTriangles);
};
