#include "TestLighting.h"

#include "DumbDefine.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Renderer.h"

namespace test
{
	TestLighting::TestLighting()
		:camera(glm::vec3(0.0f,0.0f,3.0f))
	{
		float vertices[] = { VERTICES_LIGHT_CUBE };

		// Vertex Buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 6 * 36 * sizeof(float));

		// Shader
		m_Shader = std::make_unique<Shader>("res/shader/lightedCube.shader");
		m_Shader->Bind();

		// Cube uniforms
		m_Shader->SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
		m_Shader->SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
		m_Shader->SetUniform3f("lightPos", m_LightPosition.x, m_LightPosition.y, m_LightPosition.z);
		m_Shader->SetUniform3f("viewPos", camera.Position.x,camera.Position.y,camera.Position.z);
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

		VertexBufferLayout lightLayout;
		lightLayout.Push<float>(3);
		lightLayout.Push<float>(3);

		m_LightVAO = std::make_unique<VertexArray>();
		m_LightVAO->AddBuffer(*m_LightVertexBuffer, lightLayout);

		GLCall(glEnable(GL_DEPTH_TEST));
	}

	TestLighting::~TestLighting()
	{
		GLFWwindow* window = glfwGetCurrentContext();
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL));
	}

	void test::TestLighting::OnUpdate(float deltaTime)
	{
		input(deltaTime);
		m_Shader->Bind();

		// Cube uniforms
		m_Shader->SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
		m_Shader->SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
		m_Shader->SetUniform3f("lightPos", m_LightPosition.x, m_LightPosition.y, m_LightPosition.z);
		m_Shader->SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		glm::mat4 view = camera.GetViewMatrix();
		m_Shader->SetUniformMat4f("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), 940.0f / 540.0f, 0.1f, 100.0f);
		m_Shader->SetUniformMat4f("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		if (!m_AutoRotate)
		{
			model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else
		{
			model = glm::rotate(model, ((float)glfwGetTime() * m_RotationVelocity) * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		}
		m_Shader->SetUniformMat4f("model", model);

		// ------------------------------------
		// Light Cube rendering
		m_LightShader->Bind();

		m_LightShader->SetUniformMat4f("view", view);
		m_LightShader->SetUniformMat4f("projection", projection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPosition);
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		if (!m_AutoRotate)
		{
			model = glm::rotate(model, glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		}
		else
		{
			model = glm::rotate(model, ((float)glfwGetTime() * m_RotationVelocity) * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		}
		m_LightShader->SetUniformMat4f("model", model);
	}

	void TestLighting::OnRender()
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

	void TestLighting::OnImGuiRender()
	{
		if (m_AutoRotate)
			ImGui::SliderFloat("Rotation Velocity", &m_RotationVelocity, 0.02f, 3.0f);
		ImGui::SliderFloat("FOV", &camera.Zoom, 0.01f, 90.0f);
		ImGui::SliderFloat3("Light Cube Pos", &m_LightPosition.x, -3.0f, 3.0f);
		ImGui::ColorEdit4("Background Color", m_clearColor);
		ImGui::Checkbox("Auto Rotate", &m_AutoRotate);

	}

	inline void TestLighting::input(float deltaTime)
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
}