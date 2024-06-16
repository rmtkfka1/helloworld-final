#pragma once
#include "GameObject.h"

class Box:public GameObject
{

public:

	Box();
	virtual ~Box();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;


};

