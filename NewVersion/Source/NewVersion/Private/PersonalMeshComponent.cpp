// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine.h"
#include "EngineGlobals.h"
#include "RHI.h"
#include "DynamicMeshBuilder.h"
#include "PhysicsEngine/BodySetup.h"
#include "RenderingThread.h"
#include "RenderResource.h"
#include "VertexFactory.h"
#include "LocalVertexFactory.h"
#include "PrimitiveViewRelevance.h"
#include "PrimitiveSceneProxy.h"
#include "MaterialShared.h"
#include "Materials/Material.h"
#include "Engine/CollisionProfile.h"
#include "SceneManagement.h"
#include "UObject/UObjectHash.h"
#include "UObject/UObjectIterator.h"
#include "StaticMeshResources.h"
#include "PersonalMeshComponent.h"


/** Vertex Buffer */
class FPersonalMeshVertexBuffer : public FVertexBuffer
{
public:
	TArray<FDynamicMeshVertex> Vertices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		VertexBufferRHI = RHICreateVertexBuffer(Vertices.Num() * sizeof(FDynamicMeshVertex), BUF_Static, CreateInfo);
		// Copy the vertex data into the vertex buffer.
		void* VertexBufferData = RHILockVertexBuffer(VertexBufferRHI, 0, Vertices.Num() * sizeof(FDynamicMeshVertex), RLM_WriteOnly);
		FMemory::Memcpy(VertexBufferData, Vertices.GetData(), Vertices.Num() * sizeof(FDynamicMeshVertex));
		RHIUnlockVertexBuffer(VertexBufferRHI);
	}
};

/** Index Buffer */
class FPersonalMeshIndexBuffer : public FIndexBuffer
{
public:
	TArray<int32> Indices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		IndexBufferRHI = RHICreateIndexBuffer(sizeof(int32), Indices.Num() * sizeof(int32), BUF_Static, CreateInfo);
		// Write the indices to the index buffer.
		void* Buffer = RHILockIndexBuffer(IndexBufferRHI, 0, Indices.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBufferRHI);
	}
};

/** Vertex Factory */
class FPersonalMeshVertexFactory : public FLocalVertexFactory
{
public:
	FPersonalMeshVertexFactory(ERHIFeatureLevel::Type InFeatureLevel) : FLocalVertexFactory(InFeatureLevel, "FPersonalMeshVertexFactory")
	{

	}

	/** Initialization */
	void Init(const FPersonalMeshVertexBuffer* vertexBuf)
	{
		// Commented out to enable building light of a level (but no backing is done for the Personal mesh itself)
		//check(!IsInRenderingThread());
		FPersonalMeshVertexFactory* VertexFactory = this;
		const FPersonalMeshVertexBuffer* VertexBuffer = vertexBuf;
		ENQUEUE_RENDER_COMMAND(InitPersonalMeshVertexFactory)(
			[VertexFactory, VertexBuffer](FRHICommandListImmediate& RHICmdList)
			{
				FLocalVertexFactory::FDataType NewData;
				NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
				NewData.TextureCoordinates.Add(
					FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), VET_Float2)
				);
				NewData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
				NewData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
				NewData.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Color, VET_Color);
				VertexFactory->SetData(NewData);
			}
		);

	}
};

/** Scene proxy */
class FPersonalMeshSceneProxy : public FPrimitiveSceneProxy
{
public:

