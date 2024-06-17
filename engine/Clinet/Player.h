#pragma once
#include "GameObject.h"
class Player : public GameObject
{

public:

	Player();
	virtual ~Player();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;



};

