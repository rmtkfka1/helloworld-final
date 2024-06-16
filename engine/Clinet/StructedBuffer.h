#pragma once


class StructedBuffer
{

public:
	void Init(uint32 elementSize, uint32 elementCount);
	void CreateBuffer();
	void PushData(void* buffer);
	void SetGraphicsRootShaderResourceView();

	D3D12_VERTEX_BUFFER_VIEW GetBufferView() { return _bufferView; }

private:
	ComPtr<ID3D12Resource>		_buffer;  
	D3D12_VERTEX_BUFFER_VIEW	_bufferView;

	uint32 _elementSize{};
	uint32 _elementCount{};
};

