#include "GameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"


class ResourceManager
{
public:

	static ResourceManager* GetInstance()
	{
		static ResourceManager instance;
		return &instance;
	}


public:
	
	void Clear();


	template<typename T>
	shared_ptr<T> Load(const wstring& key, const wstring& path);

	template<typename T>
	shared_ptr<T> Get(const wstring& Key);

	template<typename T>
	void Add(const wstring& Key, shared_ptr<T> object);

	template<typename T>
	ResourceType GetResourceType();

	shared_ptr<Shader> LoadShader(const wstring& key, const wstring& path, ShaderInfo info);
	shared_ptr<Mesh>   LoadMesh(const wstring& key, const wstring& path, vector<Vertex> v , vector<uint32> indices);



public:
	using KeyObjMap = std::map<wstring, shared_ptr<ResourceBase>>;
	array<KeyObjMap, RESOURCE_TYPE_COUNT> _resources;
};

template<typename T>
inline shared_ptr<T> ResourceManager::Load(const wstring& key, const wstring& path)
{
	ResourceType objectType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	//¸øÂüÀ½ 
	shared_ptr<T> object = make_shared<T>();
	object->Init(path);
	keyObjMap[key] = object;

	return object;
}


inline void ResourceManager::Clear()
{
	for (int i = 0; i < RESOURCE_TYPE_COUNT; ++i)
	{
		_resources[i].clear();
	}

}

inline shared_ptr<Shader> ResourceManager::LoadShader(const wstring& key, const wstring& path ,ShaderInfo info)
{
	ResourceType objectType = GetResourceType<Shader>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);

	if (findIt != keyObjMap.end())
	{
		return static_pointer_cast<Shader>(findIt->second);
	}

	
	shared_ptr<Shader> shader = make_shared<Shader>();
	shader->Init(path,info);

	keyObjMap[key] = shader;

	return shader;
}

inline shared_ptr<Mesh> ResourceManager::LoadMesh(const wstring& key, const wstring& path, vector<Vertex> v, vector<uint32> indices)
{
	ResourceType objectType = GetResourceType<Mesh>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);

	if (findIt != keyObjMap.end())
	{
		return static_pointer_cast<Mesh>(findIt->second);
	}

	shared_ptr<Mesh> mesh = make_shared<Mesh>();
	mesh->Init(v, indices);

	keyObjMap[key] = mesh;

	return mesh;
}


template<typename T>
shared_ptr<T> ResourceManager::Get(const wstring& key)
{
	ResourceType objectType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];

	auto findIt = keyObjMap.find(key);
	if (findIt != keyObjMap.end())
		return static_pointer_cast<T>(findIt->second);

	return nullptr;
}

template<typename T>
inline void ResourceManager::Add(const wstring& Key, shared_ptr<T> object)
{
	ResourceType objectType = GetResourceType<T>();
	KeyObjMap& keyObjMap = _resources[static_cast<uint8>(objectType)];


	keyObjMap[Key] = object;
}



template<typename T>
inline ResourceType ResourceManager::GetResourceType()
{
	if (std::is_same_v<T, Mesh>)
		return ResourceType::Mesh;
	else if (std::is_same_v<T, Shader>)
		return ResourceType::Shader;
	else if (std::is_same_v<T, Model>)
		return ResourceType::Model;
	else if (std::is_same_v<T, Texture>)
		return ResourceType::Texture;
	else
		return ResourceType::None;
}