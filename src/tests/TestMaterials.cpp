#include "TestMaterials.h"

#include "DumbDefine.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

namespace test{

	TestMaterials::TestMaterials()
		:camera(glm::vec3(0.0f, 0.0f, 3.0f))
	{
		float vertices[] = { VERTICES_LIGHT_CUBE };

		RegisterMaterial();

		m_ActualMaterial = LocateMaterial("Default");

		// Vertex Buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 6 * 36 * sizeof(float));

		// Shader
		m_Shader = std::make_unique<Shader>("res/shader/Materials.shader");
		m_Shader->Bind();

		// Cube uniforms
		m_Shader->SetUniform3f("material.ambient", m_ActualMaterial->ambient.x, m_ActualMaterial->ambient.y, m_ActualMaterial->ambient.z);
		m_Shader->SetUniform3f("material.diffuse", m_ActualMaterial->diffuse.x, m_ActualMaterial->diffuse.y, m_ActualMaterial->diffuse.z);
		m_Shader->SetUniform3f("material.specular", m_ActualMaterial->specular.x, m_ActualMaterial->specular.y, m_ActualMaterial->specular.z);
		m_Shader->SetUniform1f("material.shininess", m_ActualMaterial->shininess * 128);

		m_Shader->SetUniform3f("light.ambient", 0.2f, 0.2f, 0.2f);
		m_Shader->SetUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		m_Shader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
		m_Shader->SetUniform3f("light.position", m_LightPosition.x, m_LightPosition.y, m_LightPosition.z);

		m_Shader->SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		//Layout
		VertexBufferLayout cubeLayout;
		cubeLayout.Push<float>(3);
		cubeLayout.Push<float>(3);

		// Vertex Array
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VertexBuffer, cubeLayout);

		//Light stuff here
		m_LightVertexBuffer = std::make_unique<VertexBuffer>(vertices, 5 * 36 * sizeof(float));

		m_LightShader = std::make_unique<Shader>("res/shader/lightCube.shader");
		m_LightShader->Bind();

		m_LightShader->SetUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

		VertexBufferLayout lightLayout;
		lightLayout.Push<float>(3);
		lightLayout.Push<float>(3);

		m_LightVAO = std::make_unique<VertexArray>();
		m_LightVAO->AddBuffer(*m_LightVertexBuffer, lightLayout);

