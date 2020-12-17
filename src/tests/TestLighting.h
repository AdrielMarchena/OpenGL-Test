#pragma once
#include "Test.h"

#include <memory>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "camera.h"

namespace test
{
	class TestLighting : public Test
	{
	public:
		TestLighting();
		~TestLighting();
		
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		float m_FOV = 45.0f;
		float m_Angle = 20.0f;
		float m_RotationVelocity = 1.0f;
		float m_Radians = 50.0f;
		bool m_AutoRotate = false;
		float m_clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
		input::Keyboard keyboard;
		input::Mouse mouse;
		Camera camera;

		glm::vec3 m_Rotation = { 1.0f,1.0f,1.0f };
		glm::vec3 m_Position = { -0.7f, -0.54f, -4.5f };
		
		glm::vec3 m_LightPosition = { 1.2f, 1.0f, 2.0f };


		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		std::unique_ptr<VertexArray> m_LightVAO;
		std::unique_ptr<Shader> m_LightShader;
		std::unique_ptr<VertexBuffer> m_LightVertexBuffer;

		glm::vec3 m_CameraVelocity = { 0.02f,0.02f,0.02f };

	private:
		inline void input();
	};
}

