#include "pch.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "ImguiManager.h"
SphereCollider::SphereCollider():BaseCollider(ColliderType::Sphere)
{

}

SphereCollider::~SphereCollider()
{

	int a = 5;

}

void SphereCollider::Init()
{
	/*_orginsphere.Transform(_sphere, GetOwner()->_transform->GetMatrix());*/
}

void SphereCollider::Update()
{
	
}

void SphereCollider::Render()
{


}

bool SphereCollider::CheckCollusion(shared_ptr<BaseCollider>& other)
{
	if (other->GetColliderType() == ColliderType::Box)
	{
		shared_ptr<BoxCollider> otherbox = static_pointer_cast<BoxCollider>(other);

		if (_sphere.Intersects(otherbox->GetBox()))
		{
			return true;
		}
	}

	if (other->GetColliderType() == ColliderType::Sphere)
	{
		shared_ptr<SphereCollider> otherSphere = static_pointer_cast<SphereCollider>(other);

		if (_sphere.Intersects(otherSphere->GetSphere()))
		{
			return true;
		}
	}

	return false;

}

bool SphereCollider::CheckCollusion(Ray& ray, OUT float& distance)
{
	return _sphere.Intersects(ray.position, ray.direction, OUT distance);
}

void SphereCollider::MakeBoundingSphere()
{

	_sphere.Center = this->GetCenter();
	_sphere.Radius = this->GetRadius();

	_orginsphere.Center = this->GetCenter();
	_orginsphere.Radius = this->GetRadius();


	Update();

}
