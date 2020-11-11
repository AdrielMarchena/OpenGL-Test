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
		:m_Position{ VERTICES_CUBE },
		 m_Color{ 0.0f,1.0f,1.0f,1.0f },
		m_Angle(20.0f),
		m_Rotation(0.0f),
		m_RotateTime(1.0f),
		m_AutoRot(false),
		m_EnableDepthTest(true),
		m_Shader("res/shader/cube.shader"),
		m_VertexArray(),
		m_VertexBuffer(m_Position,5 * 36 * sizeof(float)),
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
	}

	TestCube::~TestCube()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestCube::OnUpdate(float deltaTime)
	{
		using namespace input;
		static float rot = 1.0f;
		//TODO: Find a way to clear this function after this instance is destroyed
		m_Keyboard.clicked(Keyboard::ky::ARROW_UP, [&]() {
				std::cout << "Arrow Up pressed, printing on TestCube Class" << std::endl;
		});

		if (!m_EnableDepthTest)
		{
			GLCall(glDisable(GL_DEPTH_TEST));
		}
		else
		{
			GLCall(glEnable(GL_DEPTH_TEST));
		}
			
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		m_Shader.SetUniformMat4f("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		m_Shader.SetUniformMat4f("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(m_Angle), glm::vec3(1.0f, 0.3f, 0.5f));
		if (!m_AutoRot)
		{
			model = glm::rotate(model, m_Rotation * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		else
		{
			model = glm::rotate(model, ((float)glfwGetTime() * m_RotateTime) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		}
		m_Shader.SetUniformMat4f("model", model);
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
		if(!m_AutoRot)
			ImGui::SliderFloat("Rotation", &m_Rotation, 0.0f, 20.0f);
		else
			ImGui::SliderFloat("Rotation Time", &m_RotateTime, 0.02f, 3.0f);
		ImGui::SliderFloat("Angle", &m_Angle, 1.0f, 100.0f);
		ImGui::Checkbox("Auto Rotate", &m_AutoRot);
		ImGui::Checkbox("Depth Test", &m_EnableDepthTest);

	}

}