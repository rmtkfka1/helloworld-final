#pragma once


enum class SceneType
{
	NONE,
	STARTSCENE,
	STAGE1,
	STAGE2,

};
class Scene;

class SceneManger
{

public:

	static SceneManger* GetInstance()
	{

		static SceneManger instance;
		return &instance;
	}


public:
	void Init();
	void Update();
	void LateUpdate();
	void Render();
	void ClearScene();

public:

	void ChangeScene(SceneType type);
	shared_ptr<Scene> GetCurrentScene() { return _scene; }
	SceneType GetSceneType() { return _sceneType; }

private:

	shared_ptr<Scene> _scene;
	SceneType _sceneType = SceneType::NONE;

};

