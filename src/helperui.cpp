#include "helperui.h"

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