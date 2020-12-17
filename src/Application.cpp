#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/Test.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"


#include "tests/TestCube.h"
#include "tests/TestClearColor.h"
#include "tests/TestPyramid.h"
#include "tests/TestTriangle.h"
#include "tests/TestTexture2D.h"
#include "tests/TestLighting.h"

std::vector<std::function<void()>> input::Keyboard::clickedFunction = { []() {} };
std::vector<bool> input::Keyboard::pressedKeys = { 0 };
std::vector<bool> input::Keyboard::clickedKeys = { 0 };

input::Keyboard KeyBoard_Global;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    KeyBoard_Global.handleInputs(window, key, scancode, action, mods);
    /*if(key == GLFW_KEY_F11)
        glfwMaximizeWindow(window);*/
}

float lastX = 960.0f / 2.0f;
float lastY = 540.0f / 2.0f;
bool firstMouse = true;
double input::Mouse::mouse_pos_x = 0.0;
double input::Mouse::mouse_pos_y = 0.0;
input::Mouse Mouse_Global;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xpox = xpos - lastX;
    float ypox = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    Mouse_Global.handleInput(xpox, ypox);
}

void lowerCase(std::string& w)
{

    for (int i = 0; i < w.size(); i++)
        w[i] = std::tolower(w[i]);

}

int main(void)
{
   
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Test OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    //An additional scope, to delete some things before call glfwTerminate();
    {
        //GLCall(glEnable(GL_DEPTH_TEST));
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //Callback func to keyboard events
        glfwSetKeyCallback(window, key_callback);
        glfwSetCursorPosCallback(window, cursor_position_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        Renderer renderer;

        IMGUI_CHECKVERSION();
        ImGuiContext* ctx = ImGui::CreateContext();
        ImGui::SetCurrentContext(ctx);
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui::StyleColorsDark();

        ImGui_ImplOpenGL3_Init((char *)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<test::TestLighting>("Light Test");
        testMenu->RegisterTest<test::TestCube>("Cube Test");
        testMenu->RegisterTest<test::TestPyramid>("Pyramid Test");
        testMenu->RegisterTest<test::TestTriangle>("Triangle Test");
        testMenu->RegisterTest<test::TestTexture2D>("Texture2D Test");
        testMenu->RegisterTest<test::TestClearColor>("Clear Color Test");

        float deltaTime = 0.0f;	// time between current frame and last frame
        float lastFrame = 0.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {

            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(deltaTime);
                currentTest->OnRender();
                ImGui::Begin("Test");

                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }

                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        delete currentTest;
        if (currentTest != testMenu)
        {
            delete testMenu;
        }

    }//End of additional scope


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}