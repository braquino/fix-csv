#ifndef OPENGL_GLFW_UI_H_
#define OPENGL_GLFW_UI_H_

#include "baseui.h"
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <memory>

class OpenglGlfwUI : public BaseUI
{
public:
    OpenglGlfwUI(int _width, int _height, std::string _title);
    ~OpenglGlfwUI();
    void start_loop(std::function<void()> func) override;

private:
    GLFWwindow *window;
};

#endif // OPENGL_GLFW_UI_H_