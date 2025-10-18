#include "Input.hpp"

#include "GLFW/glfw3.h"
#include "Application.hpp"

bool Input::IsKeyPressed(int key)
{
	return glfwGetKey(Application::Get()->GetWindow().GetNativeWindow(), key) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button)
{
	return glfwGetMouseButton(Application::Get()->GetWindow().GetNativeWindow(), button) == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
	double xpos, ypos;
	glfwGetCursorPos(Application::Get()->GetWindow().GetNativeWindow(), &xpos, &ypos);
	return glm::vec2((float)xpos, (float)ypos);
}

float Input::GetMouseX()
{
	return GetMousePosition().x;
}

float Input::GetMouseY()
{
	return GetMousePosition().y;
}
