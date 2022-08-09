#pragma once
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"

class Object
{
public:
	Object() {}
	void Initialize(Model& model, Transform& transform, Rigidbody& rigidbody, Collider& collider);
	virtual void Draw(Matrix4x4& viewProjectionMatrix) {};
	
	Model m_model;
	Transform m_transform;
	Rigidbody m_rigidbody;
	Collider m_collider;
	SphereCollider m_sphereCollider;
	CubeCollider m_cubeCollider;
	PlaneCollider m_planeCollider;
};