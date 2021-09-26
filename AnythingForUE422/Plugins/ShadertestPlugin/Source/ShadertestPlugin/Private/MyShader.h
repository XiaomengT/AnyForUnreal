#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Classes/Kismet/BlueprintFunctionLibrary.h"
#include "MyShader.generated.h"

UCLASS(MinimalAPI, meta = (ScriptName = "ShaderLibrary"))
class UShaderBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()


	UFUNCTION(BlueprintCallable, Category = "ShadertestPlugin", meta = (worldContext = "WorldContextObject"))
		static void DrawTestShaderRenderTarget(class UTextureRenderTarget2D* OutputRenderTarget, AActor* Ac, FLinearColor MyColor);

};