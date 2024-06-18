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

private:
	float elapsedTime = 0.0f;
	float _rotationSpeed = 1.0f;
	float _speed = 1000.0f;

	bool movingForward = false;
	float distance = 0;
};

