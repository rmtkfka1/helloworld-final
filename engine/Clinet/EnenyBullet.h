#pragma once
#include "GameObject.h"
class EnenyBullet :public GameObject
{
public:

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void DeleteBullet();
	void PosUpdate();

private:
	vec3 _direction;
	float _distance{};
	float _speed = 3000.0f;

};

