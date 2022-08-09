#pragma once
#include "../Utils/SimpleMath.h"
#include "Collision.h"
#include <vector>

class Actor;
class PhysicsManager
{
private:
	const float m_epsilon = 0.00001f;
	Vector3 m_gravity = Vector3(0, -3.5f, 0);
	vector<Contact> m_contacts;

	vector<Actor*>* m_actors;
	float& m_deltaTime;
	vector<Vector3>* m_lineForDebug;

public:
	PhysicsManager(vector<Actor*>* actors, float& deltaTime, vector<Vector3>* lineForDebug);

	void Update();
	void GenerateGravity();
	void UpdateTransform();
	void DetectCollision();
	void ResolveCollision();
	void ResolvePenetration();
};