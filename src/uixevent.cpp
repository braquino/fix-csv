#include <uixevent.h>

UixEvent::UixEvent(std::shared_ptr<CsvManager> csv) 
  : csv{csv}, input_path{""}, output_path{""}, 
    separator{std::string(1, csv->sep)}, quote{std::string(1, csv->quote)},
    stats{nullptr}, progress{0}, edited_row_str{""}, curr_row{nullptr}
{

}

void UixEvent::reset()
{

}

void UixEvent::row_change(Row r)
{
  this->edited_row_str = r.str;
  this->curr_row.reset(new Row(r));
}

void UixEvent::evt_closeclick()
{

}

void UixEvent::evt_openclick()
{
  if (separator.size() == 1 && quote.size() == 1)
  {
    try
    {
      this->reset();
      if (this->output_path.size() == 0)
      {
        this->output_path = this->input_path + ".out";
      }
      csv->sep = separator[0];
      csv->quote = quote[0];
      csv->open_file(this->input_path);
      Row header = csv->next_row();
      this->row_change(header);
    }
    catch(const std::exception& e)
    {
      //spdlog::error("Error trying to open file: {}", e.what());
    }
  }
  else
  {
    throw std::runtime_error("Invalid separator or quote char");
  }
}

void UixEvent::evt_saveclick()
{

}

void UixEvent::evt_nextrowclick()
{
  long last_row = csv->curr_row_num();
  Row r = csv->next_row();
  if (csv->eof())
  {
      //this->toggle_row_buttons(false);
      spdlog::info("End of file");
  }
  if (csv->curr_row_num() != last_row)
      this->row_change(r);
  else
      spdlog::warn("No row to show");
}

void UixEvent::evt_prevrowclick()
{
  long last_row = csv->curr_row_num();
  Row r = csv->back_row();
  if (csv->curr_row_num() != last_row)
      this->row_change(r);
  else
      spdlog::warn("No row to show");
}

void UixEvent::evt_nexterrorclick()
{
 
}

void UixEvent::evt_delrowclick()
{
  this->edited_row_str = "";
  this->evt_saverowclick();
}

void UixEvent::evt_saverowclick()
{
  spdlog::debug("Button save row clicked");
  try
  {
      long row = csv->curr_row_num();
      this->csv->replace_row(row, this->edited_row_str);
      spdlog::info("Row {} saved as: {}", row, this->edited_row_str);
  }
  catch (const std::exception& e)
  {
      spdlog::error("Error saving row: {}", e.what());
  }
}

void UixEvent::evt_statsclick()
{
  if (separator.size() == 1 && quote.size() == 1 && this->input_path.size() > 0)
  {
    try
    {
      std::shared_ptr<CsvManager> stats_csv{new CsvManager{}};
      stats_csv->sep = separator[0];
      stats_csv->quote = quote[0];
      stats_csv->open_file(this->input_path);
      long long file_size = stats_csv->get_size();
      std::future<RowsReport> reporter = std::async(
        [] (std::shared_ptr<CsvManager> _csv) {
          CsvStatistics stats_calc{_csv->next_row()};
          while (true)
          {
            Row r = _csv->next_row();
            if (_csv->eof()) break;
            stats_calc.add_row(r);
          }
          return stats_calc.get_rows_report();
        },
        stats_csv
      );
      this->waiting<RowsReport>(stats_csv, file_size, reporter);
        try
        {
            RowsReport report = reporter.get();
            this->stats.reset(new RowsReport{report});
        }
        catch (const std::exception& e)
        {
            spdlog::error("Error trying calculate statistics: {}", e.what());
        }
    }
    catch(const std::exception& e)
    {
      spdlog::error("Error trying to open file: {}", e.what());
    }
  }
  else
  {
    throw std::runtime_error("Invalid separator or quote char");
  }
}

void UixEvent::evt_resetclick()
{

}

long UixEvent::get_row()
{
  return csv->curr_row_num();
}

int UixEvent::get_header_count()
{
  return csv->get_header_count();
}

Row UixEvent::get_cur_row()
{
  return csv->curr_row();
}

Row UixEvent::get_header()
{
  return csv->get_header();
}