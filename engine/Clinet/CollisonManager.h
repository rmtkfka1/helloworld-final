#pragma once

class BaseCollider;
class BoxCollider;

class CollisonManager
{
public:


	static CollisonManager* GetInstance()
	{
		static CollisonManager instance;
		return &instance;
	}


public:
	void Init();
	void Update();
	void Clear();


	void CheckObjectCollusion();
	void CheckRayCollusion();
	vec3 ToWorldPosition(const vec3& pos, const Matrix& W, const Matrix& V, const Matrix& P);


	void AddCollider(shared_ptr<BaseCollider> collider);
	void RemoveCollider(shared_ptr<BaseCollider> collider);

private:
	vector<shared_ptr<BaseCollider>> _colliders;

};

