#include "Window.h"


Window::Window(int width, int height, std::string titel) : width(width), height(height), titel(titel)
{
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }


    handel = glfwCreateWindow(width, height, titel.c_str(), NULL, NULL);
    if (!handel)
    {
        glfwTerminate();
        std::cerr << "Failed to create Window" << std::endl;
        exit(-1);
    }

    glfwMakeContextCurrent(handel);
    glViewport(0, 0, width, height);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(handel);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(handel);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::terminate()
{
    glfwDestroyWindow(handel);
    glfwTerminate();
}

