#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "csvmanager.h"

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

private:
    Ui::MainWindow *ui;
    std::shared_ptr<CsvManager> csv;
};
#endif // MAINWINDOW_H
