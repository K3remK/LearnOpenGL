#include "Camera.h"

#include "GLFW/glfw3.h"
#include "Input.hpp"

Camera::Camera(const CameraSettings& settings /*= CameraSettings()*/)
	: m_Settings(settings)
{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(GetPosition(), GetPosition() + GetFront(), GetUp());
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspectiveFov(glm::radians(GetFov()), m_ViewportWidth, m_ViewportHeight, GetNearClip(), GetFarClip());
}

void Camera::OnUpdate(float ts)
{
	float velocity = m_Settings.MovementSpeed * ts;

	if (Input::IsKeyPressed(GLFW_KEY_W))
		m_Settings.Position += m_Settings.Front * velocity;
	
	if (Input::IsKeyPressed(GLFW_KEY_S))
		m_Settings.Position -= m_Settings.Front * velocity;

	if (Input::IsKeyPressed(GLFW_KEY_A))
		m_Settings.Position -= m_Settings.Right * velocity;

	if (Input::IsKeyPressed(GLFW_KEY_D))
		m_Settings.Position += m_Settings.Right * velocity;

	if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		m_Settings.Position -= m_Settings.Up * velocity;

	if (Input::IsKeyPressed(GLFW_KEY_SPACE))
		m_Settings.Position += m_Settings.Up * velocity;

}

void Camera::OnEvent(Event& e)
{
	EventDispatcher dp(e);

	dp.Dispatch<WindowResizeEvent>(std::bind(&Camera::OnWindowResized, this, std::placeholders::_1));
	dp.Dispatch<KeyPressedEvent>(std::bind(&Camera::OnKeyPressedEvent, this, std::placeholders::_1));
	dp.Dispatch<MouseMovedEvent>(std::bind(&Camera::OnMouseMovedEvent, this, std::placeholders::_1));
	dp.Dispatch<MouseScrolledEvent>(std::bind(&Camera::OnMouseScrolledEvent, this, std::placeholders::_1));
}

bool Camera::OnKeyPressedEvent(KeyPressedEvent& e)
{
	float velocity = m_Settings.MovementSpeed;
	switch (e.GetKeyCode())
	{
	case GLFW_KEY_R:                     Reset(); break;
	}

	return true;
}

bool Camera::OnMouseMovedEvent(MouseMovedEvent& e)
{
	if (m_FirstMouseMove)
	{
		m_LastMousePos = { e.GetX(), e.GetY() };
		m_FirstMouseMove = false;
	}
	if (!m_UpdateCamera)
		return false;

	float xoffset = e.GetX() - m_LastMousePos.x;
	float yoffset = m_LastMousePos.y - e.GetY();

	m_LastMousePos = { e.GetX(), e.GetY() };

	xoffset *= m_Settings.MouseSensitivity;
	yoffset *= m_Settings.MouseSensitivity;

	m_Settings.Yaw += xoffset;
	m_Settings.Pitch += yoffset;

	if (m_ConstainPitch)
	{
		if (m_Settings.Pitch > 89.0f)
			m_Settings.Pitch = 89.0f;
		else if (m_Settings.Pitch < -89.0f)
			m_Settings.Pitch = -89.0f;
	}

	UpdateCameraVectors();
	return true;
}

bool Camera::OnMouseScrolledEvent(MouseScrolledEvent& e)
{
	if (!m_UpdateCamera)
		return false;

	m_Settings.Fov += (e.GetXOffset() - e.GetYOffset());
	m_Settings.Fov = std::fmax(m_Settings.Fov, 10.0f);
	m_Settings.Fov = std::fmin(m_Settings.Fov, 170.0f);

	return true;
}

bool Camera::OnWindowResized(WindowResizeEvent& e)
{
	m_ViewportWidth = e.GetWidth();
	m_ViewportHeight = e.GetHeight();
	return true;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_Settings.Yaw)) * cos(glm::radians(m_Settings.Pitch));
	direction.y = sin(glm::radians(m_Settings.Pitch));
	direction.z = sin(glm::radians(m_Settings.Yaw)) * cos(glm::radians(m_Settings.Pitch));
	m_Settings.Front = glm::normalize(direction);

	m_Settings.Right = glm::normalize(glm::cross(m_Settings.Front, m_Settings.WorldUp));
	m_Settings.Up = glm::normalize(glm::cross(m_Settings.Right, m_Settings.Front));
}
