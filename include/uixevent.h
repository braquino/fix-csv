#ifndef UIXEVENT_H_
#define UIXEVENT_H_

#include <csvmanager.h>
#include <csvstatistics.h>
#include <memory>
#include <thread>
#include <future>
#include <spdlog/spdlog.h>

class UixEvent
{
private:
  std::shared_ptr<CsvManager> csv;
  template <typename T>
  void waiting(std::shared_ptr<CsvManager> _csv, long long& file_size, std::future<T>& exec)
  {
      spdlog::debug("File size: {}", file_size);
      while(exec.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready) {
          progress = (_csv->get_position() / file_size);
      }
  }
public:
  UixEvent(std::shared_ptr<CsvManager> csv);
  ~UixEvent() {};

  void evt_openclick();
  void evt_closeclick();
  void evt_saveclick();
  void evt_resetclick();
  void evt_statsclick();
  void evt_nextrowclick();
  void evt_prevrowclick();
  void evt_nexterrorclick();
  void evt_delrowclick();
  void evt_saverowclick();

  void reset();
  void row_change(Row r);

  long get_row();
  int get_header_count();
  Row get_cur_row();
  Row get_header();

  std::string input_path;
  std::string output_path;
  std::string edited_row_str;
  std::string separator;
  std::string quote;
  std::shared_ptr<RowsReport> stats;
  std::shared_ptr<Row> curr_row;
  float progress;
};

#endif // UIXEVENT_H_
