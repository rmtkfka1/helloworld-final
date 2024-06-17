#pragma once
class GameObject;
class Player;

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
	void PlayerUpdate();
	void CameraPosUpdate();
	void CameraLookUpdate();
	void RegenerateMatrix();


public:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float _near = 0.1f;
	float _far = 300000.f;
	float _fov = XM_PI / 4.f;
	float _scale = 1.f;

	RECT _rect{};
	POINT _centerScreen{};
	POINT _mousePos{};

public:
	float _width{};
	float _height{};

public:
	Matrix _matView = {}; //m_xmf4x4View
	Matrix _matProjection = {}; //m_xmf4x4Projection
	static Matrix S_MatView;
	static Matrix S_MatProjection;

	bool flag = true;
public:

	float _cameraSpeed = 10.0f;
	float _cameraDistance{};

	vec3 _cameraPos{};
	vec3 _cameraRight{};
	vec3 _cameraLook{};
	vec3 _cameraUp{};
	vec3 _offset = vec3(0,500.0f, -4000.0f);

	float _cameraYaw{};
	float _cameraPitch{};


	vec3 _shake = { 0,0,0 };
	bool _animationflag = false;
	float _elaspedTime = 0;



	weak_ptr<Player> _player;

	CameraType _cameraType= CameraType1;

};

