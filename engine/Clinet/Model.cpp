#include "pch.h"
#include "Mesh.h"
#include "Model.h"
#include "tinyxml2.h"
#include "Utils.h"
#include "Material.h"
#include "ResourceManager.h"
#include "FileUtils.h"
#include <fstream>
#include <iostream>
#include "Transform.h"
#include "Helper.h"
Model::Model():ResourceBase(ResourceType::Model),Component(COMPONENT_TYPE::MODEL)
{

}

Model::~Model()
{

}

shared_ptr<Model> Model::ReadData(wstring filename)
{
	
	shared_ptr<Model> model;
	/*ResourceManager::GetInstance()->Get<Model>(filename);*/

	//if (model)
	//{
	//	return model;
	//};

	model = make_shared<Model>();
	model->SetName(filename);
	model->ReadMaterial(filename);
	model->ReadModel(filename);
	//ResourceManager::GetInstance()->Add<Model>(filename, model);
	
	return model;
}


void Model::Init(const wstring& path)
{
}

void Model::ReadMaterial(wstring filename)
{
	wstring fullPath = _texturePath + filename + L".xml";
	auto parentPath = filesystem::path(fullPath).parent_path();
	auto shaderPath = L"../Resources/Shader/";

	tinyxml2::XMLDocument* document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = document->LoadFile(Utils::ToString(fullPath).c_str());
	assert(error == tinyxml2::XML_SUCCESS);

	tinyxml2::XMLElement* root = document->FirstChildElement();
	tinyxml2::XMLElement* materialNode = root->FirstChildElement();

	while (materialNode)
	{
		shared_ptr<Material> material = make_shared<Material>();

		tinyxml2::XMLElement* node = nullptr;

		node = materialNode->FirstChildElement();
		material->SetName(Utils::ToWString(node->GetText()));

		//shader
		node = node->NextSiblingElement();

		if (node->GetText())
		{
			wstring shaderStr = Utils::ToWString(node->GetText());
			if (shaderStr.length() > 0)
			{
				auto shader = ResourceManager::GetInstance()->Load<Shader>(shaderStr, shaderStr);
				shader->SetName(shaderStr);
				material->SetShader(shader);
			}
		}

		// Diffuse Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = ResourceManager::GetInstance()->Load<Texture>(textureStr, (parentPath / textureStr).wstring());
				texture->SetName(textureStr);
				material->SetDiffuseTexture(texture);
			}
		}

		// Specular Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring texture = Utils::ToWString(node->GetText());
			if (texture.length() > 0)
			{
				wstring textureStr = Utils::ToWString(node->GetText());
				if (textureStr.length() > 0)
				{
					auto texture = ResourceManager::GetInstance()->Load<Texture>(textureStr, (parentPath / textureStr).wstring());
					texture->SetName(textureStr);
					material->SetSpecularTexture(texture);
				}
			}
		}

		// Normal Texture
		node = node->NextSiblingElement();
		if (node->GetText())
		{
			wstring textureStr = Utils::ToWString(node->GetText());
			if (textureStr.length() > 0)
			{
				auto texture = ResourceManager::GetInstance()->Load<Texture>(textureStr, (parentPath / textureStr).wstring());
				texture->SetName(textureStr);
				material->SetNormalTexture(texture);
			}
		}

		// Ambient
		{
			node = node->NextSiblingElement();

			vec4 _color;
			_color.x = node->FloatAttribute("R");
			_color.y = node->FloatAttribute("G");
			_color.z = node->FloatAttribute("B");
			_color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().ambient = _color;
		}

		// Diffuse
		{
			node = node->NextSiblingElement();

			vec4 _color;
			_color.x = node->FloatAttribute("R");
			_color.y = node->FloatAttribute("G");
			_color.z = node->FloatAttribute("B");
			_color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().diffuse = _color;
		}

		// Specular
		{
			node = node->NextSiblingElement();

			vec4 _color;
			_color.x = node->FloatAttribute("R");
			_color.y = node->FloatAttribute("G");
			_color.z = node->FloatAttribute("B");
			_color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().specular = _color;
		}

		// Emissive
		{
			node = node->NextSiblingElement();

			vec4 _color;
			_color.x = node->FloatAttribute("R");
			_color.y = node->FloatAttribute("G");
			_color.z = node->FloatAttribute("B");
			_color.w = node->FloatAttribute("A");
			material->GetMaterialDesc().emissive = _color;
		}

		_materialData.push_back(material);

		// Next Material
		materialNode = materialNode->NextSiblingElement();
	}

	BindCacheInfo();
}

