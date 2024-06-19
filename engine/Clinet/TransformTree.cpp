#include "pch.h"
#include "TransformTree.h"
#include "GameObject.h"
#include "Transform.h"
#include "Model.h"
#include "Helper.h"
TransformTree::TransformTree():Component(COMPONENT_TYPE::TRASNFORMTREE)
{

}

TransformTree::~TransformTree()
{
}

void TransformTree::MakeTransformTree()
{

	vector<shared_ptr<ModelBone>>& bones= GetOwner()->GetModel()->GetBones();
	unordered_map<int32, shared_ptr<Transform>> map;

	for (auto& bone : bones)
	{
		if (bone->index == 0)
		{
			_root = make_shared<Transform>();
	

			Matrix m = bone->transformData;

			vec3 pos;
			Quaternion rotation;
			vec3 scale;
			m.Decompose(scale, rotation, pos);

			_root->_name = bone->name;
			_root->SetLocalPosition(pos);
			_root->SetLocalRotation(Helper::ToEulerAngles(rotation));
			_root->SetLocalScale(scale);
			_root->Update();

			map[bone->index] = _root;
			_nameMap[_root->_name] = _root;
		}

		else
		{
			shared_ptr<Transform> transform = make_shared<Transform>();

			Matrix m = bone->transformData;

			vec3 pos;
			Quaternion rotation;
			vec3 scale;
			m.Decompose(scale, rotation, pos);

			transform->_name = bone->name;
			transform->SetLocalPosition(pos);
			transform->SetLocalRotation(Helper::ToEulerAngles(rotation));
			transform->SetLocalScale(scale);
			transform->Update();

			transform->SetParent(map[bone->parentIndex]);
			transform->GetParent().lock()->AddChild(transform);
			map[bone->index] = transform;
			_nameMap[transform->_name] = transform;
		}
	}

	vector<shared_ptr<ModelMesh>>& meshes = GetOwner()->GetModel()->GetMeshes();

	for (auto& data : meshes)
	{
		shared_ptr<Transform> transform = _nameMap[data->name];

		if(transform)
			transform->SetCenter(data->box.Center);

		if (transform)
			transform->SetSize(data->box.Extents);

	}

}


shared_ptr<Transform>& TransformTree::findByName(wstring name)
{
	auto it = _nameMap.find(name);

	if (it != _nameMap.end()) {
		return it->second;
	}

	assert(false);
}
