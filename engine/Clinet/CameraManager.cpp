#include "pch.h"
#include "CameraManager.h"
#include "Core.h"
#include "ConstantBuffer.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "ImguiManager.h"
#include "ObjectManager.h"
#include <random>
#include "Player.h"
#include "Model.h"
#include "TransformTree.h"

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

Matrix CameraManager::S_MatView;
Matrix CameraManager::S_MatProjection;

void CameraManager::Clear()
{
    // Clear function implementation (if needed)
}

void CameraManager::Init()
{
    _width = static_cast<float>(core->GetWindowInfo().width);
    _height = static_cast<float>(core->GetWindowInfo().height);
    SetCursor(LoadCursor(NULL, IDC_ARROW)); // �⺻ ȭ��ǥ Ŀ���� ����
    GetWindowRect(GetForegroundWindow(), &_rect);
    _centerScreen = { (_rect.right + _rect.left) / 2, (_rect.bottom + _rect.top) / 2 };

    _mousePos = _centerScreen;

    _player = ObjectManager::GetInstance()->_player.lock();
    auto& transform = _player.lock()->GetTransformTree()->GetRoot();
    transform->GetLook().Normalize();


    _cameraPos = transform->GetLocalPosition() + _offset;
    _cameraLook = vec3(0, 0, 1);
    _cameraUp = vec3(0, 1.0f, 0);
}

void CameraManager::Update()
{
    MouseUpdate();
    PlayerUpdate();
    Animation();
    CameraPosUpdate();
    CameraLookUpdate();
    RegenerateMatrix();

    // �������� ��� ������Ʈ
    if (_type == PROJECTION_TYPE::PERSPECTIVE)
        S_MatProjection = ::XMMatrixPerspectiveFovLH(_fov, _width / _height, _near, _far);
    else
        S_MatProjection = ::XMMatrixOrthographicLH(_width * _scale, _height * _scale, _near, _far);

    _matView = S_MatView;
    _matProjection = S_MatProjection;

    ImguiManager::GetInstance()->_temp = _cameraPos;
}

void CameraManager::MouseUpdate()
{
   

    // ���콺 ��ġ ������Ʈ
    _mousePos = KeyManager::GetInstance()->GetMousePos();

    // ȭ�� �߽ɰ��� ���� ���
    float deltaPosX = static_cast<float>(_mousePos.x - _centerScreen.x);
    float deltaPosY = static_cast<float>(_mousePos.y - _centerScreen.y);

    // �ð� ���� ���
    float dt = TimeManager::GetInstance()->GetDeltaTime();

    // Yaw�� Pitch ������Ʈ
    _cameraYaw += deltaPosX * _cameraSpeed * dt;
    _cameraPitch -= deltaPosY * _cameraSpeed * dt;

    // Pitch ����
    if (_cameraPitch > 89.0f) _cameraPitch = 89.0f;
    if (_cameraPitch < -89.0f) _cameraPitch = -89.0f;

    if (_cameraYaw < 0.0f) _cameraYaw += 360.0f;
    if (_cameraYaw > 360.0f) _cameraYaw -= 360.0f;

    // ���콺 Ŀ�� ��ġ �缳��
    SetCursorPos(static_cast<int>(_centerScreen.x), static_cast<int>(_centerScreen.y));
}

void CameraManager::Animation()
{
    float dt = TimeManager::GetInstance()->GetDeltaTime();;

    if (_elaspedTime > 1.5f)
    {
        _animationflag = false;
        _elaspedTime = 0;
        _shake = vec3(0, 0, 0);
    }

    if (_animationflag)
    {
        float intenisty = 1000.0f;
        _shake.x = distribution(generator) * intenisty * dt;
        _shake.y = distribution(generator) * intenisty * dt;

        _elaspedTime += dt;
    }
}

void CameraManager::PlayerUpdate()
{
    // ��ġ�� �丸ŭ �÷��̾ ȸ����Ų��.
    auto& playerTransform = _player.lock()->GetTransformTree()->GetRoot();
    auto pos = playerTransform->GetLocalRotation();
    playerTransform->SetLocalRotation(vec3(XMConvertToRadians(pos.x+_cameraPitch), XMConvertToRadians(pos.y+_cameraYaw+180.0f), 0));
}

void CameraManager::CameraPosUpdate()
{
    auto& playerTransform = _player.lock()->GetTransformTree()->GetRoot();

    Matrix mat = Matrix::Identity;

    _cameraRight = playerTransform->GetRight();
    _cameraUp = playerTransform->GetUp();
    _cameraLook = playerTransform->GetLook();

    mat._11 = _cameraRight.x; mat._21 = _cameraUp.x; mat._31 = _cameraLook.x;
    mat._12 = _cameraRight.y; mat._22 = _cameraUp.y; mat._32 = _cameraLook.y;
    mat._13 = _cameraRight.z; mat._23 = _cameraUp.z; mat._33 = _cameraLook.z;

    vec3 offset = vec3::Transform(_offset, mat);
    vec3 cameraPos = playerTransform->GetWorldPosition() + offset;
    vec3 direction = cameraPos - _cameraPos;

    float length = direction.Length();
    direction.Normalize();

    float distance = length * 30.0f * TimeManager::GetInstance()->GetDeltaTime();

    _cameraPos = _cameraPos + direction * distance;
}

void CameraManager::CameraLookUpdate()
{
    auto& playerTransform = _player.lock()->GetTransformTree()->GetRoot();

    Matrix mtxLookAt = XMMatrixLookAtLH(_cameraPos, playerTransform->GetWorldPosition(), playerTransform->GetUp());
    _cameraRight = vec3(mtxLookAt._11, mtxLookAt._21, mtxLookAt._31);
    _cameraUp = vec3(mtxLookAt._12, mtxLookAt._22, mtxLookAt._32);
    _cameraLook = vec3(mtxLookAt._13, mtxLookAt._23, mtxLookAt._33);
}

void CameraManager::RegenerateMatrix()
{
    _cameraLook.Normalize();
    _cameraRight.Cross(_cameraUp, _cameraLook);
    _cameraRight.Normalize();

    _cameraUp.Cross(_cameraLook, _cameraRight);
    _cameraUp.Normalize();

    S_MatView = XMMatrixLookToLH(_cameraPos+ _shake, _cameraLook, _cameraUp);

    /*S_MatView._11 = _cameraRight.x; S_MatView._12 = _cameraUp.x; S_MatView._13 = _cameraLook.x;
    S_MatView._21 = _cameraRight.y; S_MatView._22 = _cameraUp.y; S_MatView._23 = _cameraLook.y;
    S_MatView._31 = _cameraRight.z; S_MatView._32 = _cameraUp.z; S_MatView._33 = _cameraLook.z;
    S_MatView._41 = -_cameraPos.Dot(_cameraRight);
    S_MatView._42 = -_cameraPos.Dot(_cameraUp);
    S_MatView._43 = -_cameraPos.Dot(_cameraLook);*/
}