#include "OpenGLContext.h"

#include <cassert>
#include <print>

#include <glad/glad.h>

void APIENTRY GLDebugMessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::cerr << "GL CALLBACK: "
		<< (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
		<< " type = 0x" << std::hex << type
		<< ", severity = 0x" << severity
		<< ", message = " << message
		<< std::endl;
}

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle)
{
	assert(windowHandle && "OpenGLContext window is null!");
}

void OpenGLContext::Init()
{
	/********      GLAD INITILIZATION       *****/
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		assert(true && "Failed to initialize GLAD");
	}

	// OpenGL state
	// ------------
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//// Enable debug output
	//glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes callback synchronous

	// Register callback
	glDebugMessageCallback(GLDebugMessageCallback, nullptr);

	//// Optionally filter messages
	//glDebugMessageControl(GL_DONT_CARE,        // any source
	//	GL_DONT_CARE,        // any type
	//	GL_DEBUG_SEVERITY_NOTIFICATION, // filter severity
	//	0, nullptr,          // no specific IDs
	//	GL_FALSE);           // disable

	//

	/*std::println("OpenGL Info: ");
	std::println(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	std::println(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	std::println(reinterpret_cast<const char*>(glGetString(GL_VERSION)));*/
}

void OpenGLContext::SwapBuffers() const
{
	glfwSwapBuffers(m_WindowHandle);
}

void OpenGLContext::OnEvent(Event& e)
{
	EventDispatcher ed(e);
	ed.Dispatch<WindowResizeEvent>(std::bind(&OpenGLContext::OnWindowResize, this, std::placeholders::_1));
}

bool OpenGLContext::OnWindowResize(WindowResizeEvent& e) const
{
	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	return true;
}
