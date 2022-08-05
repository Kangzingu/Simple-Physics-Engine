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
	// 1. 層搾
	Vector3 contactTangent[2];
	Matrix4x4 contactToWorld; 
	Vector3 relativeContactPosition[2];
	Vector3 contactVelocity;
	Vector3 velocity;
	Vector3 contactVelocity;
	Vector3 accVelocity;
	float velocityFromAcc;
	float thisRestitution;
	float desiredDeltaVelocity;
	for (int i = 0; i < contacts.size(); i++)
	{
		// Contact Basis 域至
		if (abs(contacts[i].normal.x) > abs(contacts[i].normal.y))
		{
			float s = 1.0f / sqrt(contacts[i].normal.z * contacts[i].normal.z + contacts[i].normal.x * contacts[i].normal.x);

			contactTangent[0].x = contacts[i].normal.z * s;
			contactTangent[0].y = 0;
			contactTangent[0].z = -contacts[i].normal.x * s;

			contactTangent[1].x = contacts[i].normal.y * contactTangent[0].x;
			contactTangent[1].y = contacts[i].normal.z * contactTangent[0].x - contacts[i].normal.x * contactTangent[0].z;
			contactTangent[1].z = -contacts[i].normal.y * contactTangent[0].x;
		}
		else
		{
			float s = 1.0f / sqrt(contacts[i].normal.z * contacts[i].normal.z + contacts[i].normal.y * contacts[i].normal.y);

			contactTangent[0].x = 0;
			contactTangent[0].y = -contacts[i].normal.z * s;
			contactTangent[0].z = contacts[i].normal.y * s;

			contactTangent[1].x = contacts[i].normal.y * contactTangent[0].z - contacts[i].normal.z * contactTangent[0].y;
			contactTangent[1].y = -contacts[i].normal.x * contactTangent[0].z;
			contactTangent[1].z = contacts[i].normal.x * contactTangent[0].y;
		}

		contactToWorld = Matrix4x4(contacts[i].normal, contactTangent[0], contactTangent[1]);

		relativeContactPosition[0] = contacts[i].point - contacts[i].object1->transform.GetPosition();
		if (contacts[i].object2 != nullptr)
		{
			relativeContactPosition[1] = contacts[i].point - contacts[i].object2->transform.GetPosition();
		}

		// Local Velocity 域至
		velocity = Vector3::Cross(contacts[i].object1->rigidbody.GetAngularVelocity(), relativeContactPosition[0]);
		velocity += contacts[i].object1->rigidbody.GetVelocity();

		 contactVelocity = contactToWorld.Transpose() * velocity;
		accVelocity = (contacts[i].object1->rigidbody.GetAccumulatedForce() / contacts[i].object1->rigidbody.GetMass()) * deltaTime;
		accVelocity = contactToWorld.Transpose() * accVelocity;
		accVelocity.x = 0;
		contactVelocity += accVelocity;
		if (contacts[i].object2 != nullptr)
		{
			velocity = Vector3::Cross(contacts[i].object2->rigidbody.GetAngularVelocity(), relativeContactPosition[1]);
			velocity += contacts[i].object1->rigidbody.GetVelocity();

			contactVelocity -= contactToWorld.Transpose() * velocity;
			accVelocity = (contacts[i].object2->rigidbody.GetAccumulatedForce() / contacts[i].object2->rigidbody.GetMass()) * deltaTime;
			accVelocity = contactToWorld.Transpose() * accVelocity;
			accVelocity.x = 0;
			contactVelocity -= accVelocity;
		}

		// 琶推廃 Delta Velocity 域至
		const static float velocityLimit = 0.25f;
		velocityFromAcc = 0;
		if (contacts[i].object1->rigidbody.IsKinematic() == false)// 食奄辞 溌昔馬澗惟 巷充 税耕亜 赤畏劃幻..
		{
			velocityFromAcc += Vector3::Dot((contacts[i].object1->rigidbody.GetAccumulatedForce() / contacts[i].object1->rigidbody.GetMass()) * deltaTime, contacts[i].normal);
		}
		
		if (contacts[i].object2 != nullptr && contacts[i].object2->rigidbody.IsKinematic() == false)
		{
			velocityFromAcc -= Vector3::Dot((contacts[i].object2->rigidbody.GetAccumulatedForce() / contacts[i].object2->rigidbody.GetMass()) * deltaTime, contacts[i].normal);
		}
		
		thisRestitution = 0.4f;
		if (abs(contactVelocity.x) < velocityLimit)
		{
			thisRestitution = 0.0f;
		}

		desiredDeltaVelocity = -contactVelocity.x - thisRestitution * (contactVelocity.x - velocityFromAcc);
	}

	// 2. 違庁 是帖 繕舛
	int iter, index;
	Vector3 linearChange[2], angularChange[2];
	float max;
	float positionEpsilon = 0.01f;// 格巷 拙精 contact depth税 災照舛失聖 背社馬奄 是廃 葵..?
	Vector3 deltaPosition;

	int positionIterationsUsed = 0;
	int positionIterations = contacts.size() * 2;// 訊 2壕昔走澗..
	while (positionIterationsUsed < positionIterations)
	{
		// 亜舌 宿唖廃(depth亜 燕精)杏 達製
		max = positionEpsilon;
		index = contacts.size();
		for (int i = 0; i < contacts.size(); i++)
		{
			if (contacts[i].depth > max)
			{
				max = contacts[i].depth;
				index = i;
			}
		}
		if (index == contacts.size())
			break;

		// 据掘 c[index].matchAwakeState();虞澗 搾醗失鉢吉 軒走球郊巨 凹酔澗牛廃?暗 敗

		// 匂走芝 痕井 拝暗績
		const float angularLimit = 0.2f;
		float angularMove[2];
		float linearMove[2];

		float totalInertia = 0;
		float linearInertia[2];
		float angularInertia[2];
		
		// 析舘 contact 疎妊域税 淫失 度辞 域至岨 馬壱
		Matrix4x4 inverseInertiaTensor = contacts[index].object1->transform.GetWorldMatrix() * contacts[index].object1->rigidbody.GetInertiaTensor();

		Vector3 angularInertiaWorld = Vector3::Cross(relativeContactPosition[0], contacts[index].normal);
		angularInertiaWorld = inverseInertiaTensor * angularInertiaWorld;
		angularInertiaWorld = Vector3::Cross(angularInertiaWorld, relativeContactPosition[0]);
		angularInertia[0] = Vector3::Dot(angularInertiaWorld, contacts[index].normal);

		linearInertia[0] = 1.0f / contacts[index].object1->rigidbody.GetMass();

		totalInertia += linearInertia[0] + angularInertia[0];
		if (contacts[index].object2 != nullptr)
		{
			inverseInertiaTensor = contacts[index].object2->transform.GetWorldMatrix() * contacts[index].object2->rigidbody.GetInertiaTensor();

			angularInertiaWorld = Vector3::Cross(relativeContactPosition[1], contacts[index].normal);
			angularInertiaWorld = inverseInertiaTensor * angularInertiaWorld;
			angularInertiaWorld = Vector3::Cross(angularInertiaWorld, relativeContactPosition[1]);
			angularInertia[1] = Vector3::Dot(angularInertiaWorld, contacts[index].normal);

			linearInertia[1] = 1.0f / contacts[index].object2->rigidbody.GetMass();

			totalInertia += linearInertia[1] + angularInertia[1];
		}

		// 域至馬壱 旋遂拝暗績
		float sign = 1;
		angularMove[0] = sign * contacts[index].depth * (angularInertia[0] / totalInertia);
		linearMove[0] = sign * contacts[index].depth * (linearInertia[0] / totalInertia);

		Vector3 projection = relativeContactPosition[0];
		projection += contacts[index].normal * Vector3::Dot(-relativeContactPosition[0], contacts[index].normal);

		float maxMagnitude = angularLimit * Vector3::Magnitude(projection);

		if (angularMove[0] < -maxMagnitude)
		{
			float totalMove = angularMove[0] + linearMove[0];
			angularMove[0] = -maxMagnitude;
			linearMove[0] = totalMove - angularMove[0];
		}
		else if (angularMove[0] > maxMagnitude)
		{
			float totalMove = angularMove[0] + linearMove[0];
			angularMove[0] = maxMagnitude;
			linearMove[0] = totalMove - angularMove[0];
		}

		if (angularMove[0] == 0)
		{
			angularChange[0] = Vector3::Zero();
		}
		else
		{
			Vector3 targetAngularDirection = ;
		}
	}

	// 3. 紗亀 繕舛


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

