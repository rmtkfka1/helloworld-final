#include "pch.h"
#include "Transform.h"
#include "Helper.h"
#include "CameraManager.h"
#include "Core.h"
#include "ConstantBuffer.h"

Transform::Transform():Component(COMPONENT_TYPE::TRANSFORM)
{

}

Transform::~Transform()
{

}


void Transform::Update()
{
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	Matrix matTranslationBack = Matrix::CreateTranslation(-_center);
	Matrix matTranslationBack2 = Matrix::CreateTranslation(_center);


	_matLocal = matTranslationBack * matScale * matRotation * _rotateToPlayer * matTranslation * matTranslationBack2;

	if (HasParent())
	{
		_matWorld = _matLocal * _parent.lock()->GetWorldMatrix();
	}
	else
	{
		_matWorld = _matLocal;
	}

	Quaternion quat;
	_matWorld.Decompose(_scale, quat, _position);
	_rotation = Helper::ToEulerAngles(quat);

	_right = vec3::TransformNormal(vec3::Right, _matWorld);
	_up = vec3::TransformNormal(vec3::Up, _matWorld);
	_look = vec3::TransformNormal(vec3::Backward, _matWorld);

	// Children
	for (const shared_ptr<Transform>& child : _children)
		child->Update();
}

void Transform::PushData()
{
	TransformParams transformParams = {};
	transformParams.matWorld = _matWorld;
	transformParams.matView = CameraManager::S_MatView;
	transformParams.matProjection = CameraManager::S_MatProjection;

	core->GetConstantBuffer(CBV_REGISTER::b1)->PushData(&transformParams, sizeof(transformParams));
}


void Transform::SetWorldScale(const vec3 worldScale)
{
	if (HasParent())
	{
		vec3 parentScale = _parent.lock()->GetWorldScale();
		vec3 scale = worldScale;
		scale.x /= parentScale.x;
		scale.y /= parentScale.y;
		scale.z /= parentScale.z;
		SetLocalScale(scale);
	}
	else
	{
		SetLocalScale(worldScale);
	}
}

void Transform::SetWorldRotation(const vec3 worldRotation)
{
	if (HasParent())
	{
		Matrix inverseMatrix = _parent.lock()->GetWorldMatrix().Invert();

		vec3 rotation;
		rotation.TransformNormal(worldRotation, inverseMatrix);

		SetLocalRotation(rotation);
	}
	else
		SetLocalRotation(worldRotation);
}

void Transform::SetWorldPosition(const vec3 worldPosition)
{
	if (HasParent())
	{
		Matrix worldToParentLocalMatrix = _parent.lock()->GetWorldMatrix().Invert();

		vec3 position;
		position.Transform(worldPosition, worldToParentLocalMatrix);

		SetLocalPosition(position);
	}
	else
	{
		SetLocalPosition(worldPosition);
	}
}
