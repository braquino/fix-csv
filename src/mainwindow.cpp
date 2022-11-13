#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <spdlog/spdlog.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , csv{new CsvManager{}}
{
    ui->setupUi(this);
    //connect(ui->btn_open_file, SIGNAL(clicked(bool)), this, SLOT(on_btn_open_file_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_open_file_clicked()
{
    spdlog::info("Button open file clicked");
    auto path = QFileDialog::getOpenFileName();
    this->csv->open_file(path.toStdString());
    ui->txt_filename->setText(path);
    ui->txt_current_row->setText(QString::fromStdString(std::to_string(this->csv->get_row())));
}


void MainWindow::on_btn_back_row_clicked()
{
    spdlog::info("Button back clicked");
    ui->txt_raw_row->setText(QString::fromStdString(this->csv->back_row()));
    ui->txt_current_row->setText(QString::fromStdString(std::to_string(this->csv->get_row())));
}


void MainWindow::on_btn_next_row_clicked()
{
    spdlog::info("Button next clicked");
    ui->txt_raw_row->setText(QString::fromStdString(this->csv->next_row()));
    ui->txt_current_row->setText(QString::fromStdString(std::to_string(this->csv->get_row())));
}

