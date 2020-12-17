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

		input();
		glm::mat4 view = camera.GetViewMatrix();
		m_Shader->SetUniformMat4f("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), 940.0f / 540.0f, 0.1f, 100.0f);
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
		GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
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
		ImGui::SliderFloat("FOV", &camera.Zoom, 1.0f, 90.0f);
		ImGui::ColorEdit4("Background Color", m_clearColor);
		ImGui::Checkbox("Auto Rotate", &m_AutoRotate);

	}

	inline void TestCube::input()
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
			camera.ProcessKeyboard(Camera_Movement::LEFT, m_CameraVelocity.x);
		}
		if (keyboard.isPress(Keyboard::ky::D_KEY))
		{
			camera.ProcessKeyboard(Camera_Movement::RIGHT, m_CameraVelocity.x);
		}
		//Z
		if (keyboard.isPress(Keyboard::ky::W_KEY))
		{
			camera.ProcessKeyboard(Camera_Movement::FORWARD, m_CameraVelocity.z);
		}
		if (keyboard.isPress(Keyboard::ky::S_KEY))
		{
			camera.ProcessKeyboard(Camera_Movement::BACKWARD, m_CameraVelocity.z);
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