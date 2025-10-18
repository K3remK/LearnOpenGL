#include "Application.hpp"
#include <cassert>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "Shader.h"

#include <glm/glm.hpp>

void SetDarkThemeColors();

Application* Application::s_Instance = nullptr;

Application::Application(const std::string& name)
{
	assert(!s_Instance && "Application already exists!");

	m_Window = std::make_unique<Window>(WindowProps(name));
	m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

	m_OpenGLContext = std::make_unique<OpenGLContext>(m_Window->GetNativeWindow());
	m_OpenGLContext->Init();

	m_Camera = std::make_shared<Camera>();

	s_Instance = this;
}

Application::~Application()
{
}

void Application::Run()
{

	//// text rendering stuff
	//FT_Library ft;
	//if (FT_Init_FreeType(&ft))
	//{
	//	std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	//	return -1;
	//}

	//FT_Face face;
	//if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	//{
	//	std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	//	return -1;
	//}
	//FT_Set_Pixel_Sizes(face, 0, 48);  // height of the font will be 48px and width will be determined based on that

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction since the texture that is created is 8-bit (1 byte)

	//for (unsigned char c = 0; c < 128; c++)
	//{
	//	// load character glyph 
	//	if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	//	{
	//		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	//		continue;
	//	}
	//	// generate texture
	//	unsigned int texture;
	//	glGenTextures(1, &texture);
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	glTexImage2D(
	//		GL_TEXTURE_2D,
	//		0,
	//		GL_RED,
	//		face->glyph->bitmap.width,
	//		face->glyph->bitmap.rows,
	//		0,
	//		GL_RED,
	//		GL_UNSIGNED_BYTE,
	//		face->glyph->bitmap.buffer
	//	);
	//	// set texture options
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//	// now store character for later use
	//	Character character = {
	//		texture,
	//		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	//		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
	//		static_cast<unsigned int>(face->glyph->advance.x)
	//	};
	//	Characters.insert(std::pair<char, Character>(c, character));
	//}
	//glBindTexture(GL_TEXTURE_2D, 0);
	//// clear resources  
	//FT_Done_Face(face);
	//FT_Done_FreeType(ft);

	float vertices[] = {
		// positions         // colors          // texture coordinates
		-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // bottom-left yellow
		-0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // top-left green
		0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // top-right  blue
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,    1.0f, 0.0f    // bottom-right  red
	};

	float CubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePositions[] = {

		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)

	};

	float texCoords[] = {
		0.0f, 0.0f, // lower-left corner
		1.0f, 0.0f, // lower-right corner
		0.5f, 1.0f // top-center corner
	};

	float vertices2[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
	0, 1, 2,   // first triangle
	0, 2, 3    // second triangle
	};

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// texture coord
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(2);

	// Element buffer array for indexed drawing of overlapping vertices
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attributes bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// VAO and VBO for text textures;
	//glGenVertexArrays(1, &VAO2);
	//glGenBuffers(1, &VBO2);
	//glBindVertexArray(VAO2);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // The 2D quad requires 6 vertices of 4 floats each, so we reserve 6 * 4 floats of memory. 
	//// Because we'll be updating the content of the VBO's memory quite often we'll allocate the memory with GL_DYNAMIC_DRAW.
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	// uncomment this call to draw in wire frame polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	const char* cubeObjectVShaderFile = "assets/shaders/triangle_vert.glsl";
	const char* cubeObjectFShaderFile = "assets/shaders/triangle_frag.glsl";

	const char* lightObjectVShader = "assets/shaders/lighting_vert.glsl";
	const char* lightObjectFShader = "assets/shaders/lighting_frag.glsl";

	/*const char* textVShader = "text_vert.glsl";
	const char* textFShader = "text_frag.glsl";*/

	Shader cubeObjectShader(cubeObjectVShaderFile, cubeObjectFShaderFile);
	Shader lightObjectShader(lightObjectVShader, lightObjectFShader);
	//Shader textShader(textVShader, textFShader);

	// textures

	// texture 1
	//unsigned int texture1, texture2;
	//glGenTextures(1, &texture1);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);   // tell stb_image.h to flip loaded texture's on the y-axis.
	//unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);

	//// texture 2
	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture2);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}
	//stbi_image_free(data);

	//// for GL_BORDER_TEXTURE_COLOR
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	//
	// tell openGL for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	// either set it manually like so:
	//glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
	// or set it via the texture class
	//shader.setInt("texture2", 1);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Window->GetWidth()), 0.0f, static_cast<float>(m_Window->GetHeight()));
	/*textShader.use();
	textShader.setMat4("projection", projection);*/

	// imgui setup

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();


	float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only
	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)
#if GLFW_VERSION_MAJOR >= 3 && GLFW_VERSION_MINOR >= 3
	io.ConfigDpiScaleFonts = true;          // [Experimental] Automatically overwrite style.FontScaleDpi in Begin() when Monitor DPI changes. This will scale fonts but _NOT_ scale sizes/padding for now.
	io.ConfigDpiScaleViewports = true;      // [Experimental] Scale Dear ImGui and Platform Windows when Monitor DPI changes.
