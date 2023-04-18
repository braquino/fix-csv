#include "baseui.h"
#include "uimanager.h"
#include "logsinkui.h"
#include "opengl_glfw_ui.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"


int main(int, char**)
{   
    auto log = std::make_shared<LogAppImGui>();
    auto ui_log_sink = std::make_shared<LogSinkUi_mt>(log);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();

    auto logger = std::make_shared<spdlog::logger>("logger");
    logger->sinks().push_back(console_sink);
    logger->sinks().push_back(ui_log_sink);
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);

    std::unique_ptr<BaseUI> ui = std::make_unique<OpenglGlfwUI>(1280, 820, "fix-csv");

    auto manager = std::make_unique<UiManager>();

    ImGui::StyleColorsLight();
    auto& style = ImGui::GetStyle();
    style.FrameBorderSize = 1;
    style.FrameRounding = 3;
    style.WindowRounding = 3;
    ui->io->ConfigWindowsMoveFromTitleBarOnly = true;

    ui->start_loop([&ui, &manager, &log]{
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

            ImGui::SetCursorPos(ImVec2(57,28));
            ImGui::PushItemWidth(120);
            ImGui::InputText("##5", manager->num_rows, IM_ARRAYSIZE(manager->num_rows), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(15,62));
            ImGui::Text("Fields Count:");

            for (int i=0; i<3; i++)
                manager->col_rank[i].render(ImVec2(40, 83 + i * 22));

            ImGui::SetCursorPos(ImVec2(190,30));
            if (ImGui::Button("Calculate\nStatistics", ImVec2(100, 33)))
                manager->on_click_calculate_statistics();

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
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + ImGui::GetWindowWidth() - 20);
            ImGui::InputTextMultiline("##raw_row", manager->raw_row, IM_ARRAYSIZE(manager->raw_row), ImVec2(ImGui::GetWindowWidth() - 20, 100));
            ImGui::PopTextWrapPos();
            ImGui::Spacing();

            if (ImGui::Button("<< Back Row", ImVec2(100, 20)))
                manager->on_click_back_row();
            
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

            if (ImGui::Button("Next Row >>", ImVec2(100, 20)))
                manager->on_click_next_row();

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 65);

            if (ImGui::Button("Update Raw Row", ImVec2(130, 20)))
                manager->on_click_update_raw();

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 241);

            if (ImGui::Button("Save Row", ImVec2(100, 20)))
                manager->on_click_save_row();

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

            if (ImGui::Button("Delete Row", ImVec2(100, 20)))
                manager->on_click_delete_row();

            ImGui::End();
        }

        { // Table Row
            ImGui::Begin("Table Row");
            if (!manager->header.str.empty())
                render_table(manager->header, manager->row, manager->stats);
            ImGui::End();
        }

        { // Find Error
            ImGui::Begin("Find Errors");

            if (ImGui::Button("Find Next Error", ImVec2(120, 30)))
                manager->on_click_next_error();
            ImGui::Spacing();
            ImGui::Checkbox("Bad field count", &manager->f_error_field_count);
            ImGui::Checkbox("Invalid quote", &manager->f_error_bad_quote);
            ImGui::Checkbox("Non printable char", &manager->f_error_non_print_char);
            ImGui::Spacing();
            ImGui::Checkbox("Wrong type in field", &manager->f_error_type);
            ImGui::Combo("Type", &manager->f_err_selected_type, manager->f_err_type_opt_str);
            ImGui::Combo("Field", &manager->f_err_selected_field, manager->f_err_type_opt_fields_str);
            
            ImGui::End();
        }

        { // Logs
            log->Draw("Log");
        }

        { // Progress
            if (manager->show_progress)
            {
                ImGui::Begin("Loading");
                ImGui::ProgressBar(manager->progress);
                ImGui::End();
            }
        }

        ImGui::ShowDemoWindow();
    });

    return 0;
}