#pragma once
#include "GameObject.h"

class Mesh;
class Material;

class Terrain :public GameObject
{


public:

	Terrain();
	virtual ~Terrain();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;

	void AddMesh(shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void AddMaterial(shared_ptr<Material> mateiral) { _material = mateiral; }

private:
	shared_ptr<Mesh> _mesh;
	shared_ptr<Material> _material;

};

