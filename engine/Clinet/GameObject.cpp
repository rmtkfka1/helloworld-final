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

	for (auto& data : meshData)
	{
		data->transform->Update();
	}

	for (auto& compoent : _component)
	{
		compoent->Update();
	}

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


void GameObject::SetTransform(shared_ptr<Transform> transform)
{
	_rootTransform = transform;
}

void GameObject::MakeModelTransform()
{
	_rootTransform = make_shared<Transform>();

	for (auto& bone : _model->GetBones())
	{
		//모델에 저장되있는 계층적 데이터를 가져옴
		Matrix m = bone->transformData;

		vec3 pos;
		Quaternion rotation;
		vec3 scale;
		m.Decompose(scale, rotation, pos);

		_rootTransform->_name = bone->name;
		
		_rootTransform->SetLocalPosition(pos);
		_rootTransform->SetLocalRotation(Helper::ToEulerAngles(rotation));
		_rootTransform->SetLocalScale(scale);
		_rootTransform->Update();

		if (bone->parentIndex > 0)
		{
			_rootTransform->SetParent();
			bone->parent->transform->AddChild(bone->transform);
		}
	};
		
	for (auto& data : _meshData)
	{
		data->bone->transform->SetCenter(data->box.Center);
	};
}



void GameObject::SetModel(shared_ptr<Model> model)
{
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
			static_pointer_cast<BoxCollider>(component)->SetTotalCenter(_model->GetCenter());
			static_pointer_cast<BoxCollider>(component)->SetSize(_model->GetSize());
			static_pointer_cast<BoxCollider>(component)->MakeBoundingBox();
			CollisonManager::GetInstance()->AddCollider(static_pointer_cast<BoxCollider>(component));
			_collider = static_pointer_cast<BoxCollider>(component);
		}

		else if (static_pointer_cast<BaseCollider>(component)->GetColliderType() == ColliderType::Sphere)
		{
			shared_ptr<SphereCollider> collider = static_pointer_cast<SphereCollider>(component);
			collider->SetTotalCenter(_model->GetCenter());
			collider->SetRadius(_model->GetRadius());
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

