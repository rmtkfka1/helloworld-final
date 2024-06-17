#pragma once
#include "ResourceBase.h"
class Shader;
class Texture;


struct MaterialParams
{
	void SetInt(uint8 index, int32 value) { intParams[index] = value; }
	void Setfloat(uint8 index, float value) { floatParams[index] = value; }
	array<int32, 4> intParams{};
	array<float, 4> floatParams{};

};

class Material:public ResourceBase
{
public:
	Material();
	virtual ~Material();

	shared_ptr<Shader>& GetShader() { return _shader; }
	MaterialDesc& GetMaterialDesc() { return _desc; }

	void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
	void SetInt(uint8 index, int32 value) { _params.SetInt(index, value); }
	void SetFloat(uint8 index, float value) { _params.Setfloat(index, value); }
	void SetDiffuseTexture(shared_ptr<Texture> texture) { _diffuseTexture = texture; }
	void SetNormalTexture(shared_ptr<Texture> texture) { _normalTexture = texture; }
	void SetSpecularTexture(shared_ptr<Texture> texture) { _SpecularTexture = texture; }
	void SetHeightTexture(shared_ptr<Texture> texture) { _HeightTexture = texture; }
	void Update();

private:
	MaterialDesc		_desc;
	MaterialParams		_params;

	shared_ptr<Shader>	_shader;
	shared_ptr<Texture> _diffuseTexture;
	shared_ptr<Texture> _normalTexture;
	shared_ptr<Texture> _SpecularTexture;
	shared_ptr<Texture> _HeightTexture;
};

