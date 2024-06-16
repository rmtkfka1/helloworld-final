#include "pch.h"
#include "CameraManager.h"
#include "Core.h"
#include "ConstantBuffer.h"
#include "Scene.h"
#include "SceneManger.h"
#include "GameObject.h"
#include "Transform.h"
#include "ImguiManager.h"
#include "ObjectManager.h"
#include <random>

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
    SetCursor(LoadCursor(NULL, IDC_ARROW)); // 기본 화살표 커서로 변경
    GetWindowRect(GetForegroundWindow(), &_rect);
    _centerScreen = { (_rect.right + _rect.left) / 2, (_rect.bottom + _rect.top) / 2 };

    _mousePos = _centerScreen;
    _cameraLook = vec3(0, 1.0f, 1.0f);
    _cameraUp = vec3(0, 1.0f, 0);
}

void CameraManager::Update()
{


    MouseUpdate();

    // 회전 행렬 생성 및 카메라 방향 벡터 업데이트
    Matrix rotationMatrix = Matrix::CreateRotationX(XMConvertToRadians(_cameraPitch));
    rotationMatrix *= Matrix::CreateRotationY(XMConvertToRadians(_cameraYaw));
    _cameraLook = vec3::TransformNormal(vec3(0, 0, 1.0f), rotationMatrix);

    // 이동 처리
    float speed = 100.0f * TimeManager::GetInstance()->GetDeltaTime();

    if (KeyManager::GetInstance()->GetButton(KEY_TYPE::W))
    {
        _cameraPos += _cameraLook * speed;
    }
    if (KeyManager::GetInstance()->GetButton(KEY_TYPE::S))
    {
        _cameraPos -= _cameraLook * speed;
    }

    vec3 _cameraRight = -_cameraLook.Cross(_cameraUp);

    if (KeyManager::GetInstance()->GetButton(KEY_TYPE::D))
    {
        _cameraPos += _cameraRight * speed;
    }
    if (KeyManager::GetInstance()->GetButton(KEY_TYPE::A))
    {
        _cameraPos -= _cameraRight * speed;
    }


    // 뷰 행렬 업데이트
    S_MatView = DirectX::XMMatrixLookToLH(_cameraPos, _cameraLook, _cameraUp);

    // 프로젝션 행렬 업데이트
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
    // 마우스 위치 업데이트
    _mousePos = KeyManager::GetInstance()->GetMousePos();

    // 화면 중심과의 차이 계산
    float deltaPosX = static_cast<float>(_mousePos.x - _centerScreen.x);
    float deltaPosY = static_cast<float>(_mousePos.y - _centerScreen.y);

    // 시간 간격 얻기
    float dt = TimeManager::GetInstance()->GetDeltaTime();

    // Yaw와 Pitch 업데이트
    _cameraYaw += deltaPosX * _cameraSpeed * dt;
    _cameraPitch += deltaPosY * _cameraSpeed * dt;

    // Pitch 제한
    if (_cameraPitch > 89.0f) _cameraPitch = 89.0f;
    if (_cameraPitch < -89.0f) _cameraPitch = -89.0f;

    if (_cameraYaw < 0.0f) _cameraYaw += 360.0f;
    if (_cameraYaw > 360.0f) _cameraYaw -= 360.0f;

    // 마우스 커서 위치 재설정
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

