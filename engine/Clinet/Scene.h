#pragma once

class GameObject;

class Scene
{

public:
	Scene();
	virtual ~Scene();

	virtual void Init(); 
	virtual void Update();  //객체로직
	virtual void LateUpdate(); //객체 메모리 추가 삭제
	virtual void Render(); //렌더링
	virtual void ClearScene(); //씬정리 , 씬이동 , 씬에있는 메모리삭제

	vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }
	void ReserveAddGameObject(shared_ptr<GameObject> object);
	void ReserveDeleteGameObject(shared_ptr<GameObject> object);
	void AddGameObject(shared_ptr<GameObject> object);
	void DeleteGameObject(shared_ptr<GameObject> object);

protected:
	vector<shared_ptr<GameObject>> _gameObjects;
	queue<shared_ptr<GameObject>> _reserveAddQueue;
	queue<shared_ptr<GameObject>> _reserveDeleteQueue;
	
public:

	bool _sceneEnd = false;

};

