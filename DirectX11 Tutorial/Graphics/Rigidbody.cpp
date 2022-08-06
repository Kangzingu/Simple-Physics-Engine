#include "Object.h"
#include "Rigidbody.h"

void Rigidbody::Initialize(float inverseMass, float damping, float angularDamping, Vector3 velocity, Vector3 rotationVelocity, Matrix4x4 inertiaTensor)
{
	this->inverseMass = inverseMass;
	this->damping = damping;
	this->angularDamping = angularDamping;
	this->velocity = velocity;
	this->angularVelocity = rotationVelocity;
	this->inertiaTensor = inertiaTensor;
	this->ClearAccumulatedForce();
}
void Rigidbody::AddForce(Vector3 force)
{
	accumulatedForce += force;
}
void Rigidbody::AddForceAt(Vector3 force, Vector3 worldPoint)
{
	accumulatedForce += force;
	accumulatedTorque +=  (Vector3::Cross(worldPoint - object->transform.GetPosition(), force));
}
void Rigidbody::AddTorqueAt(Vector3 force, Vector3 worldPoint)
{
	accumulatedTorque += (Vector3::Cross(worldPoint - object->transform.GetPosition(), force));
}
void Rigidbody::Update(float deltaTime)
{
	velocity = (velocity + (accumulatedForce * inverseMass) * deltaTime) * pow(damping, deltaTime);
	object->transform.Translate(velocity * deltaTime);
	
	angularVelocity = (angularVelocity + (GetWorldInertiaTensorInverse() * accumulatedTorque) * deltaTime) * pow(angularDamping, deltaTime);
	object->transform.Rotate(angularVelocity * deltaTime);

	ClearAccumulatedForce();
}
void Rigidbody::SetEnabled(bool isEnabled)
{
	this->isEnabled = isEnabled;
}
void Rigidbody::SetInverseMass(float inverseMass)
{
	this->inverseMass = inverseMass;
}
void Rigidbody::SetDamping(float damping)
{
	this->damping = damping;
}
void Rigidbody::SetAngularDamping(float angularDamping)
{
	this->angularDamping = angularDamping;
}
void Rigidbody::SetVelocity(Vector3 velocity)
{
	this->velocity = velocity;
}
void Rigidbody::AddVelocity(Vector3 velocity)
{
	this->velocity += velocity;
}
void Rigidbody::SetAngularVelocity(Vector3 angularVelocity)
{
	this->angularVelocity = angularVelocity;
}
void Rigidbody::AddAngularVelocity(Vector3 angularVelocity)
{
	this->angularVelocity += angularVelocity;
}
void Rigidbody::SetAccumulatedForce(Vector3 accumulatedForce)
{
	this->accumulatedForce = accumulatedForce;
}
void Rigidbody::SetAccumulatedTorque(Vector3 accumulatedTorque)
{
	this->accumulatedTorque = accumulatedTorque;
}
void Rigidbody::SetInertiaTensor(Matrix4x4 inertiaTensor)
{
	this->inertiaTensor = inertiaTensor;
}
void Rigidbody::ClearAccumulatedForce()
{
	accumulatedForce = Vector3::Zero();
	accumulatedTorque = Vector3::Zero();
}
bool Rigidbody::IsEnabled(){return isEnabled;}
float Rigidbody::GetInverseMass(){return inverseMass;}
float Rigidbody::GetDamping(){return damping;}
float Rigidbody::GetangularDamping(){return angularDamping;}
Vector3 Rigidbody::GetVelocity(){return velocity;}
Vector3 Rigidbody::GetAngularVelocity(){return angularVelocity;}
Vector3 Rigidbody::GetAccumulatedForce(){return accumulatedForce;}
Vector3 Rigidbody::GetAccumulatedTorque(){return accumulatedTorque;}
Matrix4x4 Rigidbody::GetInertiaTensor(){return inertiaTensor;}

Matrix4x4 Rigidbody::GetWorldInertiaTensorInverse()
{
	return Matrix4x4((object->transform.GetRotationMatrix() * inertiaTensor) * object->transform.GetRotationMatrix().Inverse());
}

// �� = ���� * ���ӵ�
// ���޽� �� = ���� * �ӵ�
// ��ũ = �����ټ� * �����ӵ�
// ���޽� ��ũ = �����ټ� * ���ӵ�
// ���ӵ� = �����ټ�^-1 * (�����ġ X ���޽� ��) �ε�..?