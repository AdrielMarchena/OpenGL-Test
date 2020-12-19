#pragma once

#include "Test.h"

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

#include <memory>
#include <vector>
#include "Material.h"

namespace test {

	class TestMaterials : public Test
	{
	public:
		TestMaterials();
		~TestMaterials();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

		float m_RotationVelocity = 1.0f;
		bool m_AutoRotate = false;
		float m_clearColor[4] = { 0.84f,0.84f,0.84f,1.0f };
		input::Keyboard keyboard;
		input::Mouse mouse;
		Camera camera;

		glm::vec3 m_LightPosition = { 1.2f, 1.0f, 2.0f };
		glm::vec3 m_LightColor = { 1.0f,1.0f,1.0f };

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;

		std::unique_ptr<VertexArray> m_LightVAO;
		std::unique_ptr<Shader> m_LightShader;
		std::unique_ptr<VertexBuffer> m_LightVertexBuffer;

		Material* m_ActualMaterial;
		std::vector<std::pair<std::string, Material>> m_Materials;

	private:
		inline void input(float deltaTime);
		inline void RegisterMaterial();
		inline void AddMaterial(const std::string& nameMaterial,Material material);
		inline Material* LocateMaterial(const std::string& nameMaterial);
		inline void ChangeMaterial(Material* material);
	};

}