

#include "Events/Event.hpp"
#include "Window.hpp"
#include "OpenGL/OpenGLContext.h"
#include "Camera.h"
#include <memory>

class Application 
{
public:
	Application(const std::string& name);
	virtual ~Application();
	void Run();
	void OnEvent(Event& e);
	static Application* Get() { return s_Instance; };
	Window& GetWindow() const { return *m_Window; }
private:
	bool OnWindowClose(WindowCloseEvent& e);
private:
	static Application* s_Instance;
	std::unique_ptr<Window> m_Window;
	std::unique_ptr<OpenGLContext> m_OpenGLContext;

	std::shared_ptr<Camera> m_Camera;

	bool m_Running = true;
};