#ifndef HELPERUI_H_
#define HELPERUI_H_

#include "imgui.h"
#include "csvmanager.h"
#include "csvstatistics.h"
#include "spdlog/spdlog.h"
#include <string>
#include <future>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

ImVec2 operator+(const ImVec2 &a, const ImVec2 &b);

struct RowStatRank
{
    const std::string row_name;
    char number_columns[50];
    char percent_rows[50];
    const std::string id1{"##1" + this->row_name};
    const std::string id2{"##2" + this->row_name};

    void render(const ImVec2 &start_pos);
};

void update_progress_bar(const std::shared_ptr<CsvManager> _csv,
                         float &_progress,
                         bool &_show_progress);

void render_table(const Row &header, Row &r, const std::shared_ptr<RowsReport> &stats);

#endif // HELPERUI_H_