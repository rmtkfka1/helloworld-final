#pragma once
#include "GameObject.h"
class Bullet :public GameObject
{
public:


	Bullet();
	virtual ~Bullet();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void DeleteBullet();
	void PosUpdate();

	void SetFirstPos(vec3 pos) { _firstPos = pos; }

private:
	float _speed = 1000000.0f;
	vec3 _direction;
	vec3 _firstPos;


};

