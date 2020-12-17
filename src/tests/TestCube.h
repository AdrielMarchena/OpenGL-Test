#pragma once
#include "Test.h"

#include <memory>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace test
{
	class TestCube : public Test
	{
	public:
		TestCube();
		~TestCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		float m_FOV = 45.0f;
		float m_Angle = 20.0f;
		float m_RotationVelocity = 1.0f;
		bool m_AutoRotate = false;
		float m_clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

		input::Keyboard keyboard;
		input::Mouse mouse;
		Camera camera;

		glm::vec3 m_Rotation = { 1.0f,1.0f,1.0f };

		glm::vec3 m_Position = { 0.0f, 0.0f, -3.0f };

		glm::vec3 m_CameraVelocity = { 0.02f,0.02f,0.02f };

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Texture> m_Texture;
	private:
		inline void input();
	};
}

