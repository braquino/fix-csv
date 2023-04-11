#include "baseui.h"
#include "opengl_glfw_ui.h"

int main(int, char**)
{
    std::unique_ptr<BaseUI> ui = std::make_unique<OpenglGlfwUI>(1280, 720, "fix-csv");

    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ui->start_loop([&show_another_window, &clear_color, &ui]{
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!"); 
            ImGui::Text("This is some useful text.");
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color); 
            if (ImGui::Button("Button"))
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ui->io->Framerate, ui->io->Framerate);
        ImGui::End();

    });

    return 0;
}