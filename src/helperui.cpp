#include "helperui.h"
#include <fmt/core.h>

ImVec2 operator+(const ImVec2& a, const ImVec2& b)
{
  return ImVec2(a.x + b.x, a.y + b.y);
}

void RowStatRank::render(const ImVec2& start_pos)
{
  ImGui::SetCursorPos(start_pos);
  ImGui::Text("%s", this->row_name.c_str());

  ImGui::SetCursorPos(start_pos + ImVec2(110, -2));
  ImGui::PushItemWidth(36);
  ImGui::InputText(id1.c_str(), this->number_columns, IM_ARRAYSIZE(this->number_columns), ImGuiInputTextFlags_ReadOnly);
  ImGui::PopItemWidth();

  ImGui::SetCursorPos(start_pos + ImVec2(150, -2));
  ImGui::PushItemWidth(60);
  ImGui::InputText(id2.c_str(), this->percent_rows, IM_ARRAYSIZE(percent_rows), ImGuiInputTextFlags_ReadOnly);
  ImGui::PopItemWidth();
}

void update_progress_bar(const std::shared_ptr<CsvManager>& _csv, 
                         const double& file_size, 
                         float& _progress, 
                         bool& _show_progress)
{
    _progress = 0.0f;
    spdlog::debug("File size: {}", file_size);
    while(_show_progress) {
      _progress = _csv->get_position() / file_size;
      std::this_thread::sleep_for(50ms);
    }
}

void render_table(const Row& header, const Row& r)
{ 
  uint16_t cols = std::max(r.col_count, header.col_count);

  static ImGuiTableFlags flags = 
    ImGuiTableFlags_Borders 
    | ImGuiTableFlags_RowBg
    | ImGuiTableFlags_BordersH
    | ImGuiTableFlags_BordersV
    | ImGuiTableFlags_ScrollX
    | ImGuiTableFlags_Resizable;

  ImGui::BeginTable("##table_row", cols + 1, flags);

  ImGui::TableNextColumn();
  ImGui::Text("   ");

  for (int i=0; i<cols; i++)
  {
    ImGui::TableNextColumn();
    if (i < header.fields.size())
      ImGui::TableHeader(fmt::format("{:d}: {:s}", i, header.fields[i].str).c_str());
    else
      ImGui::TableHeader("   ");
  }
  
  ImGui::TableNextColumn();
  ImGui::Text("string");

  for (int i=0; i<cols; i++)
  {
    ImGui::TableNextColumn();
    if (i < r.fields.size())
      ImGui::Text("%s", r.fields[i].str.c_str());
    else
      ImGui::Text("   ");
  }

  ImGui::TableNextColumn();
  ImGui::Text("hex");

  for (int i=0; i<cols; i++)
  {
    ImGui::TableNextColumn();
    if (i < r.fields.size())
      ImGui::Text("%s", r.fields[i].hex().c_str());
    else
      ImGui::Text("   ");
  }

  ImGui::TableNextColumn();
  ImGui::Text("char count");

  for (int i=0; i<cols; i++)
  {
    ImGui::TableNextColumn();
    if (i < r.fields.size())
      ImGui::Text("%d", r.fields[i].char_count);
    else
      ImGui::Text("   ");
  }

  ImGui::TableNextColumn();
  ImGui::Text("type");

  for (int i=0; i<cols; i++)
  {
    ImGui::TableNextColumn();
    if (i < r.fields.size())
      ImGui::Text("%s", r.fields[i].stype_str().c_str());
    else
      ImGui::Text("   ");
  }

  ImGui::EndTable();
}