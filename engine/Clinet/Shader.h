#pragma once
#include "ResourceBase.h"

enum class RASTERIZER_TYPE
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRAME,
};

enum class DEPTH_STENCILE_TYPE
{
	LESS, //±âº»°ª
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL
};

struct ShaderInfo
{
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCILE_TYPE depthStencilType = DEPTH_STENCILE_TYPE::LESS;
};

class Shader :public ResourceBase
{
public:
	Shader();
	virtual ~Shader();

	void Init(const wstring& path , ShaderInfo info = ShaderInfo());
	void SetPipelineState();

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ComPtr<ID3DBlob>					_vsBlob;
	ComPtr<ID3DBlob>					_psBlob;
	ComPtr<ID3DBlob>					_errBlob;

	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC  _pipelineDesc = {};

	wstring _path = L"../Resources/Shader/";



};

