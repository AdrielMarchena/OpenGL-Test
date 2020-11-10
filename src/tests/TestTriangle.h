#pragma once
#include "Test.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace test {

	class TestTriangle : public Test
	{
	private:
		float m_Color[4];
		float m_Position[6];
		Shader m_Shader;
		VertexBuffer m_Buffer;
		VertexBufferLayout m_Layout;
		VertexArray m_VertexArray;

	public:
		TestTriangle();
		~TestTriangle();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};

}