	FPersonalMeshSceneProxy(UPersonalMeshComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
		, VertexFactory(GetScene().GetFeatureLevel())
	{
		// Add each triangle to the vertex/index buffer
		for (int TriIdx = 0; TriIdx < Component->PersonalMeshTris.Num(); TriIdx++)
		{
			FPersonalMeshTriangle& Tri = Component->PersonalMeshTris[TriIdx];

			const FVector Edge01 = (Tri.Vertex1.Position - Tri.Vertex0.Position);
			const FVector Edge02 = (Tri.Vertex2.Position - Tri.Vertex0.Position);

			const FVector TangentX = Edge01.GetSafeNormal();
			const FVector TangentZ = (Edge02 ^ Edge01).GetSafeNormal();
			const FVector TangentY = (TangentX ^ TangentZ).GetSafeNormal();

			FDynamicMeshVertex Vert0;
			Vert0.Position = Tri.Vertex0.Position;
			Vert0.Color = Tri.Vertex0.Color;
			Vert0.SetTangents(TangentX, TangentY, TangentZ);
			for (int k = 0; k < MAX_STATIC_TEXCOORDS; k++)
			{
				Vert0.TextureCoordinate[k] = FVector2D(Tri.Vertex0.U, Tri.Vertex0.V);
			}
			int32 VIndex = VertexBuffer.Vertices.Add(Vert0);
			IndexBuffer.Indices.Add(VIndex);

			FDynamicMeshVertex Vert1;
			Vert1.Position = Tri.Vertex1.Position;
			Vert1.Color = Tri.Vertex1.Color;
			Vert1.SetTangents(TangentX, TangentY, TangentZ);
			for (int l = 0; l < MAX_STATIC_TEXCOORDS; l++)
			{
				Vert1.TextureCoordinate[l] = FVector2D(Tri.Vertex1.U, Tri.Vertex1.V);
			}
			VIndex = VertexBuffer.Vertices.Add(Vert1);
			IndexBuffer.Indices.Add(VIndex);

			FDynamicMeshVertex Vert2;
			Vert2.Position = Tri.Vertex2.Position;
			Vert2.Color = Tri.Vertex2.Color;
			Vert2.SetTangents(TangentX, TangentY, TangentZ);
			for (int j = 0; j < MAX_STATIC_TEXCOORDS; j++)
			{
				Vert2.TextureCoordinate[j] = FVector2D(Tri.Vertex2.U, Tri.Vertex2.V);
			}
			VIndex = VertexBuffer.Vertices.Add(Vert2);
			IndexBuffer.Indices.Add(VIndex);
		}

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


	SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	virtual ~FPersonalMeshSceneProxy()
	{
		VertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_PersonalMeshSceneProxy_GetDynamicMeshElements);

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : NULL,
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
			MaterialProxy = Material->GetRenderProxy();
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
				//
				FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
				DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, false, DrawsVelocity(), false);
				BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;
				//
				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 3;
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_TriangleList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, Mesh);
			}
		}
	}

	virtual void DrawDynamicElements(FPrimitiveDrawInterface* PDI, const FSceneView* View)
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_PersonalMeshSceneProxy_DrawDynamicElements);

		const bool bWireframe = AllowDebugViewmodes() && View->Family->EngineShowFlags.Wireframe;

		FColoredMaterialRenderProxy WireframeMaterialInstance(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : NULL,
			FLinearColor(0, 0.5f, 1.f)
		);

		FMaterialRenderProxy* MaterialProxy = NULL;
		if (bWireframe)
		{
			MaterialProxy = &WireframeMaterialInstance;
		}
		else
		{
			MaterialProxy = Material->GetRenderProxy();
		}

		// Draw the mesh.
		FMeshBatch Mesh;
		FMeshBatchElement& BatchElement = Mesh.Elements[0];
		BatchElement.IndexBuffer = &IndexBuffer;
		Mesh.bWireframe = bWireframe;
		Mesh.VertexFactory = &VertexFactory;
		Mesh.MaterialRenderProxy = MaterialProxy;
		//
		FBoxSphereBounds preSkiLocBounds; 
		GetPreSkinnedLocalBounds(preSkiLocBounds);
		BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), preSkiLocBounds, true, DrawsVelocity());
		BatchElement.FirstIndex = 0;
		BatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 3;
		BatchElement.MinVertexIndex = 0;
		BatchElement.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;
		Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
		Mesh.Type = PT_TriangleList;
		Mesh.DepthPriorityGroup = SDPG_World;
		PDI->DrawMesh(Mesh);
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View)
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const
	{
		return(sizeof(*this) + GetAllocatedSize());
	}

	uint32 GetAllocatedSize(void) const
	{
		return(FPrimitiveSceneProxy::GetAllocatedSize());
	}

private:

	UMaterialInterface* Material;
	FPersonalMeshVertexBuffer VertexBuffer;
	FPersonalMeshIndexBuffer IndexBuffer;
	FPersonalMeshVertexFactory VertexFactory;
	FMaterialRelevance MaterialRelevance;
};


