#include "uimanager.h"
#include "spdlog/spdlog.h"
#include "tinyfiledialogs/tinyfiledialogs.h"
#include <future>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;


UiManager::UiManager() : csv{std::make_shared<CsvManager>()} {}

void UiManager::reset()
{
  csv.reset(new CsvManager{});
  this->in_filename[0] = 0;
  this->out_filename[0] = 0;
  this->sep[0] = ',';
  this->quote[0] = '"';
  this->curr_row[0] = 0;
  this->header_count[0] = 0;
  this->curr_row_count[0] = 0;
  this->raw_row[0] = 0;
  this->field_count_err[0] = 0;
  this->num_rows[0] = 0;
  this->header = Row{};
  this->row = Row{};
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
  this->reset();
}


void UiManager::on_click_save_file()
{
  spdlog::debug("Save file clicked");
  int64_t file_size = this->csv->get_size();
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
    file_size,
    std::ref(this->progress),
    std::ref(this->show_progress)
  };
  t.detach();
  t_prog.detach();
}

void UiManager::on_click_reset_file()
{
  spdlog::debug("Reset file clicked");
  this->open();
}

void UiManager::on_click_statistics()
{
  spdlog::debug("Calculate statistics clicked");
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

}

void UiManager::on_click_update_raw()
{
  spdlog::debug("Update raw clicked");
}
