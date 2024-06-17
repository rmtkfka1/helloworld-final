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
		shared_ptr<GameObject> house = make_shared<GameObject>();

		shared_ptr<Model> model = Model::ReadData(L"mushroom/mushroom");
		house->SetModel(model);

		house->AddComponent(make_shared<BoxCollider>());
		house->GetModel()->GetRoot()->transform->SetLocalScale(vec3(20.0f, 20.0f, 20.0f));
		AddGameObject(house);
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


