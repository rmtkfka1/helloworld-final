#include "pch.h"
#include "Mesh.h"
#include "Core.h"
#include "ConstantBuffer.h"
#include "TableHeap.h"
#include "Texture.h"
Mesh::Mesh() :ResourceBase(ResourceType::Mesh)
{

}
Mesh::~Mesh()
{
}
void Mesh::Init(vector<Vertex>& vec, vector<uint32>& index)
{
	CreateVertxBuffer(vec);
	CreateIndexBuffer(index);
}

void Mesh::CreateVertxBuffer(vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	//扁夯赛积己
	{
		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

		auto hr = core->GetDevice()->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&_defaultVertexBuffer));

	}
	//uploadBuffer 积己
	{


		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

		core->GetDevice()->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_uploadVertexBuffer));

		void* data = nullptr;
		CD3DX12_RANGE readRange(0, 0);
		_uploadVertexBuffer->Map(0, &readRange, &data);
		::memcpy(data, &vec[0], bufferSize);
		_uploadVertexBuffer->Unmap(0, nullptr);

	}

	{
		//汗荤累诀
		core->GetResourceCmdList()->CopyResource(_defaultVertexBuffer.Get(), _uploadVertexBuffer.Get());

		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			_defaultVertexBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		core->GetResourceCmdList()->ResourceBarrier(1, &barrier);
		core->ResourceSet();

		_vertexBufferView.BufferLocation = _defaultVertexBuffer->GetGPUVirtualAddress();
		_vertexBufferView.StrideInBytes = sizeof(Vertex);
		_vertexBufferView.SizeInBytes = bufferSize;

	}

}

void Mesh::CreateIndexBuffer(vector<uint32>& buffer)
{
	_indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _indexCount * sizeof(uint32);


	{
		//defaultheap积己
		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

		core->GetDevice()->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_COPY_DEST,
			nullptr,
			IID_PPV_ARGS(&_defaultIndexBuffer));
	}


	{
		//uploadBuffer 积己
		D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

		core->GetDevice()->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&_uploadIndexBuffer));

		void* indexDataBuffer = nullptr;
		CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
		_uploadIndexBuffer->Map(0, &readRange, &indexDataBuffer);
		::memcpy(indexDataBuffer, buffer.data(), bufferSize);
		_uploadIndexBuffer->Unmap(0, nullptr);


	}

	{
		//汗荤累诀

		core->GetResourceCmdList()->CopyResource(_defaultIndexBuffer.Get(), _uploadIndexBuffer.Get());

		D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			_defaultIndexBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_INDEX_BUFFER);

		core->GetResourceCmdList()->ResourceBarrier(1, &barrier);
		core->ResourceSet();

		_indexBufferView.BufferLocation = _defaultIndexBuffer->GetGPUVirtualAddress();
		_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		_indexBufferView.SizeInBytes = bufferSize;

	}

};
