#include "pch.h"
#include "Material.h"
#include "core.h"
#include "TableHeap.h"
#include "Texture.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "CameraManager.h"
#include "GameObject.h"
#include "Transform.h"
Material::Material():ResourceBase(ResourceType::Material)
{
	
}
Material::~Material()
{
}
void Material::Update()
{
	// 파이프라인 세팅
	_shader->SetPipelineState();

	if (_diffuseTexture)
	{

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0));
		core->GetTableHeap()->SetSRV(_diffuseTexture->GetCpuHandle(), reg);
	}


	if (_normalTexture)
	{
		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t1));
		core->GetTableHeap()->SetSRV(_normalTexture->GetCpuHandle(), reg);
		_params.SetInt(0, 1);
	}
	else
	{
		_params.SetInt(0, 0);
	}

	if (_SpecularTexture)
	{
		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t2));
		core->GetTableHeap()->SetSRV(_SpecularTexture->GetCpuHandle(), reg);
		_params.SetInt(1, 1);
	}

	else
	{
		_params.SetInt(1, 0);
	}

	_desc.cameraPos = CameraManager::GetInstance()->_cameraPos;

	core->GetConstantBuffer(CBV_REGISTER::b2)->PushData(&_desc, sizeof(_desc));
	core->GetConstantBuffer(CBV_REGISTER::b3)->PushData(&_params, sizeof(_params));
}
