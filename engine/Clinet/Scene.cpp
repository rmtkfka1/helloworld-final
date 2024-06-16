#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "Core.h"
#include "ConstantBuffer.h"
#include "CameraManager.h"
#include "Transform.h"
Scene::Scene()
{

}



Scene::~Scene()
{
}

void Scene::Init()
{
	for (auto& i : _gameObjects)
	{
		i->Init();
	}

}

void Scene::Update()
{
	for (auto& i : _gameObjects)
	{
		i->Update();
	}


}

void Scene::LateUpdate()
{

	while (!_reserveAddQueue.empty()) 
	{
		shared_ptr<GameObject> element = _reserveAddQueue.front();
		AddGameObject(element);
		_reserveAddQueue.pop();
	}

	while (!_reserveDeleteQueue.empty())
	{
		shared_ptr<GameObject> element = _reserveDeleteQueue.front();
		DeleteGameObject(element);
		_reserveDeleteQueue.pop();
	}

}

void Scene::Render()
{

	for (auto& i : _gameObjects)
	{
		i->Render();
	}
}

void Scene::ClearScene()
{

}

void Scene::ReserveAddGameObject(shared_ptr<GameObject> object)
{
	_reserveAddQueue.push(object);
}

void Scene::ReserveDeleteGameObject(shared_ptr<GameObject> object)
{
	_reserveDeleteQueue.push(object);
}

void Scene::AddGameObject(shared_ptr<GameObject> object)
{
	_gameObjects.push_back(object);
}

void Scene::DeleteGameObject(shared_ptr<GameObject> object)
{
	
	auto it = std::remove(_gameObjects.begin(), _gameObjects.end(), object);
	_gameObjects.erase(it, _gameObjects.end());

}


