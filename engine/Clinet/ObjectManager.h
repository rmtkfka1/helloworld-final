#pragma once

class Player;

class ObjectManager
{
public:

	static ObjectManager* GetInstance()
	{
		static ObjectManager instance;
		return &instance;
	}

public:

	Player* player;

};

