#pragma once
#include "GameObject.h"



class InstancingParticle :public GameObject
{

public:

	InstancingParticle();
	virtual ~InstancingParticle();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

private:
	vector<InstanceData> _data;
	shared_ptr<StructedBuffer> _buffer;
	const int InstanceCount = 50;

public:

	vec3 _color;
	vec3 _ownerPosition;
	array<vec3, 200> _direction;

	float _offset = 0.0f;
	float _speed = 1000.0f;
	float _elapsedTime = 0;
};

