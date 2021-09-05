// Fill out your copyright notice in the Description page of Project Settings.

#include "RayBasicComponent.h"
#include "RenderingThread.h"
#include "RenderResource.h"
#include "PrimitiveViewRelevance.h"
#include "PrimitiveSceneProxy.h"
#include "VertexFactory.h"
#include "MaterialShared.h"
#include "Engine/CollisionProfile.h"
#include "Materials/Material.h"
#include "LocalVertexFactory.h"
#include "SceneManagement.h"
#include "DynamicMeshBuilder.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"



/** Vertex Buffer */
class FRayLineMeshVertexBuffer : public FVertexBuffer
{
public:

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		VertexBufferRHI = RHICreateVertexBuffer(NumVerts * sizeof(FDynamicMeshVertex), BUF_Dynamic, CreateInfo);
	}

	int32 NumVerts;
};

/** Index Buffer */
class FRayLineMeshIndexBuffer : public FIndexBuffer
{
public:

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		IndexBufferRHI = RHICreateIndexBuffer(sizeof(int32), NumIndices * sizeof(int32), BUF_Dynamic, CreateInfo);
	}

	int32 NumIndices;
};

/** Vertex Factory */
class FCustomMeshVertexFactory : public FLocalVertexFactory
{
public:

	FCustomMeshVertexFactory()
	{}


	/** Initialization */
	void Init(const FRayLineMeshVertexBuffer* VertexBuffer)
	{
		if (IsInRenderingThread())
		{
			// Initialize the vertex factory's stream components.
			FDataType NewData;
			NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
			NewData.TextureCoordinates.Add(
				FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), VET_Float2)
			);
			NewData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
			NewData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
			SetData(NewData);
		}
		else
		{
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
				InitCableVertexFactory,
				FCustomMeshVertexFactory*, VertexFactory, this,
				const FRayLineMeshVertexBuffer*, VertexBuffer, VertexBuffer,
				{
					// Initialize the vertex factory's stream components.
					FDataType NewData;
			NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer,FDynamicMeshVertex,Position,VET_Float3);
			NewData.TextureCoordinates.Add(
				FVertexStreamComponent(VertexBuffer,STRUCT_OFFSET(FDynamicMeshVertex,TextureCoordinate),sizeof(FDynamicMeshVertex),VET_Float2)
			);
			NewData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer,FDynamicMeshVertex,TangentX,VET_PackedNormal);
			NewData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer,FDynamicMeshVertex,TangentZ,VET_PackedNormal);
			VertexFactory->SetData(NewData);
				});
		}
	}
};

struct FRayLineDynamicData
{
	TArray<FVector> HitpointsPosition;
	//You can also define some other data to send
};

/** Scene proxy */
class FRayLineMeshSceneProxy : public FPrimitiveSceneProxy
{
public:

	FRayLineMeshSceneProxy(URayBasicComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
	{

		VertexBuffer.NumVerts = GetRequiredVertexCount();
		IndexBuffer.NumIndices = GetRequiredIndexCount();

		const FColor VertexColor(255, 255, 255);

		// Init vertex factory
		VertexFactory.Init(&VertexBuffer);

		// Enqueue initialization of render resource
		BeginInitResource(&VertexBuffer);
		BeginInitResource(&IndexBuffer);
		BeginInitResource(&VertexFactory);

		// Grab material
		Material = Component->GetMaterial(0);
		if (Material == NULL)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}

	}

	void BuildMesh(FRayLineDynamicData* NewDynamicData)
	{

		TArray<FDynamicMeshVertex> Vertices;
		TArray<int32> Indices;
		for (int32 i = 0; i < NewDynamicData->HitpointsPosition.Num(); i++)
		{
			FDynamicMeshVertex newvert0;
			newvert0.Position = NewDynamicData->HitpointsPosition[i] + FVector(-100, 100, 0);
			FDynamicMeshVertex newvert1;
			newvert1.Position = NewDynamicData->HitpointsPosition[i] + FVector(100, 100, 0);
			FDynamicMeshVertex newvert2;
			newvert2.Position = NewDynamicData->HitpointsPosition[i] + FVector(-100, -100, 0);
			FDynamicMeshVertex newvert3;
			newvert3.Position = NewDynamicData->HitpointsPosition[i] + FVector(100, -100, 0);

			Vertices.Add(newvert0);
			Vertices.Add(newvert1);
			Vertices.Add(newvert2);
			Vertices.Add(newvert3);

			Indices.Add(4 * i);
			Indices.Add(4 * i + 1);
			Indices.Add(4 * i + 2);
			Indices.Add(4 * i + 1);
			Indices.Add(4 * i + 3);
			Indices.Add(4 * i + 2);
		}

		check(Vertices.Num() == GetRequiredVertexCount());
		check(Indices.Num() == GetRequiredIndexCount());

		void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, Vertices.Num() * sizeof(FDynamicMeshVertex), RLM_WriteOnly);
		FMemory::Memcpy(VertexBufferData, &Vertices[0], Vertices.Num() * sizeof(FDynamicMeshVertex));
		RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);

		void* IndexBufferData = RHILockIndexBuffer(IndexBuffer.IndexBufferRHI, 0, Indices.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcpy(IndexBufferData, &Indices[0], Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBuffer.IndexBufferRHI);
	}

	/** Called on render thread to assign new dynamic data */
	void SetDynamicData_RenderThread(FRayLineDynamicData* NewDynamicData)
	{
		check(IsInRenderingThread());

		BuildMesh(NewDynamicData);

	}

	virtual ~FRayLineMeshSceneProxy()
	{
		VertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	int32 GetRequiredVertexCount() const
	{
		return 40;
	}

	int32 GetRequiredIndexCount() const
	{
		return 60;
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_FRayLineMeshSceneProxy_GetDynamicMeshElements);

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy(IsSelected()) : NULL,
			FLinearColor(0, 0.5f, 1.f)
		);

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

		FMaterialRenderProxy* MaterialProxy = NULL;
		if (bWireframe)
		{
			MaterialProxy = WireframeMaterialInstance;
		}
		else
		{
			MaterialProxy = Material->GetRenderProxy(IsSelected());
		}

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];
				// Draw the mesh.
				FMeshBatch& Mesh = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = Mesh.Elements[0];
				BatchElement.IndexBuffer = &IndexBuffer;
				Mesh.bWireframe = bWireframe;
				Mesh.VertexFactory = &VertexFactory;
				Mesh.MaterialRenderProxy = MaterialProxy;
				BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());
				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = GetRequiredIndexCount() / 3;
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = GetRequiredVertexCount();
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_TriangleList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, Mesh);
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

