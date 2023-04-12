#include "baseui.h"
#include "uimanager.h"
#include "opengl_glfw_ui.h"
#include "spdlog/spdlog.h"


int main(int, char**)
{   
    spdlog::set_level(spdlog::level::debug);

    std::unique_ptr<BaseUI> ui = std::make_unique<OpenglGlfwUI>(1280, 720, "fix-csv");

    auto manager = std::make_unique<UiManager>();

    ui->start_loop([&ui, &manager]{
        static float f = 0.0f;
        static int counter = 0;

        { // File Window
            ImGui::Begin("File"); 
            
            ImGui::SetCursorPos(ImVec2(15,35));
            if (ImGui::Button("Open File", ImVec2(110,20)))
                manager->on_click_open_file();

            ImGui::SetCursorPos(ImVec2(15,65));
            if (ImGui::Button("Close File", ImVec2(110,20)))
                manager->on_click_close_file();

            ImGui::SetCursorPos(ImVec2(230,35));
            ImGui::PushItemWidth(500);
            ImGui::InputText("##1", manager->in_filename, IM_ARRAYSIZE(manager->in_filename), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(152,39));
            ImGui::Text("File name:");

            ImGui::SetCursorPos(ImVec2(230,64));
            ImGui::PushItemWidth(500);
            
            ImGui::InputText("##2", manager->out_filename, IM_ARRAYSIZE(manager->out_filename));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(139,68));
            ImGui::Text("Output path:");

            ImGui::SetCursorPos(ImVec2(616.5,102));
            if (ImGui::Button("Save File", ImVec2(110,20)))
                manager->on_click_save_file();

            ImGui::SetCursorPos(ImVec2(18,104));
            ImGui::Text("Separator:");

            ImGui::SetCursorPos(ImVec2(93,102));
            ImGui::PushItemWidth(17);
            
            ImGui::InputText("##3", manager->sep, IM_ARRAYSIZE(manager->sep));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(155,104));
            ImGui::Text("Escape char:");

            ImGui::SetCursorPos(ImVec2(245,102));
            ImGui::PushItemWidth(17);
            
            ImGui::InputText("##4", manager->quote, IM_ARRAYSIZE(manager->quote));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(329,102));
            if (ImGui::Button("Reset File", ImVec2(110,19)))
                manager->on_click_reset_file();
            
            ImGui::End();
        }

        { // Row Statistics
            ImGui::Begin("Row Statistics");

            ImGui::SetCursorPos(ImVec2(15,30));
            ImGui::Text("Rows:");

            ImGui::SetCursorPos(ImVec2(64,28));
            ImGui::PushItemWidth(131);
            ImGui::InputText("##5", manager->num_rows, IM_ARRAYSIZE(manager->num_rows));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(15,62));
            ImGui::Text("Fields Count:");

            for (int i=0; i<3; i++)
                manager->col_rank[i].render(ImVec2(40, 83 + i * 22));

            ImGui::End();
        }
    
        { // Raw Row
            ImGui::Begin("Raw Row");

            ImGui::Text("Current Row: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(60);
            ImGui::InputText("##cr", manager->curr_row, IM_ARRAYSIZE(manager->curr_row),ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

            ImGui::Text("Field in Header: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(40);
            ImGui::InputText("##fih", manager->header_count, IM_ARRAYSIZE(manager->header_count),ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

            ImGui::Text("Fields in this Row: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(40);
            ImGui::InputText("##fitr", manager->curr_row_count, IM_ARRAYSIZE(manager->curr_row_count),ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();

            ImGui::Spacing();
            ImGui::InputTextMultiline("##raw_row", manager->raw_row, IM_ARRAYSIZE(manager->raw_row), ImVec2(ImGui::GetWindowWidth() - 20, 100));
            ImGui::Spacing();

            if (ImGui::Button("<< Back Row", ImVec2(100, 20)))
                manager->on_click_back_row();
            
            ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

            if (ImGui::Button("Next Row >>", ImVec2(100, 20)))
                manager->on_click_next_row();
            
            ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

            if (ImGui::Button("Delete Row", ImVec2(100, 20)))
                manager->on_click_delete_row();

            ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

            if (ImGui::Button("↑↑ Update Raw ↑↑", ImVec2(100, 20)))
                manager->on_click_update_raw();

            ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

            if (ImGui::Button("Next Error >>", ImVec2(100, 20)))
                manager->on_click_next_error();

            ImGui::End();
        }
    });

    return 0;
}