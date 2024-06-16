#pragma once
#include "ResourceBase.h"
class Texture;
class Mesh:public ResourceBase
{
public:

	Mesh();
	virtual ~Mesh();

	void Init(vector<Vertex>& vec, vector<uint32>& index);
	D3D12_VERTEX_BUFFER_VIEW& GetVertexView() { return _vertexBufferView; }
	D3D12_INDEX_BUFFER_VIEW& GetIndexView() { return _indexBufferView; }

	uint32 GetIndexCount() { return _indexCount; }

private:
	void CreateVertxBuffer(vector<Vertex>& vec);
	void CreateIndexBuffer(vector<uint32>& index);


public:
	ComPtr<ID3D12Resource>		_uploadVertexBuffer;
	ComPtr<ID3D12Resource>		_defaultVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	ComPtr<ID3D12Resource>		_uploadIndexBuffer;
	ComPtr<ID3D12Resource>		_defaultIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW		_indexBufferView = {};
	uint32 _indexCount = 0;


};

