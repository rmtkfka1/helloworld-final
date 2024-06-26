#include "pch.h"
#include "Terrain.h"
#include "Core.h"
#include "Mesh.h"
#include "Material.h"
#include "TableHeap.h"
#include "Transform.h"
#include "Component.h"
#include <random>
Terrain::Terrain():GameObject(GAMEOBJECT_TYPE::Terrain)
{
}

Terrain::~Terrain()
{
}

void Terrain::Init()
{


}

void Terrain::Update()
{

	_transform->Update();

	for (auto& i : _component)
	{
		i->Update();
	}

	
}

void Terrain::Render()
{
	default_random_engine dre;
	uniform_real_distribution<float> uid(-50.0f, 100.0f);

	for (auto& i : _component)
	{
		i->Render();
	}


	core->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	core->GetCmdList()->IASetVertexBuffers(0, 1, &_mesh->GetVertexView());
	core->GetCmdList()->IASetIndexBuffer(&_mesh->GetIndexView());


	_transform->PushData();
	_material->SetFloat(0, uid(dre));
	_material->Update();

	
	core->GetTableHeap()->SetGraphicsRootDescriptorTable();

	core->GetCmdList()->DrawIndexedInstanced(_mesh->GetIndexCount(), 1, 0, 0, 0);
	

}
