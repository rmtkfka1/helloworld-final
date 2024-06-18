#include "pch.h"
#include "Enemy.h"
#include "Transform.h"
#include "Model.h"
#include "Player.h"
#include "ObjectManager.h"
#include "TransformTree.h"
#include "EnenyBullet.h"
#include "BoxCollider.h"
#include "SceneManger.h"
#include "Scene.h"
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
 

    UpdatePos();

    UpdateToLookAtPlayer();

    Fire();

    GameObject::Update();
}

void Enemy::Render()
{
	GameObject::Render();


}

void Enemy::UpdatePos()
{
  
    float dt = TimeManager::GetInstance()->GetDeltaTime();
    vec3 pos = _transformTree->GetRoot()->GetLocalPosition();


    if (movingForward) 
    {
        pos += _transformTree->GetRoot()->GetLook() * _speed * dt;
        distance += 100.0f * dt;
    }
    else
    {
        pos -= _transformTree->GetRoot()->GetLook() * _speed * dt;
        distance += 100.0f * dt;
    }

    if (distance > 800.0f)
    {
        distance = 0;
        movingForward = !movingForward;
    }

    _transformTree->GetRoot()->SetLocalPosition(vec3(pos.x, pos.y, pos.z));
}

void Enemy::UpdateToLookAtPlayer()
{
    std::shared_ptr<Player> player = ObjectManager::GetInstance()->_player.lock();
    if (!player) return;

    auto& playerPos = player->GetTransformTree()->GetRoot()->GetLocalPosition();
    auto& enemyPos = _transformTree->GetRoot()->GetLocalPosition();

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


    static float temp = 1.0f;
    _transformTree->findByName(L"turret_geo")->SetRotateToPlayerMat(rotationMatrix);
    {
        vec3 pos = _transformTree->findByName(L"r_front_wheel_geo")->GetLocalRotation();
        _transformTree->findByName(L"r_front_wheel_geo")->SetLocalRotation(vec3(pos.x+temp, pos.y, pos.z));
    }

    {
        vec3 pos = _transformTree->findByName(L"l_back_wheel_geo")->GetLocalRotation();
        _transformTree->findByName(L"l_back_wheel_geo")->SetLocalRotation(vec3(pos.x + temp, pos.y, pos.z));
    }

    {
        vec3 pos = _transformTree->findByName(L"r_back_wheel_geo")->GetLocalRotation();
        _transformTree->findByName(L"r_back_wheel_geo")->SetLocalRotation(vec3(pos.x + temp, pos.y, pos.z));
    }

    {
        vec3 pos = _transformTree->findByName(L"l_front_wheel_geo")->GetLocalRotation();
        _transformTree->findByName(L"l_front_wheel_geo")->SetLocalRotation(vec3(pos.x + temp, pos.y, pos.z));
    }

    temp += 0.1f;
}

void Enemy::Fire()
{
    float dt = TimeManager::GetInstance()->GetDeltaTime();
    {
        //ÃÑ¾Ë»ý¼º
        elapsedTime += dt;

        if (elapsedTime >= 2.0f)
        {
            elapsedTime = 0.0f;

            shared_ptr <EnenyBullet> gameobject = make_shared<EnenyBullet>();
            shared_ptr<Model> model = Model::ReadData(L"Box/Box");
            gameobject->SetModel(model);


  
            vec3 pos = GetTransformTree()->findByName(L"canon_geo")->_position;

            gameobject->GetTransformTree()->GetRoot()->SetLocalPosition(pos);
            gameobject->GetTransformTree()->GetRoot()->SetLocalScale(vec3(0.5f, 0.5f, 0.5f));
            gameobject->AddComponent(make_shared<BoxCollider>());
            gameobject->Init();
            SceneManger::GetInstance()->GetCurrentScene()->ReserveAddGameObject(gameobject);

        }
    }
}
