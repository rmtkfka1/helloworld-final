#pragma once
#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#include "SimpleMath.h"
#include "SimpleMath.inl"



#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>


// °¢Á¾ lib
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")


#ifdef _DEBUG
#pragma comment(lib, "Assimp/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "Assimp/assimp-vc143-mt.lib")
#endif

#include <iostream>


using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;
using vec2 = DirectX::SimpleMath::Vector2;
using vec3 = DirectX::SimpleMath::Vector3;
using vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Color = DirectX::XMFLOAT4;



struct Vertex
{
	vec3 position = { 0, 0, 0 };
	vec2 uv = { 0, 0 };
	vec3 normal = { 0, 0, 0 };
	vec3 tangent = { 0, 0, 0 };
	vec4 blendIndices = { 0, 0, 0, 0 };
	vec4 blendWeights = { 0, 0, 0, 0 };

};



struct asBone
{
	string name;
	int32 index = -1;
	int32 parent = -1;
	Matrix transform;


};

struct asMesh
{
	string name{};
	vector<Vertex> vertices;
	vector<uint32> indices;

	int32 boneIndex{};
	string materialName{};

	BoundingBox box;
	BoundingSphere sphere;

};

struct asMaterial
{
	string shader = "default.hlsl";
	string name{};
	Color ambient{};
	Color diffuse{};
	Color specular{};
	Color emissive{};
	string diffuseFile{};
	string specularFile{};
	string normalFile{};
};

#include "tinyxml2.h"
