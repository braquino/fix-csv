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
  ImGui::PushItemWidth(90);
  ImGui::InputText(id2.c_str(), this->percent_rows, IM_ARRAYSIZE(percent_rows), ImGuiInputTextFlags_ReadOnly);
  ImGui::PopItemWidth();
}

void update_progress_bar(const std::shared_ptr<CsvManager> _csv, 
                         float& _progress, 
                         bool& _show_progress)
{
  double file_size = (double)_csv->get_size();
  _progress = 0.0f;
  spdlog::debug("File size: {}", file_size);
  while(_show_progress) {
    _progress = _csv->get_position() / file_size;
    std::this_thread::sleep_for(50ms);
  }
}

void render_table(const Row& header, const Row& r, const std::shared_ptr<RowsReport>& stats)
{ 
  uint16_t cols = std::max(r.col_count, header.col_count);

  static ImGuiTableFlags flags = 
    ImGuiTableFlags_Borders 
    | ImGuiTableFlags_RowBg
    | ImGuiTableFlags_BordersH
    | ImGuiTableFlags_BordersV
    | ImGuiTableFlags_ScrollX
    | ImGuiTableFlags_Resizable
    | ImGuiTableFlags_NoSavedSettings;

  ImGui::BeginTable("##table_row", cols + 1, flags);

  ImGui::TableSetupColumn("   ", ImGuiTableColumnFlags_WidthFixed, 80.0f);

  for (int i=0; i<cols; i++)
  {
    if (i < header.fields.size())
      ImGui::TableSetupColumn(
        fmt::format("{:d}: {:s}", i, header.fields[i].str).c_str(),
        ImGuiTableColumnFlags_WidthFixed, 120.0f
      );
    else
      ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthFixed, 120.0f);
  }
  ImGui::TableHeadersRow();

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

  if (!stats->field_statistic.empty())
  {
    ImGui::TableNextColumn();
    ImGui::Text("Statistics");

    for (int i=0; i<cols; i++)
    {
      ImGui::TableNextColumn();
      ImGui::Text("  ");
    }

    ImGui::TableNextColumn();
    ImGui::Text("1st type");

    for (int i=0; i<cols; i++)
    {
      ImGui::TableNextColumn();
      if (i < stats->field_statistic.size())
        ImGui::Text(
          "%s: %.2f%%", 
          Field::stype_to_string(stats->field_statistic[i].type_first).c_str(),
          stats->field_statistic[i].perc_type_first * 100
        );
      else
        ImGui::Text("   ");
    }

    ImGui::TableNextColumn();
    ImGui::Text("2nd type");

    for (int i=0; i<cols; i++)
    {
      ImGui::TableNextColumn();
      if (i < stats->field_statistic.size())
        ImGui::Text(
          "%s: %.2f%%", 
          Field::stype_to_string(stats->field_statistic[i].type_second).c_str(),
          stats->field_statistic[i].perc_type_second * 100
        );
      else
        ImGui::Text("   ");
    } 

    ImGui::TableNextColumn();
    ImGui::Text("3rd type");

    for (int i=0; i<cols; i++)
    {
      ImGui::TableNextColumn();
      if (i < stats->field_statistic.size())
        ImGui::Text(
          "%s: %.2f%%", 
          Field::stype_to_string(stats->field_statistic[i].type_third).c_str(),
          stats->field_statistic[i].perc_type_third * 100
        );
      else
        ImGui::Text("   ");
    }  
  }

  ImGui::EndTable();
}