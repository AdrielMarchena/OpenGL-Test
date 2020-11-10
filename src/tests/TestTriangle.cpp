#include "TestTriangle.h"

#include "Renderer.h"
#include "imgui/imgui.h"
namespace test {
	TestTriangle::TestTriangle()
		:m_Color{ 0.0f,1.0f,1.0f,1.0f },
		 m_Position{
			-0.5f,-0.5f,
			 0.5f,-0.5f,
			 0.5f,0.5f
		},
		m_VertexArray(),
		m_Buffer(m_Position, 3 * 2 * sizeof(float)),
		m_Layout(),
		m_Shader("res/shader/Uniform.shader")
	{
		m_Layout.Push<float>(2);
		m_VertexArray.AddBuffer(m_Buffer, m_Layout);

		m_Shader.Bind();
		m_Shader.SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	}

	TestTriangle::~TestTriangle()
	{
		m_Shader.Unbind();
	}

	void TestTriangle::OnUpdate(float deltaTime)
	{
		m_Shader.Bind();
		m_Shader.SetUniform4f("u_Color", m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	}

	void TestTriangle::OnRender()
	{
		m_VertexArray.Bind();
		m_Shader.Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	}

	void TestTriangle::OnImGuiRender()
	{
		ImGui::ColorEdit4("Triangle Color: ", m_Color);
	}
}