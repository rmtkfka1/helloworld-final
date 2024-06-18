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

	void UpdateToLookAtPlayer();
private:

};

