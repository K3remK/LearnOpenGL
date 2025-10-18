#pragma once


#include "Core.hpp"

#include <string>
#include "Events/Event.hpp"
#include <functional>
#include <GLFW/glfw3.h>


struct WindowProps
{
	std::string Title;
	u32 Width, Height;
	GLFWmonitor* Monitor;
	GLFWwindow* Share;

	WindowProps(const std::string& title = "OpenGL Window", u32 width = 1600, u32 height = 900, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL)
		: Title(title), Width(width), Height(height), Monitor(monitor), Share(share)
	{ }
};

class Window
{
	using EventCallbackFn = std::function<void(Event&)>;

public:
	Window(const WindowProps& props = WindowProps());
	~Window();
	void OnUpdate(float ts);
	GLFWwindow* GetNativeWindow() const { return m_WindowHandle; };
	u32 GetWidth() const { return m_Data.Width; }
	u32 GetHeight() const { return m_Data.Height; }
	void SetVsync(bool val);
	void Shutdown();
	inline void SetEventCallback(const EventCallbackFn& fn) { m_Data.EventCallback = fn; }
private:
	GLFWwindow* m_WindowHandle;
	
	struct WindowData
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		EventCallbackFn EventCallback;
	};

	WindowData m_Data;
};