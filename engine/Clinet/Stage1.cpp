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
	

	{
		shared_ptr<Player> player = make_shared<Player>();

		shared_ptr<Model> model = Model::ReadData(L"helicpoterss/helicpoter");
		player->SetModel(model);
		player->AddComponent(make_shared<BoxCollider>());
		ObjectManager::GetInstance()->_player = player;
		AddGameObject(player);
	}

	{
		shared_ptr<GameObject> gameobject = make_shared<GameObject>();
		shared_ptr<Transform> transform = make_shared<Transform>();
		gameobject->_transform = transform;
		shared_ptr<Model> model = Model::ReadData(L"sphere/sphere");

		auto& mateirals = model->GetMaterials();

		ShaderInfo info{ RASTERIZER_TYPE::CULL_FRONT,DEPTH_STENCILE_TYPE::LESS_EQUAL };
		for (auto& mateiral : mateirals)
		{
			shared_ptr<Shader> shader = make_unique<Shader>();
			shader->Init(L"sky.hlsl", info);
			mateiral->SetShader(shader);
		}

		gameobject->SetModel(model);
		AddGameObject(gameobject);
	}


	CameraManager::GetInstance()->Init();
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


