
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

cbuffer TileMap : register(b3)
{
    float2 gTileFrameStart;
    float2 gTileFrameEnd;
}

VS_Output_Tex TileMapVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    
    float3 Pos = input.Pos - gPivot;
        
    // output.Pos의 x, y, z 에는 input.Pos의 x, y, z 가 대입되고 w에는 1이 대입된다.
    output.Pos = mul(float4(Pos, 1.f), gmatWVP);
    output.UV = input.UV;
    
    return output;
}

PS_Output_Single TileMapPS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 Color = gBaseTexture.Sample(gBaseSampler, input.UV);
    
    output.Color = Color;
    
    return output;
}

VS_Output_Tex TileVS(VS_Input_Tex input)
{
    VS_Output_Tex output = (VS_Output_Tex) 0;
    
    float3 Pos = input.Pos - gPivot;
        
    // output.Pos의 x, y, z 에는 input.Pos의 x, y, z 가 대입되고 w에는 1이 대입된다.
    output.Pos = mul(float4(Pos, 1.f), gmatWVP);
    
    // 왼쪽 점일 경우
    if (input.UV.x == 0.f)
        output.UV.x = gTileFrameStart.x;
    
    else
        output.UV.x = gTileFrameEnd.x;
    
    // 위쪽 점일 경우
    if (input.UV.y == 0.f)
        output.UV.y = gTileFrameStart.y;
    
    else
        output.UV.y = gTileFrameEnd.y;
    
    return output;
}

PS_Output_Single TilePS(VS_Output_Tex input)
{
    PS_Output_Single output = (PS_Output_Single) 0;
    
    float4 Color = gBaseTexture.Sample(gBaseSampler, input.UV);
    
    output.Color = Color;
    
    return output;
}
