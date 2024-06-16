#include "Common.hlsl"
#include "helper.hlsl"
struct instanceData
{
    row_major matrix InstnaceWorldMatrix;
};


struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    uint nInstanceID : SV_InstanceID;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 Worldpos : Position;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    uint nInstanceID : SV_InstanceID;
};
    

StructuredBuffer<instanceData> instanceInfo : register(t5);


VS_OUT VS_Main(VS_IN input, uint nInstanceID : SV_InstanceID)
{
    VS_OUT output = (VS_OUT) 0;

 
    output.pos = float4(input.pos, 1.0f);
    output.pos = mul(output.pos, instanceInfo[nInstanceID].InstnaceWorldMatrix);
    output.pos = mul(output.pos, ViewMatrix);
    output.pos = mul(output.pos, ProjectionMatrix);
    
    output.Worldpos = mul(float4(input.pos, 1.0f), instanceInfo[nInstanceID].InstnaceWorldMatrix);
    output.normal = mul(float4(input.normal, 0.0f), instanceInfo[nInstanceID].InstnaceWorldMatrix);

    output.uv = input.uv;
    
    return output;
  
}

float4 PS_Main(VS_OUT input) : SV_Target
{
 
    float4 color = basic_texture.Sample(sam_0, input.uv);
    input.normal = normalize(input.normal);
    
    LightColor totalColor = (LightColor) 0.f;
    
    for (int i = 0; i < g_lightCount; ++i)
    {
        LightColor lightcolor = CalculateLightColor(i, input.normal, input.Worldpos.xyz);
        
        totalColor.diffuse += lightcolor.diffuse;
        totalColor.ambient += lightcolor.ambient;
        totalColor.specular += lightcolor.specular;
    }
    
    color.xyz = (totalColor.diffuse.xyz * color.xyz) + (totalColor.ambient.xyz * color.xyz) + (totalColor.specular.xyz * color.xyz);
    
    return color;
}