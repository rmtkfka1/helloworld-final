#include "pch.h"
#include "LightManager.h"
#include "Core.h"
#include "ConstantBuffer.h"



int LightManager::PushLight(LightInfo info)
{
	assert(_lightParmas.lightCount < 100);

	_lightParmas.LightInfos[_lightParmas.lightCount] = info;
	_lightParmas.lightCount++;

	return (_lightParmas.lightCount) -1;

}

void LightManager::SetData()
{
	core->GetConstantBuffer(CBV_REGISTER::b0)->SetData(&_lightParmas, sizeof(LightParams));
}
