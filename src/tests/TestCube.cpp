#include "TestCube.h"
#include "imgui/imgui.h"

/*I know this is dumb, sorry :D*/
#define VERTICES {\
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
-0.5f, 0.5f, -0.5f, 0.0f, 1.0f}

namespace test{
	TestCube::TestCube()
		:m_Position VERTICES,
		 m_Color{ 0.0f,1.0f,1.0f,1.0f },
		m_Angle(20.0f),
		m_Rotation(0.0f),
		m_Shader("res/shader/cube.shader"),
		m_VertexArray(),
		m_VertexBuffer(m_Position,5 * 36 * sizeof(float)),
		m_Layout(),
		m_Texture("res/textures/texture.png")
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

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		m_Shader.SetUniformMat4f("view", view);

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		m_Shader.SetUniformMat4f("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(m_Angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::rotate(model, m_Rotation * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		m_Shader.SetUniformMat4f("model", model);
	}

	void TestCube::OnRender()
	{

		GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]))
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_VertexArray.Bind();
		m_Shader.Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	void TestCube::OnImGuiRender()
	{
		ImGui::ColorEdit4("Cube Color", m_Color);
		ImGui::SliderFloat("Rotation", &m_Rotation, 1.0f, 100.0f);
	}

}