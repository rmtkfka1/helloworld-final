#include "ResourceBase.h"
#include "Component.h"
class Material;
class Mesh;
class Transform;

struct ModelBone
{
	wstring name;
	int32 index;
	int32 parentIndex;
	Matrix transformData;


	shared_ptr<ModelBone> parent; // Cache
	vector<shared_ptr<ModelBone>> children; // Cache
};


struct ModelMesh 
{
	wstring name;

	// Mesh
	shared_ptr<Mesh> meshes;
	// Material
	wstring materialName = L"";
	shared_ptr<Material> material; // Cache

	// Bones
	int32 boneIndex;
	BoundingOrientedBox box;
	shared_ptr<ModelBone> bone; // Cache;

};

class Model :public ResourceBase ,public Component
{
public:
	Model();
	~Model();

	static shared_ptr<Model> ReadData(wstring filename);

public:
	void Init(const wstring& path);
	void ReadMaterial(wstring filename);
	void ReadModel(wstring filename);



public:
	shared_ptr<ModelBone> GetRoot() { return _root; }

	uint32 GetMaterialCount() { return static_cast<uint32>(_materialData.size()); }
	vector<shared_ptr<Material>>& GetMaterials() { return _materialData; }
	shared_ptr<Material> GetMaterialByIndex(uint32 index) { return _materialData[index]; }
	shared_ptr<Material> GetMaterialByName(const wstring& name);

	uint32 GetMeshCount() { return static_cast<uint32>(_meshData.size()); }
	vector<shared_ptr<ModelMesh>>& GetMeshes() { return _meshData; }
	shared_ptr<ModelMesh> GetMeshByIndex(uint32 index) { return _meshData[index]; }
	shared_ptr<ModelMesh> GetMeshByName(const wstring& name);

	uint32 GetBoneCount() { return static_cast<uint32>(_boneData.size()); }
	vector<shared_ptr<ModelBone>>& GetBones() { return _boneData; }
	shared_ptr<ModelBone> GetBoneByIndex(uint32 index) { return (index < 0 || index >= _boneData.size() ? nullptr : _boneData[index]); }
	shared_ptr<ModelBone> GetBoneByName(const wstring& name);

	void PrintInfo();

	vec3 GetCenter() { return _totalCenter; }
	vec3 GetSize() { return _totalSize; }
	float GetRadius() { return _totalRadius; }

private:
	void BindCacheInfo();

private:
	wstring _modelPath = L"../Resources/Model/";
	wstring _texturePath = L"../Resources/Texture/";

private:
	shared_ptr<ModelBone>           _root;
	vector<shared_ptr<Material>>    _materialData;
	vector<shared_ptr<ModelBone>>   _boneData;
	vector<shared_ptr<ModelMesh>>   _meshData;

public:

	vec3 _totalCenter{};
	vec3 _totalSize{};
	BoundingBox box;
	float _totalRadius{};
};



