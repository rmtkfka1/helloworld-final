#pragma once
#include "GameObject.h"
class Player : public GameObject
{

public:

	Player();
	virtual ~Player();

	virtual void Init() override;
	virtual void Update() override;
	void KeyUpdate(std::shared_ptr<Transform>& rootTransform, float dt);
	virtual void Render() override;

	void AddLight();
	void UpdateLight();

	virtual void OnComponentBeginOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider>other);
	virtual void OnComponentEndOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider> other);

private:
	float _speed = 20000.0f;
	int _lightIndex;

};

