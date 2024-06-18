#pragma once
class GameObject;

enum class COMPONENT_TYPE:uint8
{
	COLLIDER,
	SCRIPTS,
	MODEL,
	TRANSFORM,
	TRASNFORMTREE
};

class Component
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

	virtual void Init();
	virtual void Update();
	virtual void Render();

	COMPONENT_TYPE GetComponentType() { return _type; }
	void SetOwner(shared_ptr<GameObject> owner) { _owner = owner; }
	shared_ptr<GameObject> GetOwner() { return _owner.lock(); }

protected:
	COMPONENT_TYPE _type;
	weak_ptr<GameObject> _owner;
};
