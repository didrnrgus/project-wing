
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

VS_Output_Tex MeshVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
        
    // output.Pos의 x, y, z 에는 input.Pos의 x, y, z 가 대입되고 w에는 1이 대입된다.
    output.Pos = mul(float4(input.Pos, 1.f), gmatWVP);
    output.UV = input.UV;
    
    return output;
}

PS_Output_Single DefaultMaterialPS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single)0;
    
    float4 Color = gBaseTexture.Sample(gBaseSampler, input.UV);
    
    Color.rgb *= gMtrlBaseColor.rgb;
    
    Color.a *= gMtrlOpacity;
    
    output.Color = Color;
    
    
    return output;
}
