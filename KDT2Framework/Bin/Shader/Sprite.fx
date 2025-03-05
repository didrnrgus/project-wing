
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

cbuffer Sprite : register(b3)
{
    float4 gSpriteTint;
}

VS_Output_Tex SpriteVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    
    float3 Pos = input.Pos - gPivot;
        
    // output.Pos의 x, y, z 에는 input.Pos의 x, y, z 가 대입되고 w에는 1이 대입된다.
    output.Pos = mul(float4(Pos, 1.f), gmatWVP);
    output.UV = UpdateAnimation2D(input.UV);
    
    return output;
}

PS_Output_Single SpritePS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 Color = gBaseTexture.Sample(gBaseSampler, input.UV);
    
    output.Color = Color * gSpriteTint;
    
    return output;
}
