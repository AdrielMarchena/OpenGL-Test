#pragma once

#include "Test.h"

#include "Shader.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "input/Keyboard.h"
#include "input/Mouse.h"

#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <memory>
namespace test {
	class TestPyramid : public Test
	{
	private:
		float m_clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		float m_RotationVelocity = 1.0f;
		bool m_AutoRotate = false;

		input::Keyboard keyboard;
		input::Mouse mouse;
		Camera camera;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<Texture> m_Texture;
	public:
		TestPyramid();
		~TestPyramid();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		inline void input(float deltaTime);
	};
}