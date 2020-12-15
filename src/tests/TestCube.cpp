#include "TestCube.h"

#include "DumbDefine.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"

#include "Renderer.h"

namespace test
{
	TestCube::TestCube()
	{
		float vertices[] = { VERTICES_CUBE };

		// Vertex Buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 5 * 36 * sizeof(float));

		// Shader
		m_Shader = std::make_unique<Shader>("res/shader/cube.shader");
		m_Shader->Bind();

		//Layout
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		// Vertex Array
		m_VAO = std::make_unique<VertexArray>();
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		// Texture
		m_Texture = std::make_unique<Texture>("res/textures/texture.png");
		m_Texture->Bind();
		m_Shader->SetUniform1i("texture", 0);
		GLCall(glEnable(GL_DEPTH_TEST));
	}

	TestCube::~TestCube()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestCube::OnUpdate(float deltaTime)
	{

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

	}

	void TestCube::OnRender()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_VAO->Bind();
		m_Shader->Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

	}

	void TestCube::OnImGuiRender()
	{

		if (m_AutoRotate)
			ImGui::SliderFloat("Rotation Velocity", &m_RotationVelocity, 0.02f, 3.0f);

		ImGui::SliderFloat3("Rotation", &m_Rotation.x, 0.01f, 1.0f);
		ImGui::SliderFloat("Angle", &m_Angle, 1.0f, 100.0f);
		ImGui::SliderFloat("FOV", &m_FOV, 1.0f, 90.0f);
		ImGui::Checkbox("Auto Rotate", &m_AutoRotate);

	}

}