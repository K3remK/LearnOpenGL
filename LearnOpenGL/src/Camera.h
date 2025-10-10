#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	UPWARD,
	DOWNWARD,
	LEFT,
	RIGHT
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 1.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float FOV = 45.0f;

struct CameraSettings
{
	glm::vec3 Position = { 0.0, 0.0f, 10.0f };
	glm::vec3 Front = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 Right;
	glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

	float MovementSpeed = SPEED;
	float MouseSensitivity = SENSITIVITY;
	float Fov = FOV;

	float Yaw = YAW;
	float Pitch = PITCH;
};

class Camera
{
public:
	Camera(const CameraSettings& settings)
		: m_Settings(settings)
	{
		UpdateCameraVectors();
	}

	Camera()
	{
		m_Settings = CameraSettings();
		UpdateCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(m_Settings.Position, m_Settings.Front + m_Settings.Position, m_Settings.Up);
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = m_Settings.MovementSpeed * deltaTime;

		if (direction == FORWARD)
			m_Settings.Position += m_Settings.Front * velocity;
		if (direction == BACKWARD)
			m_Settings.Position -= m_Settings.Front * velocity;
		if (direction == LEFT)
			m_Settings.Position -= m_Settings.Right* velocity;
		if (direction == RIGHT)
			m_Settings.Position += m_Settings.Right* velocity;
		if (direction == UPWARD)
			m_Settings.Position += m_Settings.Up* velocity;
		if (direction == DOWNWARD)
			m_Settings.Position -= m_Settings.Up* velocity;

	}

	void ProcessMouseMovement(float xoffset, float yoffset, const GLboolean constraintPitch = true)
	{
		xoffset *= m_Settings.MouseSensitivity;
		yoffset *= m_Settings.MouseSensitivity;

		m_Settings.Yaw += xoffset;
		m_Settings.Pitch += yoffset;

		if (constraintPitch)
		{
			if (m_Settings.Pitch > 89.0f)
				m_Settings.Pitch = 89.0f;
			else if (m_Settings.Pitch < -89.0f)
				m_Settings.Pitch = -89.0f;
		}

		UpdateCameraVectors();
	}

	void ProcessMouseScroll(float xoffset, float yoffset)
	{
		m_Settings.Fov += (xoffset - yoffset);
		m_Settings.Fov = std::fmax(m_Settings.Fov, 10.0f);
		m_Settings.Fov = std::fmin(m_Settings.Fov, 170.0f);
	}

	void Reset()
	{
		m_Settings = CameraSettings();
	}

private:
	void UpdateCameraVectors()
	{
		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Settings.Yaw)) * cos(glm::radians(m_Settings.Pitch));
		direction.y = sin(glm::radians(m_Settings.Pitch));
		direction.z = sin(glm::radians(m_Settings.Yaw)) * cos(glm::radians(m_Settings.Pitch));
		m_Settings.Front = glm::normalize(direction);

		m_Settings.Right = glm::normalize(glm::cross(m_Settings.Front, m_Settings.WorldUp));
		m_Settings.Up = glm::normalize(glm::cross(m_Settings.Right, m_Settings.Front));
	}
public:
	CameraSettings m_Settings;
};