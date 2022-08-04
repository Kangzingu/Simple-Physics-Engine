#include "Actor.h"
#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(vector<Actor*>* actors, float& deltaTime, vector<Vector3>* lineForDebug) : actors(actors), deltaTime(deltaTime), lineForDebug(lineForDebug) {}

void PhysicsManager::Update()
{
	// 授辞 域紗 白哀鍵陥.. 原走厳拭 Transform 聖 郊蚊爽澗惟 雌縦旋生経 限澗暗 旭精汽 託戚研 設 乞牽畏陥
	GenerateGravity();
	UpdateTransform();

	contacts.clear();
	DetectCollision();
	ResolveCollision();
}

void PhysicsManager::GenerateGravity()
{
	for (int i = 0; i < (*actors).size(); i++)
	{
		(*actors)[i]->rigidbody.AddForce(gravity * (*actors)[i]->rigidbody.GetMass());
	}
}

void PhysicsManager::UpdateTransform()
{
	for (int i = 0; i < (*actors).size(); i++)
	{
		(*actors)[i]->rigidbody.Update(deltaTime);
	}
}

void PhysicsManager::DetectCollision()
{
	for (int i = 0; i < (*actors).size(); i++)
	{
		for (int j = i + 1; j < (*actors).size(); j++)
		{
			if (Collision::BroadPhaseBoundingSphere(*(*actors)[i], *(*actors)[j], lineForDebug))
				Collision::NarrowPhaseCubeAndCube((*actors)[i], (*actors)[j], contacts, lineForDebug);
		}
	}
}

