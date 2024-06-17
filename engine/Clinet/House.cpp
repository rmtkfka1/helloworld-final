#include "pch.h"
#include "House.h"
#include "Model.h"
#include "Mesh.h"
#include "Core.h"
#include "Transform.h"
#include "Material.h"
#include "TableHeap.h"

void House::Init()
{
}

void House::Update()
{
	_transform->Update();
}


void House::Render()
{
	
	vector<shared_ptr<ModelMesh>>& meshData = GetModel()->GetMeshes();

	for (auto& data : meshData)
	{
		core->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->GetCmdList()->IASetVertexBuffers(0, 1, &data->meshes->GetVertexView());
		core->GetCmdList()->IASetIndexBuffer(&data->meshes->GetIndexView());

		/*data->bone->transform->PushData();*/
		_transform->PushData();

		if (data->material)
		{
			data->material->Update();
		}

		core->GetTableHeap()->SetGraphicsRootDescriptorTable();
		core->GetCmdList()->DrawIndexedInstanced(data->meshes->GetIndexCount(), 1, 0, 0, 0);
	}




}
