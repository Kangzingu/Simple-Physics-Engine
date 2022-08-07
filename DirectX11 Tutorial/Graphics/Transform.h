#pragma once
#include "Model.h"

class Object;
class Transform
{
private:
	Vector3 position = Vector3::Zero();
	Quaternion orientation = Quaternion::Identity();
	Vector3 scale = Vector3::One();

	Vector3 right = Vector3::Right();
	Vector3 up = Vector3::Up();
	Vector3 forward = Vector3::Forward();

	Matrix4x4 worldMatrix = Matrix4x4::Identity();
	Matrix4x4 translationMatrix = Matrix4x4::Identity();
	Matrix4x4 rotationMatrix = Matrix4x4::Identity();
	Matrix4x4 scalingMatrix = Matrix4x4::Identity();

public:
	Object* object;

	Matrix4x4& GetWorldMatrix();
	Matrix4x4& GetTranslationMatrix();
	Matrix4x4& GetRotationMatrix();
	Matrix4x4& GetScalingMatrix();

	Vector3& GetPosition();
	Vector3& GetRotation();
	Quaternion& GetOrientation();
	Vector3& GetScale();

	Vector3& GetRight();
	Vector3& GetUp();
	Vector3& GetForward();

public:
	void Initialize(Vector3 position, Vector3 rotation, Vector3 scale);

	void SetPosition(Vector3 position);
	void SetRotation(Quaternion orientation);
	void SetScale(Vector3 scale);

	void Translate(Vector3 position);
	void Rotate(Vector3 rotation);
	void Rotate(Quaternion orientation);
	void Scale(Vector3 scale);
};