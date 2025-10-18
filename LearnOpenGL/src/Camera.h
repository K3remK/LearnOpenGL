#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"

#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/ApplicationEvent.hpp"

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;
constexpr float FOV = 45.0f;

struct CameraSettings
{
	glm::vec3 Position = { 0.0, 0.0f, 10.0f };
	glm::vec3 Front = { 0.0f, 0.0f, -1.0f };
	glm::vec3 Up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 Right;
	glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

	float NearClip = 0.1f;
	float FarClip = 100.0f;

	float MovementSpeed = SPEED;
	float MouseSensitivity = SENSITIVITY;
	float Fov = FOV;

	float Yaw = YAW;
	float Pitch = PITCH;
};

class Camera
{
public:
	Camera(const CameraSettings& settings = CameraSettings());

	inline float GetNearClip() const { return m_Settings.NearClip; }
	inline float GetFarClip() const { return m_Settings.FarClip; }
	inline glm::vec3 GetPosition() const { return m_Settings.Position; }
	inline glm::vec3 GetFront() const { return m_Settings.Front; }
	inline glm::vec3 GetRight() const { return m_Settings.Right; }
	inline glm::vec3 GetUp() const { return m_Settings.Up; }
	inline float GetFov() const { return m_Settings.Fov; }
	inline void CameraShouldUpdate(bool val) { m_UpdateCamera = val; }
	inline void ConstrainPitch(bool enable) { m_ConstainPitch = enable; }
	inline void Reset() { m_Settings = CameraSettings(); }


	glm::mat4 GetViewMatrix() const;

	glm::mat4 GetProjectionMatrix() const;

	void OnEvent(Event& e);
	void OnUpdate(float ts);
private:
	bool OnKeyPressedEvent(KeyPressedEvent& e);
	bool OnMouseMovedEvent(MouseMovedEvent& e);
	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	bool OnWindowResized(WindowResizeEvent& e);
private:
	void UpdateCameraVectors();
private:
	CameraSettings m_Settings = CameraSettings();
	// TODO: do not hard code this
	float m_ViewportWidth = 1600, m_ViewportHeight = 900;
	bool m_FirstMouseMove = true;
	glm::vec2 m_LastMousePos = { 0.0f, 0.0f };
	bool m_UpdateCamera = true;
	bool m_ConstainPitch = true;
};