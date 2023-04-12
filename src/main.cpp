#include "baseui.h"
#include "opengl_glfw_ui.h"
#include "helperui.h"
#include "tinyfiledialogs/tinyfiledialogs.h"

int main(int, char**)
{
    std::unique_ptr<BaseUI> ui = std::make_unique<OpenglGlfwUI>(1280, 720, "fix-csv");

    char in_filename[512] = "";
    char out_filename[512] = "";
    char sep[2] = ",";
    char quote[2] = "\"";
    char curr_row[9] = "";
    char header_count[9] = "";
    char curr_row_count[9] = "";
    char raw_row[3000] = "";

    RowStatRank col_rank[] = {
        {"1st appearance", "84", "20.54%"}, 
        {"2nd appearance", "85", "5.56%"},
        {"3rd appearance", "21", "0.2%"}
    };

    ui->start_loop([&in_filename, &out_filename, &sep, &header_count, &curr_row_count,
                    &quote, &col_rank, &ui, &curr_row, &raw_row]{
        static float f = 0.0f;
        static int counter = 0;

        { // File Window
            ImGui::Begin("File"); 
            
            ImGui::SetCursorPos(ImVec2(15,35));
            if (ImGui::Button("Open File", ImVec2(110,20)))
            {
                strcat(in_filename, tinyfd_openFileDialog("Open File", NULL, 0, NULL, NULL, 0));
                strcat(out_filename, in_filename);
                strncat(out_filename, ".out", strlen(in_filename));
            }

            ImGui::SetCursorPos(ImVec2(15,65));
            ImGui::Button("Close File", ImVec2(110,20));

            ImGui::SetCursorPos(ImVec2(230,35));
            ImGui::PushItemWidth(500);
            ImGui::InputText("##1", in_filename, IM_ARRAYSIZE(in_filename), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(152,39));
            ImGui::Text("File name:");

            ImGui::SetCursorPos(ImVec2(230,64));
            ImGui::PushItemWidth(500);
            
            ImGui::InputText("##2", out_filename, IM_ARRAYSIZE(out_filename));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(139,68));
            ImGui::Text("Output path:");

            ImGui::SetCursorPos(ImVec2(616.5,102));
            ImGui::Button("Save File", ImVec2(110,20));

            ImGui::SetCursorPos(ImVec2(18,104));
            ImGui::Text("Separator:");

            ImGui::SetCursorPos(ImVec2(93,102));
            ImGui::PushItemWidth(17);
            
            ImGui::InputText("##3", sep, IM_ARRAYSIZE(sep));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(155,104));
            ImGui::Text("Escape char:");

            ImGui::SetCursorPos(ImVec2(245,102));
            ImGui::PushItemWidth(17);
            
            ImGui::InputText("##4", quote, IM_ARRAYSIZE(quote));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(329,102));
            ImGui::Button("Reset File", ImVec2(110,19));
            
            ImGui::End();
        }

        { // Row Statistics
            ImGui::Begin("Row Statistics");

            ImGui::SetCursorPos(ImVec2(15,30));
            ImGui::Text("Rows:");

            ImGui::SetCursorPos(ImVec2(64,28));
            ImGui::PushItemWidth(131);
            static char str6[128] = "4345";
            ImGui::InputText("##5", str6, IM_ARRAYSIZE(str6));
            ImGui::PopItemWidth();

            ImGui::SetCursorPos(ImVec2(15,62));
            ImGui::Text("Fields Count:");

            for (int i=0; i<3; i++)
                col_rank[i].render(ImVec2(40, 83 + i * 22));

            ImGui::End();
        }
    
        { // Raw Row
            ImGui::Begin("Raw Row");

            ImGui::Text("Current Row: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(60);
            ImGui::InputText("##cr", curr_row, IM_ARRAYSIZE(curr_row),ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

            ImGui::Text("Field in Header: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(40);
            ImGui::InputText("##fih", header_count, IM_ARRAYSIZE(header_count),ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            ImGui::SameLine(); ImGui::Spacing(); ImGui::SameLine();

            ImGui::Text("Fields in this Row: ");
            ImGui::SameLine();
            ImGui::PushItemWidth(40);
            ImGui::InputText("##fitr", curr_row_count, IM_ARRAYSIZE(curr_row_count),ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();

            ImGui::Spacing();
            ImGui::InputTextMultiline("##raw_row", raw_row, IM_ARRAYSIZE(raw_row), ImVec2(ImGui::GetWindowWidth() - 20, 100));

            ImGui::End();
        }
    });

    return 0;
}