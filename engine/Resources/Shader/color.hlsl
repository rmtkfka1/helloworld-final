
#include "Common.hlsl"
#include "helper.hlsl"

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;

    
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
    float3 worldPos : POSITION;
    float3 worldNormal : NORMAL;
};




VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, WorldMatrix);
    output.worldPos = output.pos;
    output.worldNormal = mul(float4(input.normal, 0.0f), WorldMatrix);
    
    float height = height_texture.SampleLevel(sam_0, input.uv, 0).r;
    height = height * 2.0f - 1.0;
    
    output.pos += float4(output.worldNormal * height * 100, 0.0f);
    
    output.pos = mul(output.pos, ViewMatrix);
    output.pos = mul(output.pos, ProjectionMatrix);
    

    output.uv = input.uv;
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = basic_texture.Sample(sam_0, input.uv);
    
    input.worldNormal = normalize(input.worldNormal);
    
    LightColor totalColor = (LightColor) 0.f;
    
    for (int i = 0; i < g_lightCount; ++i)
    {
        LightColor lightcolor = CalculateLightColor(i, input.worldNormal, input.worldPos.xyz);
        
        totalColor.diffuse += lightcolor.diffuse;
        totalColor.ambient += lightcolor.ambient;
        totalColor.specular += lightcolor.specular;
    }
    
    color.xyz = (totalColor.diffuse.xyz * color.xyz) + (totalColor.ambient.xyz * color.xyz) + (totalColor.specular.xyz * color.xyz);
    
   
    return color;

}