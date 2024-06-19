#pragma once
#include "GameObject.h"
class Enemy :public GameObject
{

public:
	Enemy();
	virtual ~Enemy();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void UpdatePos();
	void UpdateToLookAtPlayer();
	void Fire();


	virtual void OnComponentBeginOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider>other);
	virtual void OnComponentEndOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider> other);


private:
	float elapsedTime = 0.0f;
	float _rotationSpeed = 1.0f;
	float _speed = 1000.0f;

	bool movingForward = false;
	float distance = 0;


};

