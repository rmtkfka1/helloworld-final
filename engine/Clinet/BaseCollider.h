#pragma once
#include "Component.h"
#include <variant>

enum class ColliderType
{
	Box,
	Sphere,
};

class BaseCollider : public Component
{
public:
	BaseCollider(ColliderType colliderType);
	virtual ~BaseCollider();

public:

	virtual void Init() =0;
	virtual void Update() =0;
	virtual void Render() =0;
	virtual bool CheckCollusion(shared_ptr<BaseCollider>& other) =0;
	virtual bool CheckCollusion(Ray& ray, OUT float& distance) = 0;	//위치,나가는방향

public:
	ColliderType GetColliderType() { return _colliderType; }
	void SetTotalCenter(vec3 center) { _totalCenter = center; }
	vec3 GetCenter() { return _totalCenter; }


protected:
	vec3 _totalCenter;
	ColliderType _colliderType = {};

public:
	unordered_set<BaseCollider*> _collisionMap;
};

