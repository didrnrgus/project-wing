#pragma once

#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"

struct FTransformCBufferInfo
{
    FMatrix matWorld;
    FMatrix matView;
    FMatrix matProj;
    FMatrix matWV;
    FMatrix matWVP;
    FVector3D   Pivot;
    float       Empty;
};

struct FMaterialCBufferInfo
{
    FVector4D BaseColor;
    float Opacity;
    int TextureWidth;
    int TextureHeight;
    float Empty;
};

struct FColliderCBufferInfo
{
    FVector4D   Color;
};

struct FSpriteCBufferInfo
{
    FVector4D   Tint;
};

struct FAnimation2DCBufferInfo
{
    FVector2D   LTUV;
    FVector2D   RBUV;
    int         AnimEnable;
    FVector3D   Empty;
};

struct FUICBufferInfo
{
    FVector4D   BrushTint;
    FVector4D   Color = FVector4D::White;
    FVector2D   LTUV;
    FVector2D   RBUV;
    int         BrushAnimEnable;
    int         BrushTextureEnable;
    FVector2D   BrushEmpty;
};

struct FTileMapCBufferInfo
{
    FVector2D   LTUV;
    FVector2D   RBUV;
};