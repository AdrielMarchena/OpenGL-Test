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

#include "input/Keyboard.h"

namespace test {
	class TestCube : public Test
	{
	private:
		float m_VerticesData[180];
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
		TestCube();
		~TestCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		glm::vec3 getViewPosition();

	private:
		inline void inputs();
	};
}
