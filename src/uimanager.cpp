#include "uimanager.h"
#include "spdlog/spdlog.h"
#include "tinyfiledialogs/tinyfiledialogs.h"
#include "fmt/core.h"
#include <future>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void fill_option_str(const std::vector<std::string>& list, char* str)
{
  int char_count = 0;
  for (const auto& opt : list)
  {
    for (auto c : opt)
    {
      str[char_count] = c;
      char_count++;
    }
    str[char_count] = 0;
    char_count++;
  }
  str[char_count] = 0;
}

UiManager::UiManager() : csv{std::make_shared<CsvManager>()}, stats{new RowsReport{}} 
{
  std::vector<std::string> list_types;
  for (const auto& opt : this->f_err_type_opt)
    list_types.push_back(Field::stype_to_string(opt));
    
  fill_option_str(list_types, this->f_err_type_opt_str);
}

void UiManager::reset()
{
  csv.reset(new CsvManager{});
  this->curr_row[0] = 0;
  this->header_count[0] = 0;
  this->curr_row_count[0] = 0;
  this->raw_row[0] = 0;
  this->field_count_err[0] = 0;
  this->num_rows[0] = 0;
  this->header = Row{};
  this->row = Row{};
  this->stats = std::make_shared<RowsReport>();
  this->f_err_type_opt_fields_str[0] = 0;
  this->f_err_type_opt_fields_str[1] = 0;
  for (int i=0; i<3; i++) 
  {
    this->col_rank[i].number_columns[0] = 0;
    this->col_rank[i].percent_rows[0] = 0;
  }
}

void UiManager::row_change(const Row& r)
{
  spdlog::debug("Row received: " + r.str);
  strcpy(this->raw_row, r.str.c_str());
  const int64_t r_num = this->csv->curr_row_num();
  sprintf(this->curr_row, "%ld", r_num);
  if (r_num == 1)
    {
      //this->setup_table_row(r);
      sprintf(this->header_count, "%d", r.col_count);
      this->header = r;
    }
    else
    {
      sprintf(this->curr_row_count, "%d", r.col_count);
      if (strcmp(this->header_count, this->curr_row_count))
        strcpy(this->field_count_err, "");
      else
        strcpy(this->field_count_err, "X");
    }
    this->row = r;
}

void UiManager::open()
{
  spdlog::debug("Open file clicked");
  if (strlen(this->in_filename) > 0 && this->sep[0] != 0 && this->quote[0] != 0)
  {
    try
    {
      this->csv->sep = this->sep[0];
      this->csv->quote = this->quote[0];
      this->csv->open_file(this->in_filename);
      const Row header = csv->next_row();
      std::vector<std::string> header_str_list;
      for (int i=0; i<header.fields.size(); i++)
        header_str_list.push_back(fmt::format("{:d}: {}", i, header.fields[i].str));
      fill_option_str(header_str_list, this->f_err_type_opt_fields_str);

      this->row_change(header);
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error trying to open file: {}", e.what());
    }
  } 
  else 
  {
    spdlog::error("Error opening file, path is empty or separator and quote is missing");
  }
}

void UiManager::on_click_open_file()
{
  spdlog::debug("Open file clicked");
  try
  {
    this->reset();
    strcpy(this->in_filename, tinyfd_openFileDialog("Open File", NULL, 0, NULL, NULL, 0));
    strcpy(this->out_filename, this->in_filename);
    strcat(this->out_filename, ".out");
    this->open();
  }
  catch (const std::exception& e)
  {
    spdlog::error("Error trying to open file: {}", e.what());
  }
}

void UiManager::on_click_close_file()
{
  spdlog::debug("Close file clicked");
  this->in_filename[0] = 0;
  this->out_filename[0] = 0;
  this->reset();
}


void UiManager::on_click_save_file()
{
  spdlog::debug("Save file clicked");
  std::thread t{
    [](const std::shared_ptr<CsvManager>& _csv, const std::string& _out_filename, bool& _show_progress)
    {
      try
      {
        _show_progress = true;
        _csv->save_file(_out_filename);
        _show_progress = false;
        spdlog::info("File saved: {}", _out_filename);
      }
      catch (const std::exception& e)
      {
          spdlog::error("Error trying to save file: {}", e.what());
      }
    },
    this->csv, this->out_filename, std::ref(this->show_progress)
  };
  std::thread t_prog{
    update_progress_bar,
    this->csv,
    std::ref(this->progress),
    std::ref(this->show_progress)
  };
  t.detach();
  t_prog.detach();
}

void UiManager::on_click_reset_file()
{
  spdlog::debug("Reset file clicked");
  this->reset();
  this->open();
}

