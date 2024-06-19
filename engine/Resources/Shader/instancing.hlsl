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
   
    
    return output;
  
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    return float4(hit, float_1, float_2, 1.0f);
}