//////////////////////////////////////////////////////////////////////////////////////////

UPersonalMeshComponent::UPersonalMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
}

bool UPersonalMeshComponent::SetPersonalMeshTriangles(const TArray<FPersonalMeshTriangle>& Triangles)
{
	PersonalMeshTris = Triangles;

	UpdateCollision();

	// Need to recreate scene proxy to send it over
	MarkRenderStateDirty();

	return true;
}

void UPersonalMeshComponent::AddPersonalMeshTriangles(const TArray<FPersonalMeshTriangle>& Triangles)
{
	PersonalMeshTris.Append(Triangles);

	// Need to recreate scene proxy to send it over
	MarkRenderStateDirty();
}

void  UPersonalMeshComponent::ClearPersonalMeshTriangles()
{
	PersonalMeshTris.Reset();

	// Need to recreate scene proxy to send it over
	MarkRenderStateDirty();
}


FPrimitiveSceneProxy* UPersonalMeshComponent::CreateSceneProxy()
{
	FPrimitiveSceneProxy* Proxy = NULL;
	// Only if have enough triangles
	if (PersonalMeshTris.Num() > 0)
	{
		Proxy = new FPersonalMeshSceneProxy(this);
	}
	return Proxy;
}

int32 UPersonalMeshComponent::GetNumMaterials() const
{
	return 1;
}


FBoxSphereBounds UPersonalMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	// Only if have enough triangles
	if (PersonalMeshTris.Num() > 0)
	{
		// Minimum Vector: It's set to the first vertex's position initially (NULL == FVector::ZeroVector might be required and a known vertex vector has intrinsically valid values)
		FVector vecMin = PersonalMeshTris[0].Vertex0.Position;

		// Maximum Vector: It's set to the first vertex's position initially (NULL == FVector::ZeroVector might be required and a known vertex vector has intrinsically valid values)
		FVector vecMax = PersonalMeshTris[0].Vertex0.Position;

		// Get maximum and minimum X, Y and Z positions of vectors
		for (int32 TriIdx = 0; TriIdx < PersonalMeshTris.Num(); TriIdx++)
		{
			vecMin.X = (vecMin.X > PersonalMeshTris[TriIdx].Vertex0.Position.X) ? PersonalMeshTris[TriIdx].Vertex0.Position.X : vecMin.X;
			vecMin.X = (vecMin.X > PersonalMeshTris[TriIdx].Vertex1.Position.X) ? PersonalMeshTris[TriIdx].Vertex1.Position.X : vecMin.X;
			vecMin.X = (vecMin.X > PersonalMeshTris[TriIdx].Vertex2.Position.X) ? PersonalMeshTris[TriIdx].Vertex2.Position.X : vecMin.X;

			vecMin.Y = (vecMin.Y > PersonalMeshTris[TriIdx].Vertex0.Position.Y) ? PersonalMeshTris[TriIdx].Vertex0.Position.Y : vecMin.Y;
			vecMin.Y = (vecMin.Y > PersonalMeshTris[TriIdx].Vertex1.Position.Y) ? PersonalMeshTris[TriIdx].Vertex1.Position.Y : vecMin.Y;
			vecMin.Y = (vecMin.Y > PersonalMeshTris[TriIdx].Vertex2.Position.Y) ? PersonalMeshTris[TriIdx].Vertex2.Position.Y : vecMin.Y;

			vecMin.Z = (vecMin.Z > PersonalMeshTris[TriIdx].Vertex0.Position.Z) ? PersonalMeshTris[TriIdx].Vertex0.Position.Z : vecMin.Z;
			vecMin.Z = (vecMin.Z > PersonalMeshTris[TriIdx].Vertex1.Position.Z) ? PersonalMeshTris[TriIdx].Vertex1.Position.Z : vecMin.Z;
			vecMin.Z = (vecMin.Z > PersonalMeshTris[TriIdx].Vertex2.Position.Z) ? PersonalMeshTris[TriIdx].Vertex2.Position.Z : vecMin.Z;

			vecMax.X = (vecMax.X < PersonalMeshTris[TriIdx].Vertex0.Position.X) ? PersonalMeshTris[TriIdx].Vertex0.Position.X : vecMax.X;
			vecMax.X = (vecMax.X < PersonalMeshTris[TriIdx].Vertex1.Position.X) ? PersonalMeshTris[TriIdx].Vertex1.Position.X : vecMax.X;
			vecMax.X = (vecMax.X < PersonalMeshTris[TriIdx].Vertex2.Position.X) ? PersonalMeshTris[TriIdx].Vertex2.Position.X : vecMax.X;

			vecMax.Y = (vecMax.Y < PersonalMeshTris[TriIdx].Vertex0.Position.Y) ? PersonalMeshTris[TriIdx].Vertex0.Position.Y : vecMax.Y;
			vecMax.Y = (vecMax.Y < PersonalMeshTris[TriIdx].Vertex1.Position.Y) ? PersonalMeshTris[TriIdx].Vertex1.Position.Y : vecMax.Y;
			vecMax.Y = (vecMax.Y < PersonalMeshTris[TriIdx].Vertex2.Position.Y) ? PersonalMeshTris[TriIdx].Vertex2.Position.Y : vecMax.Y;

			vecMax.Z = (vecMax.Z < PersonalMeshTris[TriIdx].Vertex0.Position.Z) ? PersonalMeshTris[TriIdx].Vertex0.Position.Z : vecMax.Z;
			vecMax.Z = (vecMax.Z < PersonalMeshTris[TriIdx].Vertex1.Position.Z) ? PersonalMeshTris[TriIdx].Vertex1.Position.Z : vecMax.Z;
			vecMax.Z = (vecMax.Z < PersonalMeshTris[TriIdx].Vertex2.Position.Z) ? PersonalMeshTris[TriIdx].Vertex2.Position.Z : vecMax.Z;
		}

		FVector vecOrigin = ((vecMax - vecMin) / 2) + vecMin;	/* Origin = ((Max Vertex's Vector - Min Vertex's Vector) / 2 ) + Min Vertex's Vector */
		FVector BoxPoint = vecMax - vecMin;			/* The difference between the "Maximum Vertex" and the "Minimum Vertex" is our actual Bounds Box */
		return FBoxSphereBounds(vecOrigin, BoxPoint, BoxPoint.Size()).TransformBy(LocalToWorld);
	}
	else
	{
		return FBoxSphereBounds();
	}
}


