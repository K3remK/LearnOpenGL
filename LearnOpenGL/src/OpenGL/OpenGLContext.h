#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "../Events/ApplicationEvent.hpp"

class OpenGLContext 
{
public:
	OpenGLContext(GLFWwindow* windowHandle);
	void Init();
	void SwapBuffers() const;
	void OnEvent(Event& e);
private:
	bool OnWindowResize(WindowResizeEvent& e) const;
private:
	GLFWwindow* m_WindowHandle;
};
