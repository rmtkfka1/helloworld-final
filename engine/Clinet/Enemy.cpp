#include "pch.h"
#include "Enemy.h"
#include "Transform.h"
#include "Model.h"
#include "Player.h"
#include "ObjectManager.h"
Enemy::Enemy():GameObject(GAMEOBJECT_TYPE::Enemy)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
}

void Enemy::Update()
{
    UpdateToLookAtPlayer();

    GameObject::Update();
}

void Enemy::Render()
{
	GameObject::Render();
}

void Enemy::UpdateToLookAtPlayer()
{
    std::shared_ptr<Player> player = ObjectManager::GetInstance()->_player.lock();
    if (!player) return;

    auto& playerPos = player->GetModel()->GetRoot()->transform->GetLocalPosition();
    auto& enemyPos = this->GetModel()->GetRoot()->transform->GetLocalPosition();

    vec3 direction = enemyPos - playerPos;
    direction.Normalize();

    vec3 defaultUp = vec3(0.0f, 1.0f, 0.0f);

    vec3 right = defaultUp.Cross(direction);
    right.Normalize();

    vec3 up = direction.Cross(right);

    Matrix rotationMatrix = Matrix(
        right.x, right.y, right.z, 0.0f,
        up.x, up.y, up.z, 0.0f,
        direction.x, direction.y, direction.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    this->GetModel()->GetRoot()->transform->SetRotateToPlayerMat(rotationMatrix);
}
