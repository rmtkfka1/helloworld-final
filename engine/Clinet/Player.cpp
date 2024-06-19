#include "pch.h"
#include "Player.h"
#include "Model.h"
#include "Transform.h"
#include "LightManager.h"
#include "Helper.h"
#include "TransformTree.h"
#include "Bullet.h"
#include "BoxCollider.h"
#include "SceneManger.h"
#include "Scene.h"
#include "CollisonManager.h"
#include "CameraManager.h"

Player::Player():GameObject(GAMEOBJECT_TYPE::Player)
{
}

Player::~Player()
{
}

void Player::Init()
{
	GameObject::Init();
	AddLight();

}

void Player::Update()
{
	float dt = TimeManager::GetInstance()->GetDeltaTime();


	static float temp = 0.1f;
	{
		shared_ptr<Transform>& transform = _transformTree->findByName(L"Top_Rotor");
		auto& pos = transform->GetLocalRotation();
		transform->SetLocalRotation(vec3(pos.x, pos.y + temp, pos.z));
	}

	{
		shared_ptr<Transform>& transform = _transformTree->findByName(L"Back_Rotor");
		auto& pos = transform->GetLocalRotation();
		transform->SetLocalRotation(vec3(pos.x, pos.y+ temp, pos.z ));
	}


	KeyUpdate(_transformTree->GetRoot(), dt);
	UpdateLight();
	GameObject::Update();

}

void Player::KeyUpdate(std::shared_ptr<Transform>& rootTransform, float dt)
{
	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::W))
	{

		vec3 pos = rootTransform->GetLocalPosition();
		pos += rootTransform->GetLook() * _speed * dt;
		rootTransform->SetLocalPosition(pos);
	};

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::S))
	{

		vec3 pos = rootTransform->GetLocalPosition();
		pos -= rootTransform->GetLook() * _speed * dt;
		rootTransform->SetLocalPosition(pos);
	};

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::D))
	{

		vec3 pos = rootTransform->GetLocalPosition();
		pos += rootTransform->GetRight() * _speed * dt;
		rootTransform->SetLocalPosition(pos);
	};

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::A))
	{

		vec3 pos = rootTransform->GetLocalPosition();
		pos -= rootTransform->GetRight() * _speed * dt;
		rootTransform->SetLocalPosition(pos);
	};


	if (KeyManager::GetInstance()->GetButtonDown(KEY_TYPE::SPACE))
	{

		shared_ptr <Bullet> gameobject = make_shared<Bullet>();
		shared_ptr<Model> model = Model::ReadData(L"box/box");
		gameobject->SetModel(model);
		auto& pos = this->GetTransformTree()->GetRoot()->_position;


		gameobject->GetTransformTree()->GetRoot()->SetLocalPosition(pos);
		gameobject->GetTransformTree()->GetRoot()->SetLocalScale(vec3(0.05f, 0.05f, 0.05f));


		gameobject->Init();
		gameobject->AddComponent(make_shared<BoxCollider>());
		SceneManger::GetInstance()->GetCurrentScene()->ReserveAddGameObject(gameobject);

	};


	
}

void Player::Render()
{
	GameObject::Render();
}

void Player::AddLight()
{
	LightInfo info;
	info.color.ambient = vec4(0, 0, 0, 0);
	info.color.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	info.color.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	info.lightType = static_cast<int32>(LIGHT_TYPE::SPOT_LIGHT);
	info.attenuation = Helper::GetAttenuationCoeff(50000.0f);
	_lightIndex = LightManager::GetInstnace()->PushLight(info);

}

void Player::UpdateLight()
{
	auto& params = LightManager::GetInstnace()->_lightParmas;

	params.LightInfos[_lightIndex].position = _transformTree->GetRoot()->_position;
	params.LightInfos[_lightIndex].direction = _transformTree->GetRoot()->GetLook();
}

void Player::OnComponentBeginOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider> other)
{

	//if (other->GetOwner()->GetGameObjectType() == GAMEOBJECT_TYPE::EnemyBullet)
	//{
	//	SceneManger::GetInstance()->GetCurrentScene()->ReserveDeleteGameObject(other->GetOwner());
	//	CollisonManager::GetInstance()->RemoveCollider(other);
	//	CameraManager::GetInstance()->_animationflag = true;
	//}


}

void Player::OnComponentEndOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider> other)
{

}
