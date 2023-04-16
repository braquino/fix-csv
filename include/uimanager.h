#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "csvmanager.h"
#include "csvstatistics.h"
#include "helperui.h"

class UiManager {
public:
  UiManager();

  char in_filename[512] = "";
  char out_filename[512] = "";
  char sep[2] = ",";
  char quote[2] = "\"";
  char curr_row[9] = "";
  char header_count[9] = "";
  char curr_row_count[9] = "";
  char raw_row[3000] = "";
  char field_count_err[2] = "";
  RowStatRank col_rank[3] = {
    {"1st appearance", "", ""}, 
    {"2nd appearance", "", ""},
    {"3rd appearance", "", ""}
  };
  char num_rows[9] = "";
  float progress = 0.0f;
  bool show_progress = false;

  bool f_error_field_count = true;
  bool f_error_bad_quote = true;
  bool f_error_non_print_char = false;
  bool f_error_type = false;
  std::vector<SimpleType> f_err_type_opt = {SimpleType::EMPTY, SimpleType::STRING, SimpleType::NUMBER, SimpleType::INTEGER};
  char f_err_type_opt_str[500] = "\0\0";
  int f_err_selected_type = 0;
  char f_err_type_opt_fields_str[2000] = "\0\0";
  int f_err_selected_field = 0;

  Row row{};
  Row header{};
  std::shared_ptr<RowsReport> stats;

  void on_click_open_file();
  void on_click_close_file();
  void on_click_save_file();
  void on_click_reset_file();
  void on_click_statistics();
  void on_click_next_row();
  void on_click_back_row();
  void on_click_delete_row();
  void on_click_save_row();
  void on_click_next_error();
  void on_click_update_raw();
  void on_click_calculate_statistics();
  void open();

private:
  std::shared_ptr<CsvManager> csv;

  void reset();
  void row_change(const Row& r);
  void fill_statistic();
};

#endif // UIMANAGER_H_