void MakeOrthonormalBasis(const Vector3& x, Vector3* y, Vector3* z)
{
	*z = Vector3::Cross(x, *y);
	if (Vector3::SquareMagnitude(*z) == 0)
	{
		*y = Vector3(y->y, y->z, y->x);
		*z = Vector3::Cross(x, *y);
	}
	*y = Vector3::Cross(*z, x);

	*z = Vector3::Normalize(*z);
	*y = Vector3::Normalize(*y);
}
void PhysicsManager::ResolveCollision()
{
	for (int i = 0; i < contacts.size(); i++)
	{
		Vector3 x = contacts[i].normal;
		Vector3 y = Vector3::Up();
		Vector3 z;

		MakeOrthonormalBasis(x, &y, &z);
		Matrix4x4 contactToWorldCoordMatrix = Matrix4x4(x, y, z);
		// View 痕発 獣 朝五虞 奄層生稽 馬奄 是背 朝五虞税 transform税 蝕楳慶聖 弘端級拭惟 咽背爽牛, contact 奄層生稽 馬形檎 contact税 transform拭 背雁馬澗 basis楳慶税 蝕楳慶聖 咽背操醤 敗
		//Matrix4x4 toContactCoordMatrix = contactCoordBasisMatrix.Inverse();
		
		/* 食奄採斗 毘 域至 */
		Vector3 relativeContactPosition = contacts[i].point - contacts[i].object1->transform.GetPosition();// 戚暗 薦企稽 段奄鉢 背操醤敗 弘端 1戚 焼観 呪 赤製
		Vector3 torquePerUnitImpulse = Vector3::Cross(relativeContactPosition, contacts[i].normal);
		Vector3 rotationPerUnitImpulse = contacts[i].object1->rigidbody.GetInertiaTensor().Inverse() * torquePerUnitImpulse;

		// 中宜 走繊税 唖紗亀研 姥廃暗績(杉球 疎妊域 奄層)
		Vector3 velocityPerUnitImpulse = Vector3::Cross(rotationPerUnitImpulse, relativeContactPosition);

		// 酔軒澗 Contact 疎妊域 奄層 唖紗亀亜 琶推馬艦 Contact 疎妊域稽 痕発背 捜
		Vector3 velocityPerUnitImpulseContact = contactToWorldCoordMatrix.Transpose() * velocityPerUnitImpulse;

		// 酔軒亜 Contact Normal聖 x逐生稽 背辞 Basis研 姥梅生艦 衣厩 酔軒亜 据馬澗 Contact Normal 号狽 唖紗径精 x逐 葵戚 吃暗績 
		float angularComponent = velocityPerUnitImpulseContact.x;


		/* 食奄澗 恥 杯勲 域至..? 317pg 凧壱 */
		Vector3 relativeContactPosition1 = contacts[i].point - contacts[i].object1->transform.GetPosition();
		Vector3 deltaVelWorld = Vector3::Cross(relativeContactPosition1, contacts[i].normal);
		deltaVelWorld = contacts[i].object1->transform.GetRotationMatrix()* contacts[i].object1->rigidbody.GetInertiaTensor().Inverse() * contacts[i].object1->transform.GetRotationMatrix().Inverse() * deltaVelWorld;
		deltaVelWorld = Vector3::Cross(deltaVelWorld, relativeContactPosition1);

		float deltaVelocity = Vector3::Dot(deltaVelWorld, contacts[i].normal);

		deltaVelocity += (1.0f / contacts[i].object1->rigidbody.GetMass());

		if (contacts[i].object2 != nullptr)
		{
			Vector3 relativeContactPosition2 = contacts[i].point - contacts[i].object2->transform.GetPosition();
			deltaVelWorld = Vector3::Cross(relativeContactPosition2, contacts[i].normal);
			deltaVelWorld = contacts[i].object2->transform.GetRotationMatrix() * contacts[i].object2->rigidbody.GetInertiaTensor().Inverse() * contacts[i].object2->transform.GetRotationMatrix().Inverse() * deltaVelWorld;
			deltaVelWorld = Vector3::Cross(deltaVelWorld, relativeContactPosition2);

			deltaVelocity += Vector3::Dot(deltaVelWorld, contacts[i].normal);

			deltaVelocity += (1.0f / contacts[i].object2->rigidbody.GetMass());
		}


		/* 食奄澗 据馬澗 噺穿勲 域至..? 318pg 凧壱 */
		relativeContactPosition1 = contacts[i].point - contacts[i].object1->transform.GetPosition();

		Vector3 closingVelocity1 = Vector3::Cross(contacts[i].object1->rigidbody.GetAngularVelocity(), relativeContactPosition1);
		closingVelocity1 += contacts[i].object1->rigidbody.GetVelocity();
		if (contacts[i].object2 != nullptr)
		{
			Vector3 relativeContactPosition2 = contacts[i].point - contacts[i].object2->transform.GetPosition();

			Vector3 closingVelocity2 = Vector3::Cross(contacts[i].object2->rigidbody.GetAngularVelocity(), relativeContactPosition2);
			closingVelocity2 += contacts[i].object2->rigidbody.GetVelocity();

			closingVelocity1 += closingVelocity2;
		}
		Vector3 totalClosingVelocity = contactToWorldCoordMatrix.Transpose() * closingVelocity1;
		float restitution = 1.0f;
		float desiredVelocity = -totalClosingVelocity.x * (1 + restitution);

		Vector3 impulseContact = Vector3(desiredVelocity / deltaVelocity, 0, 0);
		Vector3 impulse = contactToWorldCoordMatrix * impulseContact;

		impulse *= -1;
		Vector3 velocityChange = impulse / contacts[i].object1->rigidbody.GetMass();
		Vector3 impulsiveTorque = Vector3::Cross(impulse, relativeContactPosition1);
		Vector3 rotationChange = contacts[i].object1->transform.GetRotationMatrix()* contacts[i].object1->rigidbody.GetInertiaTensor().Inverse()* contacts[i].object1->transform.GetRotationMatrix().Inverse()* impulsiveTorque;

		contacts[i].object1->rigidbody.AddVelocity(velocityChange * deltaTime);
		contacts[i].object1->rigidbody.AddAngularVelocity(rotationChange * deltaTime);
		if (contacts[i].object2 != nullptr)
		{
			Vector3 relativeContactPosition2 = contacts[i].point - contacts[i].object2->transform.GetPosition();

			impulse *= -1;
			velocityChange = impulse / contacts[i].object2->rigidbody.GetMass();
			impulsiveTorque = Vector3::Cross(impulse, relativeContactPosition2);
			rotationChange = contacts[i].object2->transform.GetRotationMatrix() * contacts[i].object2->rigidbody.GetInertiaTensor().Inverse() * contacts[i].object2->transform.GetRotationMatrix().Inverse() * impulsiveTorque;

			contacts[i].object2->rigidbody.AddVelocity(velocityChange * deltaTime);
			contacts[i].object2->rigidbody.AddAngularVelocity(rotationChange * deltaTime);
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

		totalInverseMass = 1.0f / contacts[i].object1->rigidbody.GetMass() + 1.0f / contacts[i].object2->rigidbody.GetMass();
		// 霜勲拭 搾景背辞 恭嬰兜澗闇 呪縦 政亀研 公馬畏製.. 焼たたたたたたたたた
		Vector3 movePerIMass = contacts[i].normal * (contacts[i].depth / totalInverseMass);
		contacts[i].object1->transform.Translate(movePerIMass / contacts[i].object1->rigidbody.GetMass());
		contacts[i].object2->transform.Translate(movePerIMass / -contacts[i].object2->rigidbody.GetMass());

	}
}