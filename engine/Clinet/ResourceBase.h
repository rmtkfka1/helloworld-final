#pragma once

enum class ResourceType : int8
{
	None = 0,
	Mesh,
	Model,
	Shader,
	Texture,
	Material,
	End
};

enum
{
	RESOURCE_TYPE_COUNT = static_cast<int8>(ResourceType::End)
};

class ResourceBase
{
public:
	friend class ResourceManager;

	ResourceBase(ResourceType type);
	virtual ~ResourceBase();

	ResourceType GetType() { return _type; }
	void SetName(const wstring& name) { _name = name; }
	const wstring& GetName() { return _name; }

protected:

	virtual void Load(const wstring& path) { }
	virtual void Save(const wstring& path) { }

protected:

	ResourceType _type = ResourceType::None;
	wstring _name;
	wstring _path;
};
