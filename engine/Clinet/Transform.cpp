#include "pch.h"
#include "Transform.h"
#include "Core.h"
#include "ConstantBuffer.h"
#include "CameraManager.h"
void Transform::Update()
{
	Matrix matScale = Matrix::CreateScale(_localScale);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);

	//// 센터를 고려한 평행이동 행렬 생성
	//Matrix matTranslationBack = Matrix::CreateTranslation(-_center);
	//Matrix matTranslationBack2 = Matrix::CreateTranslation(_center);

	// 변환 행렬을 순서대로 곱셈
	_matLocal =  matScale * matRotation  * matTranslation ;

	_matWorld = _matLocal; 

	shared_ptr<Transform> parent = GetParent().lock();

	if (parent != nullptr)
	{
		_matWorld *= parent->GetLocalToWorldMatrix();
	}

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


