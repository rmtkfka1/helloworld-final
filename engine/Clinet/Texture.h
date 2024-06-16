#pragma once

#include "ResourceBase.h"

class Texture:public ResourceBase
{
public:
	Texture();
	virtual ~Texture();
	void Init(const wstring& path);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }

public:

	virtual void Load(const wstring& path) override;
	virtual void Save(const wstring& path) override;

	void CreateTexture(const wstring& path);
	void CreateView();

private:
	ScratchImage			 		_image;
	ComPtr<ID3D12Resource>			_tex2D;

	ComPtr<ID3D12DescriptorHeap>	_srvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_srvHandle;
};

