#pragma once

#include "Test.h"
namespace test {

	class Tester : public Test
	{
	private:
		Test* m_ActualTest;

		bool m_CubeSelected = false;
		bool m_TriangleSelected = false;
		bool m_ClearColorSelected = false;
		bool m_PyramidSelected = false;
	public:
		Tester();
		~Tester();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		inline void showMenu();
	};

}