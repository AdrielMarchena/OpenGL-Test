#pragma once
#include "Test.h"

#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace test {
	class TestCube : public Test
	{
	private:

		float m_Position[180];
		float m_Color[4];
		float m_Angle;
		float m_Rotation;

		Shader m_Shader;
		VertexArray m_VertexArray;
		VertexBuffer m_VertexBuffer;
		VertexBufferLayout m_Layout;
		Texture m_Texture;

	public:
		TestCube();
		~TestCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
