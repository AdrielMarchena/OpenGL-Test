#pragma once

#include "Test.h"

#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "input/Keyboard.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace test {
	class TestPyramid : public Test
	{
	private:
		float m_VerticesData[72];
		float m_Color[4];
		float m_Angle;
		float m_RotateTime;
		float m_PerspectiveRadians;
		bool m_AutoRot;
		bool m_EnableDepthTest;

		glm::vec3 m_Rotation;

		glm::vec3 m_Position;
		glm::vec3 m_CameraVelocity;

		Shader m_Shader;
		VertexArray m_VertexArray;
		VertexBuffer m_VertexBuffer;
		VertexBufferLayout m_Layout;
		Texture m_Texture;
		input::Keyboard m_Keyboard;
	public:
		TestPyramid();
		~TestPyramid();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		inline void inputs();
	};
}