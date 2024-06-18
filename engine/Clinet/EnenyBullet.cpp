#include "pch.h"
#include "EnenyBullet.h"
#include "ObjectManager.h"
#include "Transform.h"
#include "Player.h"
#include "TransformTree.h"
#include "SceneManger.h"
#include "Scene.h"
#include "CollisonManager.h"
#include "Enemy.h"
void EnenyBullet::Init()
{
	GameObject::Init();

	auto& pos1 = ObjectManager::GetInstance()->_player.lock()->GetTransformTree()->GetRoot()->_position;
	auto& pos2 = ObjectManager::GetInstance()->_enemy.lock()->GetTransformTree()->findByName(L"canon_geo")->_position;

	vec3 result = pos1 - pos2;
	result.Normalize();
	_direction = result;
}
void EnenyBullet::Update()
{
	PosUpdate();
	DeleteBullet();
	GameObject::Update();

}


void EnenyBullet::Render()
{
	GameObject::Render();
}

void EnenyBullet::DeleteBullet()
{

	_distance += 100.0f * TimeManager::GetInstance()->GetDeltaTime();

	if (_distance > 2000.0f)
	{
		SceneManger::GetInstance()->GetCurrentScene()->ReserveDeleteGameObject(shared_from_this());
		CollisonManager::GetInstance()->RemoveCollider(_collider);
	}

}

void EnenyBullet::PosUpdate()
{
	float dt = TimeManager::GetInstance()->GetDeltaTime();
	vec3 pos = _transformTree->GetRoot()->GetLocalPosition();
	pos += _direction * _speed * dt;
	_transformTree->GetRoot()->SetLocalPosition(pos);
}