void UiManager::on_click_next_row()
{
  spdlog::debug("Next row clicked");
  const int64_t last_row = this->csv->curr_row_num();
  const Row r = csv->next_row();
  if (csv->curr_row_num() != last_row)
    this->row_change(r);
  else
    spdlog::warn("No row to show");
}

void UiManager::on_click_back_row()
{
  spdlog::debug("Back row clicked");
  const int64_t last_row = csv->curr_row_num();
  const Row r = csv->back_row();
  if (csv->curr_row_num() != last_row)
    this->row_change(r);
  else
    spdlog::warn("No row to show");
}

void UiManager::on_click_delete_row()
{
  spdlog::debug("Delete row clicked");
  try
  {
    int64_t row = atoi(this->curr_row);
    this->csv->replace_row(row, "");
    spdlog::info("Deleting row {}", row);
  }
  catch (const std::exception& e)
  {
    spdlog::error("Error saving row: {}", e.what());
  }
}

void UiManager::on_click_save_row()
{
  spdlog::debug("Save row clicked");
  try
  {
    int64_t row = atoi(this->curr_row);
    this->csv->replace_row(row, this->raw_row);
    spdlog::info("Row {} saved as: {}", row, this->raw_row);
  }
  catch (const std::exception& e)
  {
    spdlog::error("Error saving row: {}", e.what());
  }
}

void UiManager::on_click_next_error()
{
  spdlog::debug("Next error clicked");
  auto func = [](UiManager* _this)
  {
      Row error = _this->csv->next_error(
        _this->f_error_field_count, 
        _this->f_error_bad_quote,
        _this->f_error_non_print_char,
        _this->f_error_type,
        _this->f_err_selected_field, 
        _this->f_err_type_opt[_this->f_err_selected_type]
      );
      if (!error.fields.empty())
      {
        _this->row_change(error);
        for (const std::string& err : error.error_state)
          spdlog::info("csv error found row {} => {}", _this->csv->curr_row_num(), err);
      }
      else
          spdlog::warn("No more rows to show");
  };
  std::thread t{func, this};
  std::thread t_progress{
    update_progress_bar,
    this->csv,
    std::ref(this->progress),
    std::ref(this->show_progress)
  };
  t.detach();
  t_progress.detach();
}

void UiManager::on_click_update_raw()
{
  spdlog::debug("Update raw clicked");
  std::string new_raw{""};
  for (const Field& f : this->row.fields)
    new_raw +=  f.str + this->sep[0];
  strcpy(this->raw_row, new_raw.substr(0, new_raw.size() - 1).c_str());
}

void UiManager::fill_statistic()
{
  spdlog::debug("filling statistics");
  sprintf(this->col_rank[0].number_columns, "%d", this->stats->field_count_first);
  sprintf(this->col_rank[1].number_columns, "%d", this->stats->field_count_second);
  sprintf(this->col_rank[2].number_columns, "%d", this->stats->field_count_third);
  sprintf(this->col_rank[0].percent_rows, "%.4f%%", this->stats->perc_field_count_first * 100);
  sprintf(this->col_rank[1].percent_rows, "%.4f%%", this->stats->perc_field_count_second * 100);
  sprintf(this->col_rank[2].percent_rows, "%.4f%%", this->stats->perc_field_count_third * 100);
  sprintf(this->num_rows, "%ld", this->stats->row_count);
}

void UiManager::on_click_calculate_statistics()
{
  spdlog::debug("Calculate statistics clicked");
  if (strlen(this->in_filename) > 0 && this->sep[0] != 0 && this->quote[0] != 0)
  {
    try
    {  
      std::shared_ptr<CsvManager> stats_csv{new CsvManager{}};
      stats_csv->sep = this->sep[0];
      stats_csv->quote = this->quote[0];
      stats_csv->open_file(this->in_filename);

      auto func = [](std::shared_ptr<CsvManager> _csv, UiManager* _this)
      {
        CsvStatistics stats_calc{_csv->next_row()};
        _this->show_progress = true;
        while (true)
        {
          const Row r = _csv->next_row();
          if (_csv->eof())
            break;
          stats_calc.add_row(r);
        }
        _this->stats = std::make_shared<RowsReport>(stats_calc.get_rows_report());
        _this->fill_statistic();
        _this->show_progress = false;
      };
      std::thread reporter{func, stats_csv, this};

      std::thread t_prog{
        update_progress_bar,
        stats_csv,
        std::ref(this->progress),
        std::ref(this->show_progress)
      };      
      reporter.detach();
      t_prog.detach();
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error trying calculate statistics: {}", e.what());
    }
  }
  else 
  {
    spdlog::error("Error opening file, path is empty or separator and quote is missing");
  }
}
