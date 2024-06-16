#include "pch.h"
#include "StructedBuffer.h"
#include "Core.h"
void StructedBuffer::Init(uint32 elementSize, uint32 elementCount)
{
	_elementSize = elementSize;
	_elementCount = elementCount;

	CreateBuffer();

}

void StructedBuffer::CreateBuffer()
{

	const uint64 bufferSize = _elementSize * _elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	core->GetDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_buffer));

}


void StructedBuffer::PushData(void* buffer)
{
	const uint64 bufferSize = _elementSize * _elementCount;

	void* dataBuffer = nullptr;
	D3D12_RANGE readRange{ 0, 0 };
	_buffer->Map(0, &readRange, &dataBuffer);
	memcpy(dataBuffer, buffer, bufferSize);

	_bufferView.BufferLocation = _buffer->GetGPUVirtualAddress();
	_bufferView.StrideInBytes = _elementSize;
	_bufferView.SizeInBytes = bufferSize;
}

void StructedBuffer::SetGraphicsRootShaderResourceView()
{
	core->GetCmdList()->SetGraphicsRootShaderResourceView(2, _buffer->GetGPUVirtualAddress());
}



