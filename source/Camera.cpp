#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

Camera::Camera()
{
	View = glm::mat4(1.0f);
	Projection = glm::mat4(1.0f);

	ViewDirty = true;
	ProjectionDirty = true;

	Position = glm::vec3(0.0f);
	Rotation = glm::vec3(0.0f);

	FieldOfView = 90.0f;
	NearClipPlane = 1.0f;
	FarClipPlane = 1000.0f;
}

const glm::mat4& Camera::GetView()
{
	if (ViewDirty)
	{
		UpdateView();
		ViewDirty = false;
	}
	return View;
}

const glm::mat4& Camera::GetProjection()
{
	if (ProjectionDirty)
	{
		UpdateProjection();
		ProjectionDirty = false;
	}
	return Projection;
}

glm::quat Camera::GetOrientation()
{
	glm::vec3 radians = glm::radians(Rotation);

	glm::quat pitch = glm::angleAxis(radians.x, glm::vec3(1, 0, 0));
	glm::quat yaw = glm::angleAxis(-radians.y, glm::vec3(0, 1, 0));
	glm::quat roll = glm::angleAxis(radians.z, glm::vec3(0, 0, 1));

	glm::quat orientation = glm::inverse(yaw * pitch * roll);

	return orientation;
}

glm::vec3 Camera::GetRight()
{
	glm::quat orientation = GetOrientation();
	return glm::vec3(1, 0, 0) * orientation;
}

glm::vec3 Camera::GetUp()
{
	glm::quat orientation = GetOrientation();
	return glm::vec3(0, 1, 0) * orientation;
}

glm::vec3 Camera::GetForward()
{
	glm::quat orientation = GetOrientation();
	return  glm::vec3(0, 0, -1) * orientation;
}

void Camera::UpdateView()
{
	glm::quat orientation = GetOrientation();

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), -Position);
	glm::mat4 rotation = glm::mat4_cast(orientation);

	View = rotation * translation;
}

void Camera::UpdateProjection()
{
	Projection = glm::perspective(glm::radians(75.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
}