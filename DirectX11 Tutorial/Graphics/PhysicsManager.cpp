#include "Actor.h"
#include "PhysicsManager.h"

void PhysicsManager::Update(vector<Actor*> actors, float deltaTime, vector<pair<Vector3, Vector3>>& aabb)
{
	for (int i = 0; i < actors.size(); i++)
	{
		if (isGravityOn)
			actors[i]->rigidbody.AddForce(gravity * actors[i]->rigidbody.GetMass());
		actors[i]->rigidbody.Update(deltaTime);
	}
	// 中宜戚 赤醸陥壱 帖壱 背衣
	vector<Contact> contacts;
	for (int i = 0; i < actors.size(); i++)
	{
		for (int j = i + 1; j < actors.size(); j++)
		{
			if(Collision::BroadPhaseBoundingSphere(*actors[i], *actors[j], aabb))
				Collision::CubeAndCube(actors[i], actors[j], contacts);
		}
	}

	// 鋼降域呪(析舘 績税稽 走舛敗)
	float restitution = 0.3f;
	for (int i = 0; i < contacts.size(); i++)
	{
		// 1. 紗亀 痕井

		float separatingVelocity;
		Vector3 relativeVelocity = contacts[i].object1->rigidbody.GetVelocity() - contacts[i].object2->rigidbody.GetVelocity();
		separatingVelocity = Vector3::Dot(relativeVelocity, contacts[i].normal);

		// 菰嬢走壱 赤澗暗虞檎
		if (separatingVelocity > 0)
			continue;

		float newSepVelocity = -separatingVelocity * restitution;

		Vector3 accCausedVelocity = contacts[i].object1->rigidbody.GetAccumulatedForce() / contacts[i].object1->rigidbody.GetMass() - contacts[i].object2->rigidbody.GetAccumulatedForce() / contacts[i].object2->rigidbody.GetMass();
		float accCausedSepVelocity = Vector3::Dot(accCausedVelocity, contacts[i].normal) * deltaTime;
		if (accCausedSepVelocity < 0)
		{
			// ? 紗亀研 製呪艦猿 皐醤走 希背走澗暗 焼観悪..
			newSepVelocity += restitution * accCausedSepVelocity;
			if (newSepVelocity < 0) newSepVelocity = 0;
		}
		float deltaVelocity = newSepVelocity - separatingVelocity;
		float totalInverseMass = 1.0f / contacts[i].object1->rigidbody.GetMass() + 1.0f / contacts[i].object2->rigidbody.GetMass();

		if (totalInverseMass <= 0)
			continue;

		float impulse = deltaVelocity / totalInverseMass;
		Vector3 impulsePerIMess = contacts[i].normal * impulse;
		contacts[i].object1->rigidbody.AddVelocity(impulsePerIMess / contacts[i].object1->rigidbody.GetMass());
		contacts[i].object2->rigidbody.AddVelocity(impulsePerIMess / -contacts[i].object2->rigidbody.GetMass());

		// 2. 違庁 是帖 繕舛
		if (contacts[i].depth <= 0)
			continue;

		// 霜勲拭 搾景背辞 恭嬰兜澗闇 呪縦 政亀研 公馬畏製.. 焼たたたたたたたたた
		Vector3 movePerIMass = contacts[i].normal * (contacts[i].depth / totalInverseMass);
		contacts[i].object1->transform.Translate(movePerIMass / contacts[i].object1->rigidbody.GetMass());
		contacts[i].object2->transform.Translate(movePerIMass / -contacts[i].object2->rigidbody.GetMass());
	}
}

void PhysicsManager::SetGravityOn(bool isGravityOn)
{
	this->isGravityOn = isGravityOn;
}
