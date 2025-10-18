#include "Window.hpp"

#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>

#include "Events/MouseEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/ApplicationEvent.hpp"

Window::Window(const WindowProps& props)
{

	m_Data.Title = props.Title;
	m_Data.Width = props.Width;
	m_Data.Height = props.Height;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MAC OS

	/********      WINDOW INITILIZATION       *****/

	m_WindowHandle = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), props.Monitor, props.Share);
	if (m_WindowHandle == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		assert(!m_WindowHandle && "Failed to create GLFW window!");
	}
	glfwMakeContextCurrent(m_WindowHandle); // make the current threads context this window

	glfwSetWindowUserPointer(m_WindowHandle, &m_Data);
	SetVsync(true);


	// Window close event
	glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			data.EventCallback(e);
		});

	// Mouse moved callback
	glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* window, double xpos, double ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent e(static_cast<float>(xpos), static_cast<float>(ypos));
			data.EventCallback(e);
		});

	// Window Resize Event
	glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowResizeEvent e(width, height);
			data.EventCallback(e);
		});

	// Mouse scroll callback
	glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent e(static_cast<float>(xoffset), static_cast<float>(yoffset));
		data.EventCallback(e);
	});

	// Key Event Callbacks
	glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
			case GLFW_PRESS:
			{
				KeyPressedEvent e(key);
				data.EventCallback(e);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent e(key);
				data.EventCallback(e);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent e(key, true);
				data.EventCallback(e);
				break;
			}
		}
	});

	// Mouse button event callback
	glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window, int button, int action, int mode) 
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent e(button);
			data.EventCallback(e);
		}
		break;
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent e(button);
			data.EventCallback(e);
		}
		break;
		}
	});
}

Window::~Window()
{
	Shutdown();
}

void Window::OnUpdate(float ts)
{
	glfwPollEvents();
}

void Window::SetVsync(bool val)
{
	if (val)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_Data.VSync = val;
}

void Window::Shutdown()
{
	glfwDestroyWindow(m_WindowHandle);
	glfwTerminate();
}

