#pragma once

#include "Component.h"
class Transform;

class TransformTree:public Component
{
public:
	TransformTree();
	virtual~TransformTree();

public:
	void MakeTransformTree();
public:
	shared_ptr<Transform>& findByName(wstring name);

public:
	shared_ptr<Transform>& GetRoot() { return _root; }

private:
	shared_ptr<Transform> _root;
	unordered_map<wstring, shared_ptr<Transform>> _nameMap;

};

