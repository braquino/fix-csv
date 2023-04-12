#ifndef HELPERUI_H_
#define HELPERUI_H_

#include "imgui.h"
#include <string>

ImVec2 operator+(const ImVec2& a, const ImVec2& b);

struct RowStatRank 
{
  const std::string row_name;
  char number_columns[9];
  char percent_rows[9];
  const std::string id1{"##1" + this->row_name};
  const std::string id2{"##2" + this->row_name};

  void render(const ImVec2& start_pos);
};


#endif // HELPERUI_H_