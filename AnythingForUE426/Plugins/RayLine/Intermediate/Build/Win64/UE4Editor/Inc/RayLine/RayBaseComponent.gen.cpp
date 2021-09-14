// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "RayLine/Public/RayBaseComponent.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeRayBaseComponent() {}
// Cross Module References
	RAYLINE_API UScriptStruct* Z_Construct_UScriptStruct_FRayLineHitPointDesc();
	UPackage* Z_Construct_UPackage__Script_RayLine();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	RAYLINE_API UClass* Z_Construct_UClass_URayBaseComponent_NoRegister();
	RAYLINE_API UClass* Z_Construct_UClass_URayBaseComponent();
	ENGINE_API UClass* Z_Construct_UClass_UMeshComponent();
// End Cross Module References
class UScriptStruct* FRayLineHitPointDesc::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern RAYLINE_API uint32 Get_Z_Construct_UScriptStruct_FRayLineHitPointDesc_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FRayLineHitPointDesc, Z_Construct_UPackage__Script_RayLine(), TEXT("RayLineHitPointDesc"), sizeof(FRayLineHitPointDesc), Get_Z_Construct_UScriptStruct_FRayLineHitPointDesc_Hash());
	}
	return Singleton;
}
template<> RAYLINE_API UScriptStruct* StaticStruct<FRayLineHitPointDesc>()
{
	return FRayLineHitPointDesc::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FRayLineHitPointDesc(FRayLineHitPointDesc::StaticStruct, TEXT("/Script/RayLine"), TEXT("RayLineHitPointDesc"), false, nullptr, nullptr);
static struct FScriptStruct_RayLine_StaticRegisterNativesFRayLineHitPointDesc
{
	FScriptStruct_RayLine_StaticRegisterNativesFRayLineHitPointDesc()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("RayLineHitPointDesc")),new UScriptStruct::TCppStructOps<FRayLineHitPointDesc>);
	}
} ScriptStruct_RayLine_StaticRegisterNativesFRayLineHitPointDesc;
	struct Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_HitPosition_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_HitPosition;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_HitNextDir_MetaData[];
#endif
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_HitNextDir;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_HitPointIndex_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_HitPointIndex;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/RayBaseComponent.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FRayLineHitPointDesc>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPosition_MetaData[] = {
		{ "Category", "RayLineHitPoint" },
		{ "ModuleRelativePath", "Public/RayBaseComponent.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPosition = { "HitPosition", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FRayLineHitPointDesc, HitPosition), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPosition_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPosition_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitNextDir_MetaData[] = {
		{ "Category", "RayLineHitPoint" },
		{ "ModuleRelativePath", "Public/RayBaseComponent.h" },
	};
#endif
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitNextDir = { "HitNextDir", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FRayLineHitPointDesc, HitNextDir), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitNextDir_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitNextDir_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPointIndex_MetaData[] = {
		{ "Category", "RayLineHitPoint" },
		{ "ModuleRelativePath", "Public/RayBaseComponent.h" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPointIndex = { "HitPointIndex", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FRayLineHitPointDesc, HitPointIndex), METADATA_PARAMS(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPointIndex_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPointIndex_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPosition,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitNextDir,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::NewProp_HitPointIndex,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_RayLine,
		nullptr,
		&NewStructOps,
		"RayLineHitPointDesc",
		sizeof(FRayLineHitPointDesc),
		alignof(FRayLineHitPointDesc),
		Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FRayLineHitPointDesc()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FRayLineHitPointDesc_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_RayLine();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("RayLineHitPointDesc"), sizeof(FRayLineHitPointDesc), Get_Z_Construct_UScriptStruct_FRayLineHitPointDesc_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FRayLineHitPointDesc_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FRayLineHitPointDesc_Hash() { return 941522499U; }
	void URayBaseComponent::StaticRegisterNativesURayBaseComponent()
	{
	}
	UClass* Z_Construct_UClass_URayBaseComponent_NoRegister()
	{
		return URayBaseComponent::StaticClass();
	}
	struct Z_Construct_UClass_URayBaseComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_DebugSec_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_DebugSec;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_URayBaseComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UMeshComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_RayLine,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URayBaseComponent_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Rendering" },
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Object LOD Physics Collision Mobility Trigger" },
		{ "IncludePath", "RayBaseComponent.h" },
		{ "ModuleRelativePath", "Public/RayBaseComponent.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_URayBaseComponent_Statics::NewProp_DebugSec_MetaData[] = {
		{ "Category", "RayLineComp" },
		{ "ModuleRelativePath", "Public/RayBaseComponent.h" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UClass_URayBaseComponent_Statics::NewProp_DebugSec = { "DebugSec", nullptr, (EPropertyFlags)0x0010000000000005, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(URayBaseComponent, DebugSec), METADATA_PARAMS(Z_Construct_UClass_URayBaseComponent_Statics::NewProp_DebugSec_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_URayBaseComponent_Statics::NewProp_DebugSec_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_URayBaseComponent_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_URayBaseComponent_Statics::NewProp_DebugSec,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_URayBaseComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<URayBaseComponent>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_URayBaseComponent_Statics::ClassParams = {
		&URayBaseComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_URayBaseComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_URayBaseComponent_Statics::PropPointers),
		0,
		0x00B010A4u,
		METADATA_PARAMS(Z_Construct_UClass_URayBaseComponent_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_URayBaseComponent_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_URayBaseComponent()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_URayBaseComponent_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(URayBaseComponent, 4180983736);
	template<> RAYLINE_API UClass* StaticClass<URayBaseComponent>()
	{
		return URayBaseComponent::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_URayBaseComponent(Z_Construct_UClass_URayBaseComponent, &URayBaseComponent::StaticClass, TEXT("/Script/RayLine"), TEXT("URayBaseComponent"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(URayBaseComponent);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
