#pragma once

class Mesh;
class Texture;
class Shader;
class Transform;
class Model;
class Material;
class Component;
class BaseCollider;
class StructedBuffer;

enum class GAMEOBJECT_TYPE
{
	None,
	Player,
	Enemy,
	Bullet,
	Carrier,
	Box,
	PlayerShield,
	Particle,
	EnemyBullet
};

class GameObject :public enable_shared_from_this<GameObject>
{

public:
	GameObject();
	GameObject(GAMEOBJECT_TYPE type);
	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Render(uint32 instance,  shared_ptr<StructedBuffer> buffer);

	void SetTransform(shared_ptr<Transform> transform);
	void SetMaterial(shared_ptr<Material> material) { _material = material; }
	void SetModel(shared_ptr<Model> model);
	void AddComponent(shared_ptr<Component> component);

	GAMEOBJECT_TYPE GetGameObjectType() { return _type; }

	virtual void OnComponentBeginOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider>other);
	virtual void OnComponentEndOverlap(shared_ptr<BaseCollider> collider, shared_ptr<BaseCollider> other);

public:

	GAMEOBJECT_TYPE _type =GAMEOBJECT_TYPE::None;

	vector<shared_ptr<Component>> _component;

	shared_ptr<Transform> _transform;
	shared_ptr<Material> _material;
	shared_ptr<Model> _model;
	shared_ptr<BaseCollider> _collider;

	vec3 _totalCenter{};
	vec3 _totalSize{};
	float _totalRadius{};


};

