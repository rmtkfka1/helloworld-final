#include "pch.h"
#include "Stage1.h"
#include "Core.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "SceneManger.h"
#include "GameObject.h"
#include "CameraManager.h"
#include "Helper.h"
#include "Material.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "ImguiManager.h"
#include "BoxCollider.h"
#include "CollisonManager.h"
#include "ObjectManager.h"
#include "SphereCollider.h"

#include "Lamp.h"
#include "LightManager.h"
#include <random>
#include "Player.h"
#include "House.h"
#include "Terrain.h"
#include "Enemy.h"
#include "TransformTree.h"


default_random_engine dre;
uniform_int_distribution<int> uid(0, 3);
uniform_real_distribution<float> uidZ(2000.0f, 18000.0f);

Stage1::Stage1()
{

}

Stage1::~Stage1()
{
	LightManager::GetInstnace()->_lightParmas.lightCount = 0;
	CollisonManager::GetInstance()->Clear();

}

void Stage1::Init()
{

	AddLight();
	
	{
		shared_ptr<Enemy> ememy = make_shared<Enemy>();

		shared_ptr<Model> model = Model::ReadData(L"Tank/Tank");
		ememy->SetModel(model);
		ememy->AddComponent(make_shared<BoxCollider>());

		ememy->GetTransformTree()->GetRoot()->SetLocalScale(vec3(1.0f, 1.0f, 1.0f));
		ememy->GetTransformTree()->GetRoot()->SetLocalPosition(vec3(-5000.0f, 0, 0));
		ObjectManager::GetInstance()->_enemy = ememy;
		AddGameObject(ememy);
	}

	{
		shared_ptr<Player> player = make_shared<Player>();

		shared_ptr<Model> model = Model::ReadData(L"helicpoter/helicpoter");
		player->SetModel(model);
		player->AddComponent(make_shared<BoxCollider>());

		player->GetTransformTree()->GetRoot()->SetLocalPosition(vec3(-3000.0f,300.0f, 0));
		ObjectManager::GetInstance()->_player = player;
		AddGameObject(player);
	}



	{

		shared_ptr<Terrain> terrain = make_shared<Terrain>();

		shared_ptr<Mesh> mesh = Helper::MakeSquareGrid(200, 200, 5000.0f, vec2(30, 30));
		terrain->AddMesh(mesh);

		shared_ptr<Material> material = make_shared<Material>();

		material->SetName(L"terrian_mateiral");

		shared_ptr<Texture> diffuse = make_shared<Texture>();
		diffuse->Init(L"../Resources/Texture/terrian/terrain.png");

		shared_ptr<Texture> height = make_shared<Texture>();
		height->Init(L"../Resources/Texture/terrian/he.png");

		material->SetHeightTexture(height);
		material->SetDiffuseTexture(diffuse);

		shared_ptr<Shader> shader = make_shared<Shader>();
		ShaderInfo info;
	

		shader->Init(L"color.hlsl", info);

		shared_ptr<Transform> transform = make_shared<Transform>();
		terrain->_transform = transform;

		terrain->_transform->SetLocalPosition(vec3(-1000.0f, -50.0f, 0));
		terrain->_transform->SetLocalRotation(vec3(XMConvertToRadians(90.0f), 0, 0));

		material->SetShader(shader);

		terrain->AddMaterial(material);

		AddGameObject(terrain);
	}




	CameraManager::GetInstance()->Init();
	Super::Init();

}

void Stage1::Update()
{

	LightManager::GetInstnace()->SetData();
	CameraManager::GetInstance()->Update();
	Super::Update();
}

void Stage1::LateUpdate() 
{
	Super::LateUpdate();
}

void Stage1::Render()
{
	LightManager::GetInstnace()->SetData();
	Super::Render();
	CollisonManager::GetInstance()->Update();

}

void Stage1::ClearScene()
{
	Super::ClearScene();

	if (_sceneEnd)
	{
		SceneManger::GetInstance()->ChangeScene(SceneType::STAGE1);
	}

}

void Stage1::AddLight()
{
	LightInfo info;

	info.lightType = static_cast<int32>(LIGHT_TYPE::DIRECTIONAL_LIGHT);
	info.color.ambient = vec4(0.1f, 0.1f, 0.1f, 0.1f);
	info.color.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	info.color.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	info.direction = vec4(0, -1.0f, 0, 0);
	
	LightManager::GetInstnace()->PushLight(info);

}


