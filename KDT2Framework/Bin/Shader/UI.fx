
#include "Share.fx"

struct VS_Input_Tex
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VS_Output_Tex
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

cbuffer UI : register(b3)
{
    float4 gBrushTint;
    float4 gWidgetColor;
    float2 gBrushAnimLTUV;
    float2 gBrushAnimRBUV;
    int gBrushAnimEnable;
    int gBrushTextureEnable;
    float2 gBrushEmpty;
}

float2 UpdateAnimationUI(float2 UV)
{
    if (gBrushAnimEnable == 0)
        return UV;
    
    float2 Result = (float2) 0.f;
    
    // 왼쪽 점일 경우
    if (UV.x == 0.f)
        Result.x = gBrushAnimLTUV.x;
    else
        Result.x = gBrushAnimRBUV.x;
    
    // 위쪽 점일 경우
    if (UV.y == 0.f)
        Result.y = gBrushAnimLTUV.y;
    else
        Result.y = gBrushAnimRBUV.y;
    
    return Result;
}

VS_Output_Tex UIVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    
    float3 Pos = input.Pos - gPivot;
        
    // output.Pos의 x, y, z 에는 input.Pos의 x, y, z 가 대입되고 w에는 1이 대입된다.
    output.Pos = mul(float4(Pos, 1.f), gmatWVP);
    output.UV = UpdateAnimationUI(input.UV);
    
    return output;
}

PS_Output_Single UIPS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 Color = float4(1.f, 1.f, 1.f, 1.f);
    
    if (gBrushTextureEnable)
        Color = gBaseTexture.Sample(gBaseSampler, input.UV);
    
    output.Color = Color * gBrushTint * gWidgetColor;
    
    return output;
}
