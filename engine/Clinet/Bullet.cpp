#include "pch.h"
#include "Bullet.h"
#include "ObjectManager.h"
#include "Player.h"
#include "TransformTree.h"
#include "Transform.h"
#include "SceneManger.h"
#include "CollisonManager.h"
#include "Scene.h"
Bullet::Bullet():GameObject(GAMEOBJECT_TYPE::Bullet)
{

}

Bullet::~Bullet()
{

}

void Bullet::Init()
{
	GameObject::Init();
	_direction = ObjectManager::GetInstance()->_player.lock()->GetTransformTree()->GetRoot()->GetLook();
	

}

void Bullet::Update()
{
	PosUpdate();
	DeleteBullet();
	GameObject::Update();
}

void Bullet::Render()
{
	GameObject::Render();
}

void Bullet::DeleteBullet()
{

	
	_distance += 100.0f * TimeManager::GetInstance()->GetDeltaTime();

	if (_distance > 20000.0f)
	{
		SceneManger::GetInstance()->GetCurrentScene()->ReserveDeleteGameObject(shared_from_this());
		CollisonManager::GetInstance()->RemoveCollider(_collider);
	}

}

void Bullet::PosUpdate()
{
	float dt = TimeManager::GetInstance()->GetDeltaTime();
	vec3 pos = _transformTree->GetRoot()->GetLocalPosition();
	pos += _direction * _speed * dt;
	_transformTree->GetRoot()->SetLocalPosition(pos);
}
