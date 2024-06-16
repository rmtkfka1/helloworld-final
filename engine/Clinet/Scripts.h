#pragma once
#include "Component.h"


class Scripts :public Component
{
public:
	Scripts();
	virtual ~Scripts();

	virtual void Init();
	virtual void Update();
	virtual void Render();
};

