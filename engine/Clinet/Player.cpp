#include "pch.h"
#include "Player.h"
#include "Model.h"
#include "Transform.h"
#include "LightManager.h"
#include "Helper.h"


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

	vector<shared_ptr<ModelBone>>& meshData = GetModel()->GetBones();
	auto& rootTransform = GetModel()->GetRoot()->transform;


	static float temp = 0.1f;

	for (auto& data : meshData)
	{
		if (data->name == L"Top_Rotor")
		{
			auto& pos = data->transform->GetLocalRotation();
			data->transform->SetLocalRotation(vec3(pos.x, pos.y + temp, pos.z));
		}
	}

	KeyUpdate(rootTransform, dt);
	GameObject::Update();
	UpdateLight();

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
	params.LightInfos[_lightIndex].position = this->GetModel()->GetRoot()->transform->GetLocalPosition();
	params.LightInfos[_lightIndex].direction = this->GetModel()->GetRoot()->transform->GetLook();
}
