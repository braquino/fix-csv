#ifndef UIMANAGER_H_
#define UIMANAGER_H_

#include "csvmanager.h"
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

private:
  std::unique_ptr<CsvManager> csv;

  void reset();
  void row_change(const Row& r);
};

#endif // UIMANAGER_H_