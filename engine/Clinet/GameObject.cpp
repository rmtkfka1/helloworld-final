#include "pch.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Core.h"
#include "TableHeap.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "Shader.h"
#include "Transform.h"
#include "CameraManager.h"
#include "Material.h"
#include "Model.h"
#include "ImguiManager.h"
#include "Scripts.h"
#include "Component.h"
#include "BoxCollider.h"
#include "CollisonManager.h"
#include "SphereCollider.h"
#include "StructedBuffer.h"


GameObject::GameObject()
{
	_type = GAMEOBJECT_TYPE::None;
}

GameObject::GameObject(GAMEOBJECT_TYPE type):_type(type)
{

}


GameObject::~GameObject()
{
}

void GameObject::Init()
{

	for (auto& compoent : _component)
	{
		compoent->Init();
	}

}

void GameObject::Update()
{
	
	vector<shared_ptr<ModelBone>>& meshData = _model->GetBones();


	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::LEFT))
	{
		static float temp = 0.001f;
		temp += 0.001f;

		for (auto& data : meshData)
		{
			if (data->name == L"turret_geo")
			{
				auto& pos = data->transform->GetLocalRotation();
				data->transform->SetLocalRotation(vec3(pos.x,pos.y+temp,pos.z));
			}
		}
	}


	for (auto& data : meshData)
	{
		data->transform->Update();
	}

	//for (auto& compoent : _component)
	//{
	//	compoent->Update();
	//}

}

void GameObject::Render()
{

	for (auto& compoent : _component)
	{
		compoent->Render();
	}

	vector<shared_ptr<ModelMesh>>& meshData = _model->GetMeshes();

	for (auto& data : meshData)
	{
		core->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->GetCmdList()->IASetVertexBuffers(0, 1, &data->meshes->GetVertexView());
		core->GetCmdList()->IASetIndexBuffer(&data->meshes->GetIndexView());


		data->bone->transform->PushData();

		if (data->material)
		{
			data->material->Update();
		}

		core->GetTableHeap()->SetGraphicsRootDescriptorTable();
		core->GetCmdList()->DrawIndexedInstanced(data->meshes->GetIndexCount(), 1, 0, 0, 0); 
	
	}

}

void GameObject::Render(uint32 instance , shared_ptr<StructedBuffer> buffer)
{

	for (auto& compoent : _component)
	{
		compoent->Render();
	}

	vector<shared_ptr<ModelMesh>>& meshData = _model->GetMeshes();

	for (auto& data : meshData)
	{
		core->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		D3D12_VERTEX_BUFFER_VIEW pVertexBufferViews[] = { data->meshes->GetVertexView(), buffer->GetBufferView()};
		core->GetCmdList()->IASetVertexBuffers(0, _countof(pVertexBufferViews), pVertexBufferViews);
		core->GetCmdList()->IASetIndexBuffer(&data->meshes->GetIndexView());

		data->bone->transform->PushData();

		if (data->material)
		{
			data->material->Update();
		}

		buffer->SetGraphicsRootShaderResourceView();
		core->GetTableHeap()->SetGraphicsRootDescriptorTable();
		core->GetCmdList()->DrawIndexedInstanced(data->meshes->GetIndexCount(), instance, 0, 0, 0);
	}
}

//COMPONET 이지만 따로관리 
void GameObject::SetTransform(shared_ptr<Transform> transform)
{
	_transform = transform;
	_transform->SetTotalCenter(_totalCenter);
	_transform->SetSize(_totalSize);

}



void GameObject::SetModel(shared_ptr<Model> model)
{
	_totalCenter = model->GetCenter();
	_totalSize = model->GetSize();
	_totalRadius = model->GetRadius();
	_model = model;
}



////////////////////////////////////////////////

void GameObject::AddComponent(shared_ptr<Component> component)
{
	component->SetOwner(shared_from_this());

	if (component->GetComponentType()== COMPONENT_TYPE::COLLIDER)
	{
		if (static_pointer_cast<BaseCollider>(component)->GetColliderType() == ColliderType::Box)
		{
			static_pointer_cast<BoxCollider>(component)->SetTotalCenter(this->_totalCenter);
			static_pointer_cast<BoxCollider>(component)->SetSize(this->_totalSize);
			static_pointer_cast<BoxCollider>(component)->MakeBoundingBox();
			CollisonManager::GetInstance()->AddCollider(static_pointer_cast<BoxCollider>(component));
			_collider = static_pointer_cast<BoxCollider>(component);
		}

		else if (static_pointer_cast<BaseCollider>(component)->GetColliderType() == ColliderType::Sphere)
		{
			shared_ptr<SphereCollider> collider = static_pointer_cast<SphereCollider>(component);
			collider->SetTotalCenter(this->_totalCenter);
			collider->SetRadius(this->_totalRadius);
			collider->MakeBoundingSphere();
			CollisonManager::GetInstance()->AddCollider(collider);
			_collider = collider;
		}
	}


	_component.push_back(component);
}


void GameObject::OnComponentBeginOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider> other)
{


}
void GameObject::OnComponentEndOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider> other)
{


}

