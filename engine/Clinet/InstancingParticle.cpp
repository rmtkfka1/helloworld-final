#include "pch.h"
#include "InstancingParticle.h"
#include "Core.h"
#include "Model.h"
#include "Mesh.h"
#include "Transform.h"
#include "Material.h"
#include "CameraManager.h"
#include "TableHeap.h"
#include <random>
#include "Scene.h"
#include "SceneManger.h"
#include "StructedBuffer.h"
#include "TransformTree.h"

InstancingParticle::InstancingParticle()
{
}

InstancingParticle::~InstancingParticle()
{
}

void InstancingParticle::Init()
{
	uniform_real_distribution<float> uid(0, 1.0f);
	random_device rd;
	default_random_engine dre(rd());


	GameObject::Init();

	_color.x = uid(dre);
	_color.y = uid(dre);
	_color.z = uid(dre);

	std::uniform_real_distribution<float> uuid(-1.0f, 1.0f);


	for (auto& i : _direction)
	{
		i.x = uuid(dre);
		i.y = uuid(dre);
		i.z = uuid(dre);
	}
	
	////////
	_data.resize(InstanceCount);

	_buffer = make_shared<StructedBuffer>();
	_buffer->Init(sizeof(InstanceData), InstanceCount);

	for (int i = 0; i < InstanceCount; ++i)
	{
		_data[i].worldmat = Matrix::CreateTranslation(vec3(0, 0, i * _model->GetSize().z * 2));
	}

	_buffer->PushData((_data.data()));

}

void InstancingParticle::Update()
{
	float dt = TimeManager::GetInstance()->GetDeltaTime();

	_elapsedTime += dt;

	if (_elapsedTime > 5.0f)
	{
		//5초후 수명이다하면 삭제한다.
		SceneManger::GetInstance()->GetCurrentScene()->ReserveDeleteGameObject(shared_from_this());
	}


	_offset += dt * _speed;

	GameObject::Update();
}

void InstancingParticle::Render()
{
	for (int i = 0; i < InstanceCount; ++i)
	{
		_data[i].worldmat = Matrix::CreateTranslation(
			vec3(_direction[i].x * _offset + _ownerPosition.x
				, _direction[i].y * _offset + _ownerPosition.y
				, _direction[i].z * _offset + _ownerPosition.z));
	}

	_buffer->PushData((_data.data()));


	vector<shared_ptr<ModelMesh>>& meshData = _model->GetMeshes();

	for (auto& data : meshData)
	{
		core->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { data->meshes->GetVertexView(), _buffer->GetBufferView() };
		core->GetCmdList()->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);
		core->GetCmdList()->IASetIndexBuffer(&data->meshes->GetIndexView());

		_transformTree->findByName(data->name)->PushData();

		if (data->material)
		{
			data->material->SetFloat(0, _color.x);
			data->material->SetFloat(1, _color.y);
			data->material->SetFloat(2, _color.z);
			data->material->Update();
		}

		_buffer->SetGraphicsRootShaderResourceView();
		core->GetTableHeap()->SetGraphicsRootDescriptorTable();
		core->GetCmdList()->DrawIndexedInstanced(data->meshes->GetIndexCount(), InstanceCount, 0, 0, 0);
	}
}
