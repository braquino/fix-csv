#ifndef BASEUI_H_
#define BASEUI_H_

#include "imgui.h"
#include <functional>
#include <string>

class BaseUI
{
public:
    BaseUI(int _width, int _height, std::string _title) : width{_width}, height{_height}, title{_title}
    {
    }
    ~BaseUI()
    {
    }
    virtual void start_loop(std::function<void()> func) = 0;

    ImGuiIO *io;

protected:
    const int width;
    const int height;
    const std::string title;
};

#endif // BASEUI_H_