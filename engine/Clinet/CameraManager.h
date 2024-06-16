#pragma once
class GameObject;

enum class PROJECTION_TYPE
{
	PERSPECTIVE,
	ORTHOGRAPHIC,
};

enum CameraType
{
	CameraType1,
	CameraType2,
	CameraType3,
	CameraType4,
};

class CameraManager
{

public:
	static CameraManager* GetInstance()
	{
		static CameraManager manager;
		return &manager;
	}

	void Clear();
	void Init();
	void Update();
	void MouseUpdate();
	void Animation();



public:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 0.1f;
	float _far = 3000.f;
	float _fov = XM_PI / 4.f;
	float _scale = 1.f;
	RECT _rect{};
	float _cameraYaw{};
	float _cameraPitch{};
	POINT _centerScreen{};
	POINT _mousePos{};

public:
	float _width{};
	float _height{};

private:

	const float _cameraSpeed = 10.0f;

public:
	Matrix _matView = {};
	Matrix _matProjection = {};
	static Matrix S_MatView;
	static Matrix S_MatProjection;

	bool flag = true;
public:
	vec3 _cameraPos{};
	vec3 _cameraLook{};
	vec3 _cameraUp{};

	vec3 _shake = { 0,0,0 };
	bool _animationflag = false;
	float _elaspedTime = 0;

	CameraType _cameraType= CameraType1;
};

