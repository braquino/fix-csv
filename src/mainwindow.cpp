#include "mainwindow.h"
#include "./ui_mainwindow.h"
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

void MainWindow::reset()
{
    csv.reset(new CsvManager{});
    ui->txt_filename->setText("");
    ui->txt_out_filename->setText("");
    ui->txt_cur_fields_count->setText("");
    ui->txt_header_count->setText("");
    ui->txt_raw_row->setText("");
    ui->txt_current_row->setText("");
    ui->table_row->clear();
    ui->table_row->setColumnCount(0);
    ui->table_row->setRowCount(0);
}

void MainWindow::on_btn_open_file_clicked()
{
    spdlog::debug("Button open file clicked");
    this->reset();
    auto path = QFileDialog::getOpenFileName();
    this->csv->open_file(path.toStdString());
    ui->txt_filename->setText(path);
    ui->txt_current_row->setText(QString::fromStdString(std::to_string(this->csv->curr_row_num())));
    Row header = this->row_change(true);
}

Row MainWindow::row_change(bool forward)
{
    Row r = (forward) ? this->csv->next_row() : this->csv->back_row();
    spdlog::debug("Row received: " + r.str);
    if (r.fields.empty())
    {
        spdlog::warn("No more rows");
        return r;
    }
    ui->txt_raw_row->setText(QString::fromStdString(r.str));
    long r_num = this->csv->curr_row_num();
    ui->txt_current_row->setText(QString::fromStdString(std::to_string(r_num)));
    if (r_num == 1)
    {
        this->setup_table_row(r);
        ui->txt_header_count->setText(QString::fromStdString(std::to_string(r.col_count)));
    }
    else
    {
        ui->txt_cur_fields_count->setText(QString::fromStdString(std::to_string(r.col_count)));
        if (ui->txt_cur_fields_count->text() != ui->txt_header_count->text())
            ui->label_count_err->setText("X");
        else
           ui->label_count_err->setText("");
    }
    this->fill_table(r);
    return r;
}

void MainWindow::on_btn_back_row_clicked()
{
    spdlog::debug("Button back clicked");
    Row r = this->row_change(false);
}


void MainWindow::on_btn_next_row_clicked()
{
    spdlog::debug("Button next clicked");
    Row r = this->row_change(true);
}


void MainWindow::on_btn_next_error_clicked()
{

}

void MainWindow::setup_table_row(const Row& header)
{
    spdlog::debug("Setting up table");
    ui->table_row->setColumnCount(header.col_count);
    ui->table_row->setRowCount(4);
    ui->table_row->setVerticalHeaderLabels(QStringList{{"string", "hex", "char count", "type"}});
    QStringList h_header;
    for (const Field& f : header.fields)
        h_header.push_back(QString::fromStdString(f.str));
    ui->table_row->setHorizontalHeaderLabels(h_header);
}

void MainWindow::fill_table(const Row& r) {
    spdlog::debug("Filling table with current row");
    ui->table_row->clearContents();
    ui->table_row->setColumnCount(std::max((int)r.col_count, ui->table_row->columnCount()));
    int i = 0;
    for (const Field& f : r.fields)
    {
        ui->table_row->setItem(0, i, new QTableWidgetItem{QString::fromStdString(f.str)});
        ui->table_row->setItem(1, i, new QTableWidgetItem{QString::fromStdString(f.hex())});
        ui->table_row->setItem(2, i, new QTableWidgetItem{QString::fromStdString(std::to_string(f.char_count))});
        ui->table_row->setItem(3, i, new QTableWidgetItem{QString::fromStdString(f.stype_str())});
        i++;
    }
}

void MainWindow::on_btn_close_file_clicked()
{
    spdlog::debug("Button close file clicked");
    this->reset();
}

