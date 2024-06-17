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
		shared_ptr<Player> player = make_shared<Player>();

		shared_ptr<Model> model = Model::ReadData(L"helicpoterss/helicpoter");
		player->SetModel(model);
		player->AddComponent(make_shared<BoxCollider>());

		player->GetModel()->GetRoot()->transform->SetLocalPosition(vec3(0, 300.0f, 0));
		ObjectManager::GetInstance()->_player = player;
		AddGameObject(player);
	}

	{
		shared_ptr<House> house = make_shared<House>();

		shared_ptr<Model> model = Model::ReadData(L"mushroom/mushroom");
		house->SetModel(model);

		house->AddComponent(make_shared<BoxCollider>());
		house->GetModel()->GetRoot()->transform->SetLocalScale(vec3(20.0f, 20.0f, 20.0f));
		house->GetModel()->GetRoot()->transform->SetLocalRotation(vec3(XMConvertToRadians(30.0f), 0, 0));
		AddGameObject(house);
	}


	{

		shared_ptr<Terrain> terrain = make_shared<Terrain>();

		shared_ptr<Mesh> mesh = Helper::MakeSquareGrid(1000, 1000, 5000.0f, vec2(100, 100));
		terrain->AddMesh(mesh);

		shared_ptr<Material> material = make_shared<Material>();

		material->SetName(L"terrian_mateiral");

		shared_ptr<Shader> shader = make_shared<Shader>();
		ShaderInfo info;
		info.rasterizerType = RASTERIZER_TYPE::WIREFRAME;

		shader->Init(L"color.hlsl", info);

		shared_ptr<Transform> transform = make_shared<Transform>();
		terrain->SetTransform(transform);

		terrain->_transform->SetLocalPosition(vec3(0, -50.0f, 0));
		terrain->_transform->SetLocalRotation(vec3(90.0f, 0, 0));

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