//void PhysicsManager::ResolveCollision()
//{
//	for (int i = 0; i < contacts.size(); i++)
//	{
//		Vector3 x = contacts[i].normal;
//		Vector3 y = Vector3::Up();
//		Vector3 z;
//
//		MakeOrthonormalBasis(x, &y, &z);
//		Matrix4x4 contactToWorldCoordMatrix = Matrix4x4(x, y, z);
//		// View 痕発 獣 朝五虞 奄層生稽 馬奄 是背 朝五虞税 transform税 蝕楳慶聖 弘端級拭惟 咽背爽牛, contact 奄層生稽 馬形檎 contact税 transform拭 背雁馬澗 basis楳慶税 蝕楳慶聖 咽背操醤 敗
//		//Matrix4x4 toContactCoordMatrix = contactCoordBasisMatrix.Inverse();
//
//		/* 食奄採斗 毘 域至 */
//		Vector3 relativeContactPosition = contacts[i].point - contacts[i].object1->transform.GetPosition();// 戚暗 薦企稽 段奄鉢 背操醤敗 弘端 1戚 焼観 呪 赤製
//		Vector3 torquePerUnitImpulse = Vector3::Cross(relativeContactPosition, contacts[i].normal);
//		Vector3 rotationPerUnitImpulse = contacts[i].object1->rigidbody.GetInertiaTensor().Inverse() * torquePerUnitImpulse;
//
//		// 中宜 走繊税 唖紗亀研 姥廃暗績(杉球 疎妊域 奄層)
//		Vector3 velocityPerUnitImpulse = Vector3::Cross(rotationPerUnitImpulse, relativeContactPosition);
//
//		// 酔軒澗 Contact 疎妊域 奄層 唖紗亀亜 琶推馬艦 Contact 疎妊域稽 痕発背 捜
//		Vector3 velocityPerUnitImpulseContact = contactToWorldCoordMatrix.Transpose() * velocityPerUnitImpulse;
//
//		// 酔軒亜 Contact Normal聖 x逐生稽 背辞 Basis研 姥梅生艦 衣厩 酔軒亜 据馬澗 Contact Normal 号狽 唖紗径精 x逐 葵戚 吃暗績 
//		float angularComponent = velocityPerUnitImpulseContact.x;
//
//
//		/* 食奄澗 恥 杯勲 域至..? 317pg 凧壱 */
//		Vector3 relativeContactPosition1 = contacts[i].point - contacts[i].object1->transform.GetPosition();
//		Vector3 deltaVelWorld = Vector3::Cross(relativeContactPosition1, contacts[i].normal);
//		deltaVelWorld = contacts[i].object1->transform.GetRotationMatrix() * contacts[i].object1->rigidbody.GetInertiaTensor().Inverse() * contacts[i].object1->transform.GetRotationMatrix().Inverse() * deltaVelWorld;
//		deltaVelWorld = Vector3::Cross(deltaVelWorld, relativeContactPosition1);
//
//		float deltaVelocity = Vector3::Dot(deltaVelWorld, contacts[i].normal);
//
//		deltaVelocity += (1.0f / contacts[i].object1->rigidbody.GetMass());
//
//		if (contacts[i].object2 != nullptr)
//		{
//			Vector3 relativeContactPosition2 = contacts[i].point - contacts[i].object2->transform.GetPosition();
//			deltaVelWorld = Vector3::Cross(relativeContactPosition2, contacts[i].normal);
//			deltaVelWorld = contacts[i].object2->transform.GetRotationMatrix() * contacts[i].object2->rigidbody.GetInertiaTensor().Inverse() * contacts[i].object2->transform.GetRotationMatrix().Inverse() * deltaVelWorld;
//			deltaVelWorld = Vector3::Cross(deltaVelWorld, relativeContactPosition2);
//
//			deltaVelocity += Vector3::Dot(deltaVelWorld, contacts[i].normal);
//
//			deltaVelocity += (1.0f / contacts[i].object2->rigidbody.GetMass());
//		}
//
//
//		/* 食奄澗 据馬澗 噺穿勲 域至..? 318pg 凧壱 */
//		relativeContactPosition1 = contacts[i].point - contacts[i].object1->transform.GetPosition();
//
//		Vector3 closingVelocity1 = Vector3::Cross(contacts[i].object1->rigidbody.GetAngularVelocity(), relativeContactPosition1);
//		closingVelocity1 += contacts[i].object1->rigidbody.GetVelocity();
//		if (contacts[i].object2 != nullptr)
//		{
//			Vector3 relativeContactPosition2 = contacts[i].point - contacts[i].object2->transform.GetPosition();
//
//			Vector3 closingVelocity2 = Vector3::Cross(contacts[i].object2->rigidbody.GetAngularVelocity(), relativeContactPosition2);
//			closingVelocity2 += contacts[i].object2->rigidbody.GetVelocity();
//
//			closingVelocity1 += closingVelocity2;
//		}
//		Vector3 totalClosingVelocity = contactToWorldCoordMatrix.Transpose() * closingVelocity1;
//		float restitution = 1.0f;
//		float desiredVelocity = -totalClosingVelocity.x * (1 + restitution);
//
//		Vector3 impulseContact = Vector3(desiredVelocity / deltaVelocity, 0, 0);
//		Vector3 impulse = contactToWorldCoordMatrix * impulseContact;
//
//		impulse *= -1;
//		Vector3 velocityChange = impulse / contacts[i].object1->rigidbody.GetMass();
//		Vector3 impulsiveTorque = Vector3::Cross(impulse, relativeContactPosition1);
//		Vector3 rotationChange = contacts[i].object1->transform.GetRotationMatrix() * contacts[i].object1->rigidbody.GetInertiaTensor().Inverse() * contacts[i].object1->transform.GetRotationMatrix().Inverse() * impulsiveTorque;
//
//		contacts[i].object1->rigidbody.AddVelocity(velocityChange * deltaTime);
//		contacts[i].object1->rigidbody.AddAngularVelocity(rotationChange * deltaTime);
//		if (contacts[i].object2 != nullptr)
//		{
//			Vector3 relativeContactPosition2 = contacts[i].point - contacts[i].object2->transform.GetPosition();
//
//			impulse *= -1;
//			velocityChange = impulse / contacts[i].object2->rigidbody.GetMass();
//			impulsiveTorque = Vector3::Cross(impulse, relativeContactPosition2);
//			rotationChange = contacts[i].object2->transform.GetRotationMatrix() * contacts[i].object2->rigidbody.GetInertiaTensor().Inverse() * contacts[i].object2->transform.GetRotationMatrix().Inverse() * impulsiveTorque;
//
//			contacts[i].object2->rigidbody.AddVelocity(velocityChange * deltaTime);
//			contacts[i].object2->rigidbody.AddAngularVelocity(rotationChange * deltaTime);
//		}
//	}
//
//
//	// 鋼降域呪(析舘 績税稽 走舛敗)
//	float restitution = 0.3f;
//	for (int i = 0; i < contacts.size(); i++)
//	{
//
//		// 1. 紗亀 痕井
//
//		float separatingVelocity;
//		Vector3 relativeVelocity = contacts[i].object1->rigidbody.GetVelocity() - contacts[i].object2->rigidbody.GetVelocity();
//		separatingVelocity = Vector3::Dot(relativeVelocity, contacts[i].normal);
//
//		// 菰嬢走壱 赤澗暗虞檎
//		if (separatingVelocity > 0)
//			continue;
//
//		float newSepVelocity = -separatingVelocity * restitution;
//
//		Vector3 accCausedVelocity = contacts[i].object1->rigidbody.GetAccumulatedForce() / contacts[i].object1->rigidbody.GetMass() - contacts[i].object2->rigidbody.GetAccumulatedForce() / contacts[i].object2->rigidbody.GetMass();
//		float accCausedSepVelocity = Vector3::Dot(accCausedVelocity, contacts[i].normal) * deltaTime;
//		if (accCausedSepVelocity < 0)
//		{
//			// ? 紗亀研 製呪艦猿 皐醤走 希背走澗暗 焼観悪..
//			newSepVelocity += restitution * accCausedSepVelocity;
//			if (newSepVelocity < 0) newSepVelocity = 0;
//		}
//		float deltaVelocity = newSepVelocity - separatingVelocity;
//		float totalInverseMass = 1.0f / contacts[i].object1->rigidbody.GetMass() + 1.0f / contacts[i].object2->rigidbody.GetMass();
//
//		if (totalInverseMass <= 0)
//			continue;
//
//		float impulse = deltaVelocity / totalInverseMass;
//		Vector3 impulsePerIMess = contacts[i].normal * impulse;
//		contacts[i].object1->rigidbody.AddVelocity(impulsePerIMess / contacts[i].object1->rigidbody.GetMass());
//		contacts[i].object2->rigidbody.AddVelocity(impulsePerIMess / -contacts[i].object2->rigidbody.GetMass());
//
//
//		// 2. 違庁 是帖 繕舛
//		if (contacts[i].depth <= 0)
//			continue;
//
//		totalInverseMass = 1.0f / contacts[i].object1->rigidbody.GetMass() + 1.0f / contacts[i].object2->rigidbody.GetMass();
//		// 霜勲拭 搾景背辞 恭嬰兜澗闇 呪縦 政亀研 公馬畏製.. 焼たたたたたたたたた
//		Vector3 movePerIMass = contacts[i].normal * (contacts[i].depth / totalInverseMass);
//		contacts[i].object1->transform.Translate(movePerIMass / contacts[i].object1->rigidbody.GetMass());
//		contacts[i].object2->transform.Translate(movePerIMass / -contacts[i].object2->rigidbody.GetMass());
//
//	}
//}
