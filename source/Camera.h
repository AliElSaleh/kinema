#pragma once

#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"

class Camera
{
public:
	Camera();

	const glm::mat4& GetView();
	const glm::mat4& GetProjection();

	glm::vec3 GetRight();
	glm::vec3 GetUp();
	glm::vec3 GetForward();

	glm::vec3 GetPosition() { return Position; }
	glm::vec3 GetRotation() { return Rotation; }

	glm::quat GetOrientation();

	void SetPosition(glm::vec3 position) { Position = position; ViewDirty = true; }
	void SetRotation(glm::vec3 rotation) { Rotation = rotation; ViewDirty = true; }

	void SetFieldOfView(float degrees) { FieldOfView = degrees; ProjectionDirty = true; }
	void SetNearClipPlane(float near) { NearClipPlane = near; ProjectionDirty = true; }
	void SetFarClipPlane(float far) { FarClipPlane = far; ProjectionDirty = true; }

private:
	glm::mat4 View;
	glm::mat4 Projection;

	bool ViewDirty;
	bool ProjectionDirty;

	glm::vec3 Position;
	glm::vec3 Rotation;

	float FieldOfView;
	float NearClipPlane;
	float FarClipPlane;

	void UpdateView();
	void UpdateProjection();
};