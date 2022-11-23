#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csvmanager.h"
#include "csvstatistics.h"
#include <QTableWidgetItem>
#include <QProgressDialog>
#include <spdlog/spdlog.h>
#include <thread>
#include <future>
#include <sstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_open_file_clicked();

    void on_btn_back_row_clicked();

    void on_btn_next_row_clicked();

    void on_btn_next_error_clicked();

    void on_btn_close_file_clicked();

    void on_btn_count_rows_clicked();

    void on_btn_reopen_clicked();

    void on_btn_save_row_clicked();

    void on_btn_save_clicked();

    void on_btn_calc_stats_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<CsvManager> csv;

    void row_change(const Row& r);
    void fill_table(const Row& r);
    void setup_table_row(const Row& header);
    void reset();
    void open(const std::string& filename);
    void setup_log();
    void fill_statistics(const RowsReport& report);

    template <typename T>
    void waiting(std::shared_ptr<CsvManager> _csv, long long& file_size, std::future<T>& exec)
    {
        spdlog::debug("File size: {}", file_size);
        QProgressDialog progress("Executing operation...", "Stop", 0, (int)(file_size / 1000000), this);
        progress.setWindowModality(Qt::WindowModal);
        while(!progress.wasCanceled() && exec.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready) {
            progress.setValue((int)(_csv->get_position() / 1000000));
        }
        progress.setValue(file_size);
    }
};
#endif // MAINWINDOW_H
