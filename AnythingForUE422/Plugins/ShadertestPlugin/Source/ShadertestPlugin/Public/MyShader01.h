#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Classes/Kismet/BlueprintFunctionLibrary.h"
#include "MyShader01.generated.h"

USTRUCT(BlueprintType)
struct FMyShader01StructData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = ShaderData)
        FLinearColor ColorOne;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = ShaderData)
        FLinearColor ColorTwo;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = ShaderData)
        FLinearColor Colorthree;
    UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = ShaderData)
        FLinearColor ColorFour;
};


UCLASS(MinimalAPI, meta = (ScriptName = "ShaderLibrary"))
class UShader01BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

    //ʹ��meta = (WorldContext = "WorldContextObject")��ǿ�������ͼ�аѵ�ǰ�����Ķ�����
	UFUNCTION(BlueprintCallable, Category = "ShadertestPlugin", meta = (worldContext = "WorldContextObject"))
		static void DrawTestShader01RenderTarget(class UTextureRenderTarget2D* OutputRenderTarget, AActor* Ac, FLinearColor MyColor, UTexture* MyTexture);

};