#endif

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	SetDarkThemeColors();


	// imgui settings
	static float lightScale = 0.6f;
	static float objectScale = 1.0f;
	float clearColor[3] = { 0.2f, 0.3f, 0.3f };

	float lightColor[3] = { 1.0f, 1.0f, 1.0f };
	float lightPos[3] = { 1.2f, 1.0f, 2.0f };


	float objectColor[3] = { 0.5f, 1.0f, 0.31f };

	while (!glfwWindowShouldClose(m_Window->GetNativeWindow()))   // main loop
	{

		//std::cout << "m_Camera position: " << Util::vector_to_string(m_Camera._position) << std::endl;

		/*deltaTime = glfwGetTime() - lastFrame;
		lastFrame = glfwGetTime();*/
		// input handling
		//processInput(window.GetNativeWindow());

		// rendering commands here
		// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// bind textures on corresponding texture units
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/

		// render container
		glBindVertexArray(VAO);
		cubeObjectShader.use();
		// set the matrices

		// view matrix
		//glm::mat4 view(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		/*float radius = 5.0f;
		float currentTime = glfwGetTime();
		lightPos.x = cos(currentTime) * radius;
		lightPos.y = sin(currentTime) * radius;
		lightPos.z = sin(currentTime) * radius;*/

		/*if (glfwGetInputMode(window.GetNativeWindow(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
			updatem_Camera = false;
		else
			updatem_Camera = true;*/



		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("settings");

		ImGui::ColorPicker3("Cube Color", objectColor);
		ImGui::SliderFloat("Cube Scale", &objectScale, 0.1f, 3.0f);

		ImGui::ColorPicker3("Light Color", lightColor);
		ImGui::SliderFloat3("Light Position", lightPos, -10.0f, 10.0f);
		ImGui::SliderFloat("Light Scale", &lightScale, 0.1f, 2.0f);

		ImGui::ColorPicker3("Clear Color", clearColor);


		ImGui::End();

		// RenderCommand
		glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);

		// view matrix
		glm::mat4 view = m_Camera->GetViewMatrix();

		// projection matrix
		glm::mat4 proj = m_Camera->GetProjectionMatrix();

		// model matrix
		glm::mat4 model(1.0f);
		model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(objectScale));
		cubeObjectShader.setVec3("lightPos", glm::make_vec3(lightPos));
		cubeObjectShader.setVec3("lightColor", glm::make_vec3(lightColor));
		cubeObjectShader.setVec3("objectColor", glm::make_vec3(objectColor));
		cubeObjectShader.setVec3("viewPos", m_Camera->GetPosition());
		cubeObjectShader.setMat4("model", model);
		cubeObjectShader.setMat4("view", view);
		cubeObjectShader.setMat4("proj", proj);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// also draw the lamp object
		lightObjectShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::make_vec3(lightPos));
		model = glm::scale(model, glm::vec3(lightScale));
		lightObjectShader.setVec3("lightPos", glm::make_vec3(lightPos));
		lightObjectShader.setVec3("lightColor", glm::make_vec3(lightColor));
		lightObjectShader.setMat4("model", model);
		lightObjectShader.setMat4("proj", proj);
		lightObjectShader.setMat4("view", view);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// ImGui Rendering
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(static_cast<float>(m_Window->GetWidth()), static_cast<float>(m_Window->GetHeight()));

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		// transformation matrix
		/*glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (static_cast<float>(glfwGetTime())), glm::vec3(0.0f, 1.0f, 0.0f));
		float scaleFactor = static_cast<float>(cos(glfwGetTime()));
		trans = glm::scale(trans, glm::vec3(scaleFactor));
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform")
			, 1, GL_FALSE, glm::value_ptr(trans));*/

			//cubeObjectShader.setFloat("mixPercent", mixPercent);

			//for (unsigned int i = 0; i < 10; ++i)
			//{
			//	// model matrix
			//	glm::mat4 model(1.0f);
			//	model = glm::translate(model, cubePositions[i]);
			//	if((i+1) % 3 == 0)
			//		model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 1.0f, 1.0f));
			//	cubeObjectShader.setMat4("model", model);
			//	glDrawArrays(GL_TRIANGLES, 0, 36);
			//}

			// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw triangles using EBO
			//glBindVertexArray(0); // no need to unbind it everytime

			// transformation matrix
			/*trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
			scaleFactor = static_cast<float>(sin(glfwGetTime()));
			trans = glm::scale(trans, glm::vec3(scaleFactor));
			glUniformMatrix4fv(glGetUniformLocation(shader.ID, "transform")
				, 1, GL_FALSE, glm::value_ptr(trans));*/

				//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				// 
				//  Render the text
				//RenderText(textShader, "m_Camera Position: " + Util::vector_to_string(m_Camera._position), 25.0f, 25.0f, 0.5f, glm::vec3(0.5f, 0.8f, 0.2f));

				// check and call events and swap buffers

		m_OpenGLContext->SwapBuffers();
		m_Window->OnUpdate(ImGui::GetIO().DeltaTime);
		m_Camera->OnUpdate(ImGui::GetIO().DeltaTime);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(cubeObjectShader.ID);
}

void Application::OnEvent(Event& e)
{
	EventDispatcher ed(e);
	ed.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
	m_OpenGLContext->OnEvent(e);
	m_Camera->OnEvent(e);
}



bool Application::OnWindowClose(WindowCloseEvent& e)
{
	m_Running = false;
	return true;
}

void SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}