void Model::ReadModel(wstring filename)
{
	wstring fullPath = _modelPath + filename + L".mesh";

	ifstream in{ fullPath ,ios::binary };

	assert(in);

	//bone 정보 읽기
	{
		int count;
		in.read((char*)&count, sizeof(int));
		_boneData.reserve(count);

		for (int i = 0; i < count; ++i)
		{
			shared_ptr<ModelBone> newbone = make_shared<ModelBone>();

			{
				//bone->name
				size_t len{};
				in.read((char*)&len, sizeof(size_t));

				string temp;
				temp.resize(len);
				in.read((char*)temp.data(), len);

				newbone->name = wstring{ temp.begin(),temp.end() };
			}

			{
				//index , parent index
				in.read((char*)&newbone->index, sizeof(int32));
				in.read((char*)&newbone->parentIndex, sizeof(int32));
			}

			{
				//매트릭스정보
				XMFLOAT4X4 tempData;
				in.read((char*)(&tempData), sizeof(XMFLOAT4X4));
				newbone->transformData = XMLoadFloat4x4(&tempData);
			}

			_boneData.push_back(newbone);
		}

	};


	//mesh 정보읽기

	{
		int count;
		in.read((char*)&count, sizeof(int));
		_meshData.reserve(count);

		for (int i = 0; i < count; ++i)
		{
			shared_ptr<ModelMesh> newMesh = make_shared<ModelMesh>();
			newMesh->meshes = make_shared<Mesh>();

			{
				//name 읽기
				size_t len;
				in.read((char*)&len, sizeof(size_t));

				string temp;
				temp.resize(len);
				in.read((char*)temp.data(), len);

				newMesh->name = wstring{ temp.begin(), temp.end() };
			}

			{
				//boneindex 읽기
				in.read((char*)&newMesh->boneIndex, sizeof(int32));
			}

			{
				//materialnmae 읽기
				size_t len;
				in.read((char*)&len, sizeof(size_t));

				string temp;
				temp.resize(len);
				in.read((char*)temp.data(), len);

				newMesh->materialName = wstring{ temp.begin(), temp.end() };

			}

			{
				//vertices 읽기
				size_t len;
				in.read((char*)&len, sizeof(size_t));

				vector<Vertex> vertices;
				vertices.resize(len);

				for (int j = 0; j < len; ++j)
				{
					in.read((char*)&vertices[j].position, sizeof(vec3));
					in.read((char*)&vertices[j].uv, sizeof(vec2));
					in.read((char*)&vertices[j].normal, sizeof(vec3));
					in.read((char*)&vertices[j].tangent, sizeof(vec3));
					in.read((char*)&vertices[j].blendIndices, sizeof(vec4));
					in.read((char*)&vertices[j].blendWeights, sizeof(vec4));
				}



				//indices 읽기
				uint32 len2;
				in.read((char*)&len2, sizeof(uint32));
				vector<uint32> indicies;
				indicies.resize(len2);

				for (uint32 j = 0; j < len2; ++j)
				{
					in.read((char*)&indicies[j], sizeof(uint32));
				}


				{
					in.read((char*)&newMesh->box.Center, sizeof(vec3));
					in.read((char*)&newMesh->box.Extents, sizeof(vec3));
				}

				//mesh 만들기
				newMesh->meshes->Init(vertices, indicies);
				_meshData.push_back(newMesh);
			}

		}
	}

	{

		in.read((char*)&_totalCenter, sizeof(vec3));
		in.read((char*)&_totalSize, sizeof(vec3));
		in.read((char*)&_totalRadius, sizeof(float));
	}

	BindCacheInfo();
	
	MakeTransform();
}

void Model::MakeTransform()
{
	for (auto& bone : _boneData)
	{
		bone->transform = make_shared<Transform>();

		Matrix m = bone->transformData;

		vec3 pos;
		Quaternion rotation;
		vec3 scale;
		m.Decompose(scale, rotation, pos);

		bone->transform->_name = bone->name;
		bone->transform->SetLocalPosition(pos);
		bone->transform->SetLocalRotation(Helper::ToEulerAngles(rotation));
		bone->transform->SetLocalScale(scale);
		bone->transform->Update();
		
		if (bone->parentIndex > 0)
		{
			bone->transform->SetParent(bone->parent->transform);
			bone->parent->transform->AddChild(bone->transform);
		}
	}

	for (auto& data : _meshData)
	{
		data->bone->transform->SetCenter(data->box.Center);
	}

	
}


std::shared_ptr<Material> Model::GetMaterialByName(const wstring& name)
{
	for (auto& material : _materialData)
	{
		if (material->GetName() == name)
			return material;
	}

	return nullptr;
}

std::shared_ptr<ModelMesh> Model::GetMeshByName(const wstring& name)
{
	for (auto& mesh : _meshData)
	{
		if (mesh->name == name)
			return mesh;
	}

	return nullptr;
}

std::shared_ptr<ModelBone> Model::GetBoneByName(const wstring& name)
{
	for (auto& bone : _boneData)
	{
		if (bone->name == name)
			return bone;
	}

	return nullptr;
}

void Model::PrintInfo()
{
	cout << "사이즈: " << _totalSize.x << " " << _totalSize.y << " " << _totalSize.z << endl;
	cout << "중심점: " << _totalCenter.x << " " << _totalCenter.y << " " << _totalCenter.z << endl;

	cout << endl;
}

void Model::BindCacheInfo()
{
	/*Mesh에 Material 캐싱*/
	for (const auto& mesh : _meshData)
	{
		// 이미 찾았으면 스킵
		if (mesh->material != nullptr)
			continue;

		mesh->material = GetMaterialByName(mesh->materialName);
	}

	// Mesh에 Bone 캐싱
	for (const auto& mesh : _meshData)
	{
		// 이미 찾았으면 스킵
		if (mesh->bone != nullptr)
			continue;

		mesh->bone = GetBoneByIndex(mesh->boneIndex);
	}

	// Bone 계층 정보 채우기
	if (_root == nullptr && _boneData.size() > 0)
	{
		_root = _boneData[1];

		for (const auto& bone : _boneData)
		{
			if (bone->parentIndex >= 0)
			{
				bone->parent = _boneData[bone->parentIndex];
				bone->parent->children.push_back(bone);
			}
			else
			{
				bone->parent = nullptr;
			}

		}
	}
}