		GLCall(glEnable(GL_DEPTH_TEST));
	}

	TestMaterials::~TestMaterials()
	{
		GLFWwindow* window = glfwGetCurrentContext();
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
	}

	void TestMaterials::OnUpdate(float deltaTime)
	{
		static Material* ma = m_ActualMaterial;

		input(deltaTime);
		m_Shader->Bind();

		// Cube uniforms
		m_Shader->SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		if (ma != m_ActualMaterial)
		{

			m_Shader->SetUniform3f("material.ambient", m_ActualMaterial->ambient.x, m_ActualMaterial->ambient.y, m_ActualMaterial->ambient.z);
			m_Shader->SetUniform3f("material.diffuse", m_ActualMaterial->diffuse.x, m_ActualMaterial->diffuse.y, m_ActualMaterial->diffuse.z);
			m_Shader->SetUniform3f("material.specular", m_ActualMaterial->specular.x, m_ActualMaterial->specular.y, m_ActualMaterial->specular.z);
			m_Shader->SetUniform1f("material.shininess", m_ActualMaterial->shininess * 128);

		}

		//m_Shader->SetUniform3f("light.ambient", 0.2f, 0.2f, 0.2f);
		//m_Shader->SetUniform3f("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
		//m_Shader->SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
		m_Shader->SetUniform3f("light.position", m_LightPosition.x, m_LightPosition.y, m_LightPosition.z);

		glm::mat4 view = camera.GetViewMatrix();
		m_Shader->SetUniformMat4f("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), 940.0f / 540.0f, 0.1f, 100.0f);
		m_Shader->SetUniformMat4f("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);

		if (m_AutoRotate)
			model = glm::rotate(model, ((float)glfwGetTime() * m_RotationVelocity) * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		
		m_Shader->SetUniformMat4f("model", model);

		// ------------------------------------
		// Light Cube rendering
		m_LightShader->Bind();

		m_LightShader->SetUniform4f("color", m_LightColor.x, m_LightColor.y, m_LightColor.z, 1.0f);

		m_LightShader->SetUniformMat4f("view", view);
		m_LightShader->SetUniformMat4f("projection", projection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPosition);
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		if (!m_AutoRotate)
		{
			model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else
		{
			model = glm::rotate(model, ((float)glfwGetTime() * m_RotationVelocity) * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		}
		m_LightShader->SetUniformMat4f("model", model);

		ma = m_ActualMaterial;
	}

	void TestMaterials::OnRender()
	{
		GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_VAO->Bind();
		m_Shader->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		m_LightVAO->Bind();
		m_LightShader->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	void TestMaterials::OnImGuiRender()
	{

		if (m_AutoRotate)
			ImGui::SliderFloat("Rotation Velocity", &m_RotationVelocity, 0.02f, 3.0f);
		ImGui::SliderFloat("FOV", &camera.Zoom, 0.01f, 90.0f);
		ImGui::SliderFloat3("Light Cube Pos", &m_LightPosition.x, -3.0f, 3.0f);
		ImGui::ColorEdit4("Background Color", m_clearColor);
		ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
	
		ImGui::Text("Materials:");

		for (auto& m : m_Materials)
		{
			if (ImGui::Button(m.first.c_str()))
			{
				m_ActualMaterial = &m.second;	
			}
		}
	}

	inline void TestMaterials::input(float deltaTime)
	{
		using namespace input;

		static float oldMX = 0.0f;
		static float oldMY = 0.0f;
		static bool shouldMouse = false, Will = false;

		if (oldMX == Mouse::mouse_pos_x && oldMY == Mouse::mouse_pos_y)
			shouldMouse = false;
		else
			shouldMouse = true;

		//X
		if (keyboard.isPress(Keyboard::ky::A_KEY))
		{
			camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
		}
		if (keyboard.isPress(Keyboard::ky::D_KEY))
		{
			camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
		}
		//Z
		if (keyboard.isPress(Keyboard::ky::W_KEY))
		{
			camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
		}
		if (keyboard.isPress(Keyboard::ky::S_KEY))
		{
			camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
		}
		keyboard.clicked(Keyboard::ky::ARROW_DOWN, [&]() {
			GLFWwindow* window = glfwGetCurrentContext();
			if (window)
			{
				GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED));
				Will = true;
			}
		});

		keyboard.clicked(Keyboard::ky::ARROW_UP, [&]() {
			GLFWwindow* window = glfwGetCurrentContext();
			if (window)
			{
				GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
				Will = false;
			}
		});

		if (shouldMouse && Will)
			camera.ProcessMouseMovement(mouse.mouse_pos_x, mouse.mouse_pos_y);

		oldMX = Mouse::mouse_pos_x;
		oldMY = Mouse::mouse_pos_y;
	}
	
	inline void TestMaterials::RegisterMaterial()
	{
		Material m;
		// Default 
		m = { glm::vec3(1.0f, 0.5f, 0.31f), // ambient
			  glm::vec3(1.0f, 0.5f, 0.31f), // diffuse
			  glm::vec3(0.5f, 0.5f, 0.5f),  // specular
			  32.0f / 128 };			    // shininess

		AddMaterial("Default", m);

		// Esmerald 
		m = { glm::vec3(0.0215f, 0.1745f, 0.0215f),   // ambient
			  glm::vec3(0.07568f, 0.61424f, 0.07568f),// diffuse
			  glm::vec3(0.633f, 0.727811f, 0.633f),   // specular
			  0.6f };								  // shininess

		AddMaterial("Esmerald", m);

		// Obsidian 
		m = { glm::vec3(0.05375f, 0.05f, 0.06625f),		     // ambient
			  glm::vec3(0.18275f, 0.17f, 0.22525f),			 // diffuse
			  glm::vec3(0.332741f, 0.328634f, 0.346435f),    // specular
			  0.3f };										 // shininess

		AddMaterial("Obsidian", m);

		// Pearl 
		m = { glm::vec3(0.25f, 0.20725f, 0.20725f),		     // ambient
			  glm::vec3(1.0f, 0.829f, 0.829f),				 // diffuse
			  glm::vec3(0.296648f, 0.296648f, 0.296648f),    // specular
			  0.088f };										 // shininess

		AddMaterial("Pearl", m);

		// Gold 
		m = { glm::vec3(0.24725f,0.1995f,0.0745f),		     // ambient
			  glm::vec3(0.75164f,0.60648f,0.22648f),		 // diffuse
			  glm::vec3(0.628281f,0.555802f,0.366065f),		 // specular
			  0.4f };										 // shininess

		AddMaterial("Gold", m);

		// Yellow Plastic 
		m = { glm::vec3(0.0f,0.0f,0.0f),		 // ambient
			  glm::vec3(0.5f,0.5f,0.0f),		 // diffuse
			  glm::vec3(0.60f,0.60f,0.50f),		 // specular
			  0.25f };							 // shininess

		AddMaterial("Yellow Plastic", m);

		// Cyan Plastic 
		m = { glm::vec3(0.0f,0.1f,0.06f),						// ambient
			  glm::vec3(0.0f,0.50980392f,0.50980392f),			// diffuse
			  glm::vec3(0.50196078f,0.50196078f,0.50196078f),	// specular
			  0.25f };											// shininess

		AddMaterial("Cyan Plastic", m);

		// Yellow Rubber 
		m = { glm::vec3(0.05f,0.05f,0.0f),	// ambient
			  glm::vec3(0.5f,0.5f,0.4f),	// diffuse
			  glm::vec3(0.7f,0.7f,0.04f),	// specular
			  0.078125f };					// shininess

		AddMaterial("Yellow Rubber", m);
	}

	inline void TestMaterials::AddMaterial(const std::string& nameMaterial, Material material)
	{
		m_Materials.push_back(std::make_pair(nameMaterial, material));
	}

	inline Material* TestMaterials::LocateMaterial(const std::string& nameMaterial)
	{
		for (auto& m : m_Materials)
		{
			if (m.first == nameMaterial)
			{
				return &m.second;
			}
		}
		return LocateMaterial("Default");
	}
}