#include "TestCube.h"
#include "imgui/imgui.h"
#include "GLFW/glfw3.h"

/*I know this is dumb, sorry :D*/
#define VERTICES_CUBE \
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,\
0.5f, -0.5f, -0.5f, 1.0f, 0.0f,\
0.5f, 0.5f, -0.5f, 1.0f, 1.0f,\
0.5f, 0.5f, -0.5f, 1.0f, 1.0f,\
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,\
-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,\
\
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,\
0.5f, -0.5f, 0.5f, 1.0f, 0.0f,\
0.5f, 0.5f, 0.5f, 1.0f, 1.0f,\
0.5f, 0.5f, 0.5f, 1.0f, 1.0f,\
-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,\
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,\
\
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,\
-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,\
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,\
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,\
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,\
-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,\
\
0.5f, 0.5f, 0.5f, 1.0f, 0.0f,\
0.5f, 0.5f, -0.5f, 1.0f, 1.0f,\
0.5f, -0.5f, -0.5f, 0.0f, 1.0f,\
0.5f, -0.5f, -0.5f, 0.0f, 1.0f,\
0.5f, -0.5f, 0.5f, 0.0f, 0.0f,\
0.5f, 0.5f, 0.5f, 1.0f, 0.0f,\
\
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,\
0.5f, -0.5f, -0.5f, 1.0f, 1.0f,\
0.5f, -0.5f, 0.5f, 1.0f, 0.0f,\
0.5f, -0.5f, 0.5f, 1.0f, 0.0f,\
-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,\
-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,\
\
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,\
0.5f, 0.5f, -0.5f, 1.0f, 1.0f,\
0.5f, 0.5f, 0.5f, 1.0f, 0.0f,\
0.5f, 0.5f, 0.5f, 1.0f, 0.0f,\
-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,\
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f

namespace test{
	TestCube::TestCube()
		:m_VerticesData{ VERTICES_CUBE },
		 m_Color{ 0.0f,1.0f,1.0f,1.0f },
		m_Angle(20.0f),
		m_RotateTime(1.0f),
		m_PerspectiveRadians(45.0f),
		m_AutoRot(false),
		m_EnableDepthTest(true),
		m_Rotation(1.0f, 1.0f, 1.0f),
		m_Position(0.0f, 0.0f, -3.0f),
		m_CameraVelocity(0.02f,0.02f,0.02f),
		m_Shader("res/shader/cube.shader"),
		m_VertexArray(),
		m_VertexBuffer(m_VerticesData,5 * 36 * sizeof(float)),
		m_Layout(),
		m_Texture("res/textures/texture.png")
		//End Init list
	{
		m_Shader.Bind();
		m_Layout.Push<float>(3);
		m_Layout.Push<float>(2);
		m_VertexArray.AddBuffer(m_VertexBuffer, m_Layout);
		m_Texture.Bind();
		m_Shader.SetUniform1i("texture", 0);
		GLCall(glEnable(GL_DEPTH_TEST));

		//This mess clear the console, i don't wanna use system()
		std::cout << "\x1B[2J\x1B[H" << std::endl;
		std::cout << "Arrow keys, 'W' and 'S' keys moves the camera" << std::endl;
	}

	TestCube::~TestCube()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestCube::OnUpdate(float deltaTime)
	{
		inputs();
		if (!m_EnableDepthTest)
		{
			GLCall(glDisable(GL_DEPTH_TEST));
		}
		else
		{
			GLCall(glEnable(GL_DEPTH_TEST));
		}

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, m_Position);
		m_Shader.SetUniformMat4f("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(m_PerspectiveRadians), 940.0f / 540.0f, 0.1f, 100.0f);
		m_Shader.SetUniformMat4f("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(m_Angle), glm::vec3(0.0f, 1.0f, 1.0f));
		if (!m_AutoRot)
		{
			model = glm::rotate(model, glm::radians(50.0f), m_Rotation);
		}
		else
		{
			model = glm::rotate(model, ((float)glfwGetTime() * m_RotateTime) * glm::radians(50.0f), m_Rotation);
		}
		m_Shader.SetUniformMat4f("model", model);
		m_Shader.SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	}

	void TestCube::OnRender()
	{
		if (!m_EnableDepthTest)
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
		}
		else
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		}
		
		m_VertexArray.Bind();
		m_Shader.Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	void TestCube::OnImGuiRender()
	{
		if (m_AutoRot)
			ImGui::SliderFloat("Rotation Time", &m_RotateTime, 0.02f, 3.0f);
		ImGui::SliderFloat3("Rotation", &m_Rotation.x, 0.01f, 1.0f);
		ImGui::SliderFloat("Angle", &m_Angle, 1.0f, 100.0f);
		ImGui::SliderFloat("FOV", &m_PerspectiveRadians, 1.0f, 90.0f);
		ImGui::Checkbox("Auto Rotate", &m_AutoRot);
		ImGui::Checkbox("Depth Test", &m_EnableDepthTest);
		ImGui::SliderFloat3("Camera Velocity", &m_CameraVelocity.x, 0.02f, 1.0f);
		ImGui::ColorEdit4("Clear Color", m_Color);

	}

	glm::vec3 TestCube::getViewPosition()
	{
		return m_Position;
	}

	inline void TestCube::inputs()
	{
		using namespace input;
		
		//Y
		if (m_Keyboard.isPress(Keyboard::ky::ARROW_UP))
		{
			m_Position.y -= m_CameraVelocity.y;
		}
		if (m_Keyboard.isPress(Keyboard::ky::ARROW_DOWN))
		{
			m_Position.y += m_CameraVelocity.y;
		}
		//X
		if (m_Keyboard.isPress(Keyboard::ky::ARROW_LEFT))
		{
			m_Position.x += m_CameraVelocity.x;
		}
		if (m_Keyboard.isPress(Keyboard::ky::ARROW_RIGHT))
		{
			m_Position.x -= m_CameraVelocity.x;
		}
		//Z
		if (m_Keyboard.isPress(Keyboard::ky::W_KEY))
		{
			m_Position.z += m_CameraVelocity.z;
		}
		if (m_Keyboard.isPress(Keyboard::ky::S_KEY))
		{
			m_Position.z -= m_CameraVelocity.z;
		}
	}
}