private:

	UMaterialInterface * Material;
	FRayLineMeshVertexBuffer VertexBuffer;
	FRayLineMeshIndexBuffer IndexBuffer;
	FCustomMeshVertexFactory VertexFactory;

	FMaterialRelevance MaterialRelevance;
};

//////////////////////////////////////////////////////////////////////////

URayBasicComponent::URayBasicComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;

	DebugSec = 200.0f;

}

void URayBasicComponent::OnRegister()
{
	Super::OnRegister();

	RayLineHitPoints.Reset();
	FVector RayDirection = FVector(1.0f, 0.0f, 0.0f);
	FVector RayOrigin = FVector(0.0f, 0.0f, 0.0f);
	int32 HitPointsNum = 10;
	float SecLength = 50.0f;

	RayLineHitPoints.AddUninitialized(HitPointsNum);
	RayLineHitPoints[0].HitPosition = RayOrigin;
	RayLineHitPoints[0].HitNextDir = RayDirection;

	float t = DebugSec;
	for (int32 i = 1; i < HitPointsNum; i++)
	{
		RayLineHitPoints[i].HitPosition = RayDirection * t + RayOrigin;

		t += DebugSec;
	}

	MarkRenderDynamicDataDirty();
}

void URayBasicComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RayLineHitPoints.Reset();
	FVector RayDirection = FVector(1.0f, 0.0f, 0.0f);
	FVector RayOrigin = FVector(0.0f, 0.0f, 0.0f);
	int32 HitPointsNum = 10;
	float SecLength = 50.0f;

	RayLineHitPoints.AddUninitialized(HitPointsNum);
	RayLineHitPoints[0].HitPosition = RayOrigin;
	RayLineHitPoints[0].HitNextDir = RayDirection;

	float t = DebugSec;
	for (int32 i = 1; i < HitPointsNum; i++)
	{
		RayLineHitPoints[i].HitPosition = RayDirection * t + RayOrigin;

		t += DebugSec;
	}

	// Need to send new data to render thread
	MarkRenderDynamicDataDirty();

	UpdateComponentToWorld();
}

void URayBasicComponent::CreateRenderState_Concurrent()
{
	Super::CreateRenderState_Concurrent();

	SendRenderDynamicData_Concurrent();
}

void URayBasicComponent::SendRenderDynamicData_Concurrent()
{
	if (SceneProxy)
	{
		FRayLineDynamicData* NewDynamicData = new FRayLineDynamicData;
		NewDynamicData->HitpointsPosition.AddUninitialized(RayLineHitPoints.Num());
		for (int32 i = 0; i < RayLineHitPoints.Num(); i++)
		{
			NewDynamicData->HitpointsPosition[i] = RayLineHitPoints[i].HitPosition;
		}

		// Enqueue command to send to render thread
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
			FSendRayBasicComponentDynamicData,
			FRayLineMeshSceneProxy*, CableSceneProxy, (FRayLineMeshSceneProxy*)SceneProxy,
			FRayLineDynamicData*, NewDynamicData, NewDynamicData,
			{
				CableSceneProxy->SetDynamicData_RenderThread(NewDynamicData);
			});
	}
}

FPrimitiveSceneProxy* URayBasicComponent::CreateSceneProxy()
{
	return new FRayLineMeshSceneProxy(this);
}

int32 URayBasicComponent::GetNumMaterials() const
{
	return 1;
}

FBoxSphereBounds URayBasicComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds NewBounds;
	NewBounds.Origin = FVector::ZeroVector;
	NewBounds.BoxExtent = FVector(HALF_WORLD_MAX, HALF_WORLD_MAX, HALF_WORLD_MAX);
	NewBounds.SphereRadius = FMath::Sqrt(3.0f * FMath::Square(HALF_WORLD_MAX));
	return NewBounds;
}