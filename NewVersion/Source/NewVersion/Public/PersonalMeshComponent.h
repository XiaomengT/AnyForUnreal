// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "Interfaces/Interface_CollisionDataProvider.h"
#include "PersonalMeshComponent.generated.h"

USTRUCT(BlueprintType)
struct FPersonalMeshVertex
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Triangle)
		FVector Position;

	UPROPERTY(EditAnywhere, Category = Triangle)
		FColor Color;

	UPROPERTY(EditAnywhere, Category = Triangle)
		float U;

	UPROPERTY(EditAnywhere, Category = Triangle)
		float V;
};

USTRUCT(BlueprintType)
struct FPersonalMeshTriangle
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = Triangle)
		FPersonalMeshVertex Vertex0;

	UPROPERTY(EditAnywhere, Category = Triangle)
		FPersonalMeshVertex Vertex1;

	UPROPERTY(EditAnywhere, Category = Triangle)
		FPersonalMeshVertex Vertex2;
};



UCLASS(EditInlineNew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class NEWVERSION_API UPersonalMeshComponent : public UMeshComponent, public IInterface_CollisionDataProvider
{
	GENERATED_BODY()
	
public:
	UPersonalMeshComponent();

	UFUNCTION(BlueprintCallable, Category = "Components|PersonalMesh")
		bool SetPersonalMeshTriangles(const TArray<FPersonalMeshTriangle>& Triangles);

	/** Add to the geometry to use on this triangle mesh.  This may cause an allocation.  Use SetCustomMeshTriangles() instead when possible to reduce allocations. */
	UFUNCTION(BlueprintCallable, Category = "Components|PersonalMesh")
		void AddPersonalMeshTriangles(const TArray<FPersonalMeshTriangle>& Triangles);

	/** Removes all geometry from this triangle mesh.  Does not deallocate memory, allowing new geometry to reuse the existing allocation. */
	UFUNCTION(BlueprintCallable, Category = "Components|PersonalMesh")
		void ClearPersonalMeshTriangles();

	/** Description of collision */
	UPROPERTY(BlueprintReadOnly, Category = "Collision")
		class UBodySetup* ModelBodySetup;

	// Begin Interface_CollisionDataProvider Interface
	virtual bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData) override;
	virtual bool ContainsPhysicsTriMeshData(bool InUseAllTriData) const override;
	virtual bool WantsNegXTriMesh() override { return false; }
	// End Interface_CollisionDataProvider Interface

	// Begin UPrimitiveComponent interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual class UBodySetup* GetBodySetup() override;
	// End UPrimitiveComponent interface.

	// Begin UMeshComponent interface.
	virtual int32 GetNumMaterials() const override;
	// End UMeshComponent interface.

	void UpdateBodySetup();
	void UpdateCollision();

private:
	// Begin USceneComponent interface.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	// Begin USceneComponent interface.

	/** */
	TArray<FPersonalMeshTriangle> PersonalMeshTris;

	friend class FPersonalMeshSceneProxy;
};
