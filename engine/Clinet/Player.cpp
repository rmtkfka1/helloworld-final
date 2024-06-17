#include "pch.h"
#include "Player.h"
#include "Model.h"
#include "Transform.h"

Player::Player():GameObject(GAMEOBJECT_TYPE::Player)
{
}

Player::~Player()
{
}

void Player::Init()
{
	GameObject::Init();
}

void Player::Update()
{
	vector<shared_ptr<ModelBone>>& meshData = _model->GetBones();
	auto& rootTransform = _model->GetRoot()->transform;

	rootTransform->SetLocalScale(vec3(0.3f, 0.3f, 0.3f));

	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::ONE))
	{
		static float temp = 0.001f;
		temp += 0.000001f;

		auto& pos = rootTransform->GetLocalRotation();
		rootTransform->SetLocalRotation(vec3(pos.x, pos.y + temp, pos.z));
	};


	static float temp = 0.1f;

	for (auto& data : meshData)
	{
		if (data->name == L"Top_Rotor")
		{
			auto& pos = data->transform->GetLocalRotation();
			data->transform->SetLocalRotation(vec3(pos.x, pos.y + temp, pos.z));
		}
	}


	if (KeyManager::GetInstance()->GetButton(KEY_TYPE::RIGHT))
	{

		float _offset = 10000.0f;
		float DELTA_TIME = TimeManager::GetInstance()->GetDeltaTime();
		vec3 pos = rootTransform->GetLocalPosition();
		pos += rootTransform->GetLook() * _offset * DELTA_TIME;
		rootTransform->SetLocalPosition(pos);
	};

	GameObject::Update();
}

void Player::Render()
{
	GameObject::Render();
}
