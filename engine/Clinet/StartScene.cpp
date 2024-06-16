#include "pch.h"
#include "StartScene.h"
#include "GameObject.h"
#include "Model.h"
#include "Transform.h"
#include "CameraManager.h"
#include "Shader.h"
#include "Material.h"
#include "SceneManger.h"
#include "ImguiManager.h"
#include "ResourceManager.h"
StartScene::StartScene()
{
	
}

StartScene::~StartScene()
{
}

void StartScene::Init()
{
	Scene::Init();

	{
		shared_ptr<GameObject> gameobject = make_shared<GameObject>();
		shared_ptr<Model> model = Model::ReadData(L"pressenter/pressenter");
		gameobject->SetModel(model);
		AddGameObject(gameobject);
	}


	

	CameraManager::GetInstance()->Init();



}

void StartScene::Update()
{
	Scene::Update();

	CameraManager::GetInstance()->Update();

	float dt = TimeManager::GetInstance()->GetDeltaTime();

	static float value = 0;
	static bool down = true;

	if (down)
	{
		if (value > 39.9f)
		{
			down = false;
		}

		if (value < 40.0f)
		{
			value += 20 * dt;
			_gameObjects[0]->_transform->SetLocalRotation(vec3(value, 0, 0));
		}

	}

	else
	{

		if (value < 0.1f)
		{
			down = true;
		}

		if (value > 0)
		{
			value -= 20 * dt;
			_gameObjects[0]->_transform->SetLocalRotation(vec3(value, 0, 0));
		}

	}

	CameraManager::GetInstance()->Update();



}

void StartScene::LateUpdate()
{
	Scene::LateUpdate();
}

void StartScene::Render()
{
	Scene::Render();


}

void StartScene::ClearScene()
{
	Scene::ClearScene();

	if (KeyManager::GetInstance()->GetButtonDown(KEY_TYPE::ENTER))
	{
		SceneManger::GetInstance()->ChangeScene(SceneType::STAGE1);
		ResourceManager::GetInstance()->Clear();
		CameraManager::GetInstance()->Clear();
	}

}
