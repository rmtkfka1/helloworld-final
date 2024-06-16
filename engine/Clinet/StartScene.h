#pragma once
#include "Scene.h"
class StartScene :public Scene
{

public:

	StartScene();
	virtual ~StartScene();
	virtual void Init() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void ClearScene() override;
};

