#pragma once

class Player;
class Enemy;

class ObjectManager
{
public:

	static ObjectManager* GetInstance()
	{
		static ObjectManager instance;
		return &instance;
	}

public:

	weak_ptr<Player> _player;
	weak_ptr<Enemy> _enemy;

};

