#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csvmanager.h"
#include <QTableWidgetItem>
#include <spdlog/spdlog.h>

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

private:
    Ui::MainWindow *ui;
    std::unique_ptr<CsvManager> csv;

    void row_change(const Row& r);
    void fill_table(const Row& r);
    void setup_table_row(const Row& header);
    void reset();
    void open(const std::string& filename);
    void waiting();
};
#endif // MAINWINDOW_H
