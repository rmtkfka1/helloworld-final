#ifndef _HELPER_HLSL_
#define _HELPER_HLSL_

#include "Common.hlsl"

LightColor CalculateLightColor(int lightIndex, float3 normal, float3 worldPos)
{
    LightColor color = (LightColor) 0.f;
    
 
    if(g_light[lightIndex].lightType==0)
    {
        float Power = 1.0f;
        // Directional Light
        color.ambient = g_light[lightIndex].lightColor.ambient;
        
        float3 lightDir = normalize(-g_light[lightIndex].direction);
        
        color.diffuse = g_light[lightIndex].lightColor.diffuse * saturate(dot(lightDir, normal));
        float3 viewDir = normalize(cameraPos.xyz - worldPos);
        float3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 500.0f);
        float specularPower = 0.0f;
        color.specular = spec * g_light[lightIndex].lightColor.specular;
        
        color.diffuse *= Power;
        color.ambient *= Power;
        color.specular *= Power;
    }
    
    else if (g_light[lightIndex].lightType == 1)
    {
        //PointLight
       
        float dist = length(g_light[lightIndex].position.xyz - worldPos);
        float3 distPoly = float3(1.0, dist, dist * dist);
        
        float attenuation = 1.0 / dot(distPoly, g_light[lightIndex].attenuation);
        float3 lightDir = normalize(g_light[lightIndex].position.xyz - worldPos);
        float3 viewDir = normalize(cameraPos.xyz - worldPos);
        float3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 500.0f);
        float specularPower = 0.0f;
    
        color.ambient = g_light[lightIndex].lightColor.ambient * attenuation;
        color.diffuse = g_light[lightIndex].lightColor.diffuse * saturate(dot(lightDir, normal)) * attenuation;
        color.specular = specularPower * spec * g_light[lightIndex].lightColor.specular * attenuation;
        
    }
    else if (g_light[lightIndex].lightType == 2)
    {
        
        color.ambient = g_light[lightIndex].lightColor.ambient;
        
        float dist = length(g_light[lightIndex].position.xyz - worldPos);
        float3 distPoly = float3(1.0, dist, dist * dist);
        
        float attenuation = 1.0 / dot(distPoly, g_light[lightIndex].attenuation);
        float3 lightDir = normalize(g_light[lightIndex].position.xyz - worldPos);
        
        float theta = dot(lightDir, normalize(-g_light[lightIndex].direction.xyz));
        
        if(theta> cutoff)
        {
            normal = normalize(normal);
            
            float diff = max(dot(normal, lightDir), 0.0);
            color.diffuse = g_light[lightIndex].lightColor.diffuse * diff;
            
            float3 viewDir = normalize(cameraPos.xyz - worldPos);
            float3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 500.0f);
            float specularPower = 0.0f;
            color.specular = specularPower * spec * g_light[lightIndex].lightColor.specular;
            
            color.diffuse *= attenuation;
            color.ambient *= attenuation;
            color.specular *= attenuation;
            
        }
    }
    
  
    return color;
    
}

#endif