bool UPersonalMeshComponent::GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool InUseAllTriData)
{
	FTriIndices Triangle;

	for (int32 i = 0; i < PersonalMeshTris.Num(); i++)
	{
		const FPersonalMeshTriangle& tri = PersonalMeshTris[i];

		Triangle.v0 = CollisionData->Vertices.Add(tri.Vertex0.Position);
		Triangle.v1 = CollisionData->Vertices.Add(tri.Vertex1.Position);
		Triangle.v2 = CollisionData->Vertices.Add(tri.Vertex2.Position);

		CollisionData->Indices.Add(Triangle);
		CollisionData->MaterialIndices.Add(i);
	}

	CollisionData->bFlipNormals = true;

	return true;
}

bool UPersonalMeshComponent::ContainsPhysicsTriMeshData(bool InUseAllTriData) const
{
	return (PersonalMeshTris.Num() > 0);
}

void UPersonalMeshComponent::UpdateBodySetup()
{
	if (ModelBodySetup == NULL)
	{
		ModelBodySetup = NewObject<UBodySetup>(this, NAME_None, (IsTemplate() ? RF_Public : RF_NoFlags));
		ModelBodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;
		ModelBodySetup->bMeshCollideAll = true;
	}
}

void UPersonalMeshComponent::UpdateCollision()
{
	if (bPhysicsStateCreated)
	{
		DestroyPhysicsState();
		UpdateBodySetup();
		CreatePhysicsState();

		// Works in Packaged build only since UE4.5:
		ModelBodySetup->InvalidatePhysicsData();
		ModelBodySetup->CreatePhysicsMeshes();
	}
}

UBodySetup* UPersonalMeshComponent::GetBodySetup()
{
	UpdateBodySetup();
	return ModelBodySetup;
}
