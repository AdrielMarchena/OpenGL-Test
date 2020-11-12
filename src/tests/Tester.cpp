#include "Tester.h"

#include "imgui/imgui.h"
#include "TestClearColor.h"
#include "TestCube.h"
#include "TestTriangle.h"
#include "TestPyramid.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace test {

	Tester::Tester()
		:m_ActualTest(nullptr)
	{
	}

	Tester::~Tester()
	{
		delete m_ActualTest;
	}

	void Tester::OnUpdate(float deltaTime)
	{
		if (m_CubeSelected)
		{
			if (m_ActualTest != nullptr)
				delete m_ActualTest;
			m_ActualTest = new TestCube();
			m_CubeSelected = false;
		}
		if (m_TriangleSelected)
		{
			if (m_ActualTest != nullptr)
				delete m_ActualTest;
			m_ActualTest = new TestTriangle();
			m_TriangleSelected = false;
		}
		if (m_ClearColorSelected)
		{
			if (m_ActualTest != nullptr)
				delete m_ActualTest;
			m_ActualTest = new TestClearColor();
			m_ClearColorSelected = false;
		}
		if(m_PyramidSelected)
		{
			if (m_ActualTest != nullptr)
				delete m_ActualTest;
			m_ActualTest = new TestPyramid();
			m_PyramidSelected = false;
		}
	}

	inline void Tester::showMenu()
	{
		ImGui::MenuItem("Cube test", NULL, &m_CubeSelected);
		ImGui::MenuItem("Triangle test", NULL, &m_TriangleSelected);
		ImGui::MenuItem("Clear Color test", NULL, &m_ClearColorSelected);
		ImGui::MenuItem("Pyramid", NULL, &m_PyramidSelected);
	}

	void Tester::OnRender()
	{
		if (m_ActualTest == nullptr)
			return;
		
		m_ActualTest->OnUpdate(NULL);
		m_ActualTest->OnRender();

		//ImGui_ImplOpenGL3_NewFrame();
		//ImGui_ImplGlfw_NewFrame();
		//ImGui::NewFrame();

		m_ActualTest->OnImGuiRender();
	
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}

	void Tester::OnImGuiRender()
	{
		//if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				showMenu();
				ImGui::EndMenu();
			}

			//ImGui::EndMenuBar();
		}
	}

	
}