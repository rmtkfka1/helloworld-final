#include "pch.h"
#include "Box.h"

Box::Box():GameObject(GAMEOBJECT_TYPE::Box)
{

}

Box::~Box()
{


}

void Box::Init()
{
	GameObject::Init();
}

void Box::Update()
{
	GameObject::Update();
}

void Box::Render()
{
	GameObject::Render();
}
