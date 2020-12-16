#include "TestLighting.h"

#include "DumbDefine.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Renderer.h"

namespace test
{
	TestLighting::TestLighting()
	{
		float vertices[] = { VERTICES_NO_TEXTURE_CUBE };

		// Vertex Buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 5 * 36 * sizeof(float));

		// Shader
		m_Shader = std::make_unique<Shader>("res/shader/lightedCube.shader");
		m_Shader->Bind();

		// Cube uniforms
		m_Shader->SetUniform3f("objectColor", 1.0f, 0.5f, 0.31f);
		m_Shader->SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);

		//Layout
		VertexBufferLayout cubeLayout;
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

		m_LightVAO = std::make_unique<VertexArray>();
		m_LightVAO->AddBuffer(*m_LightVertexBuffer, lightLayout);

	}

	TestLighting::~TestLighting()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void test::TestLighting::OnUpdate(float deltaTime)
	{
		input();
		m_Shader->Bind();

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, m_Position);
		m_Shader->SetUniformMat4f("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(m_FOV), 940.0f / 540.0f, 0.1f, 100.0f);
		m_Shader->SetUniformMat4f("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(m_Angle), glm::vec3(0.0f, 1.0f, 1.0f));
		if (!m_AutoRotate)
		{
			model = glm::rotate(model, glm::radians(50.0f), m_Rotation);
		}
		else
		{
			model = glm::rotate(model, ((float)glfwGetTime() * m_RotationVelocity) * glm::radians(50.0f), m_Rotation);
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
		m_LightShader->SetUniformMat4f("model", model);

	}

	void TestLighting::OnRender()
	{
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
		ImGui::SliderFloat("Angle", &m_Angle, 1.0f, 100.0f);
		ImGui::SliderFloat("FOV", &m_FOV, 0.01f, 360.0f);
		ImGui::SliderFloat3("Rotation", &m_Rotation.x, 0.01f, 1.0f);
		ImGui::SliderFloat3("Camera Velocity", &m_CameraVelocity.x, 0.02f, 0.5f);
		ImGui::Checkbox("Auto Rotate", &m_AutoRotate);
	}

	inline void TestLighting::input()
	{
		using namespace input;

		//Y
		if (keyboard.isPress(Keyboard::ky::ARROW_UP))
		{
			m_Position.y -= m_CameraVelocity.y;
		}
		if (keyboard.isPress(Keyboard::ky::ARROW_DOWN))
		{
			m_Position.y += m_CameraVelocity.y;
		}
		//X
		if (keyboard.isPress(Keyboard::ky::ARROW_LEFT))
		{
			m_Position.x += m_CameraVelocity.x;
		}
		if (keyboard.isPress(Keyboard::ky::ARROW_RIGHT))
		{
			m_Position.x -= m_CameraVelocity.x;
		}
		//Z
		if (keyboard.isPress(Keyboard::ky::W_KEY))
		{
			m_Position.z += m_CameraVelocity.z;
		}
		if (keyboard.isPress(Keyboard::ky::S_KEY))
		{
			m_Position.z -= m_CameraVelocity.z;
		}
		keyboard.clicked(Keyboard::ky::ESC_KEY, [&]() {
			std::cout << "Position X: " << m_Position.x << std::endl;
			std::cout << "Position Y: " << m_Position.y << std::endl;
			std::cout << "Position Z: " << m_Position.z << std::endl;
		});
	}

}