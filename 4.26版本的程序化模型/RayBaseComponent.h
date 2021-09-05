// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "UObject/ObjectMacros.h"
#include "RayBaseComponent.generated.h"

class FPrimitiveSceneProxy;

USTRUCT(BlueprintType)
struct FRayLineHitPointDesc {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=RayLineHitPoint)
	FVector HitPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RayLineHitPoint)
	FVector HitNextDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RayLineHitPoint)
	int32 HitPointIndex;
};

/**
 * 
 */
UCLASS(hideCategories = (Object, LOD, Physics, Collision), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class RAYLINE_API URayBaseComponent : public UMeshComponent
{
	GENERATED_UCLASS_BODY()
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = RayLineComp)
	float DebugSec;

	private:
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	virtual int32 GetNumMaterials() const override;

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	virtual void OnRegister() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void SendRenderDynamicData_Concurrent() override;
	virtual void CreateRenderState_Concurrent(FRegisterComponentContext* Context) override;

	TArray<FRayLineHitPointDesc> RayLineHitPoints;

	friend class FRayLineMeshSceneProxy;
	
};
