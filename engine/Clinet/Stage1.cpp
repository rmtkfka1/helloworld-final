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


	CameraManager::GetInstance()->Init();
	

	{
		shared_ptr<GameObject> player = make_shared<GameObject>();

		shared_ptr<Model> model = Model::ReadData(L"helicpoter/helicpoter");
		player->SetModel(model);
		player->AddComponent(make_shared<BoxCollider>());

		AddGameObject(player);
	}

	Super::Init();

}

void Stage1::Update()
{
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


