#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QProgressDialog>
#include <thread>
#include <future>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , csv{new CsvManager{}}
{
    ui->setupUi(this);
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

void MainWindow::waiting()
{
    int numFiles{100};
    QProgressDialog progress("Copying files...", "Abort Copy", 0, numFiles, this);
    progress.setWindowModality(Qt::WindowModal);

    for (int i = 0; i < numFiles; i++) {
        progress.setValue(i);

        if (progress.wasCanceled())
            break;
        spdlog::debug("waiting 1s");
        QThread::sleep(1);
    }
    progress.setValue(numFiles);
}

void MainWindow::open(const std::string& filename)
{
    this->reset();
    ui->txt_filename->setText(QString::fromStdString(filename));
    ui->txt_out_filename->setText(QString::fromStdString(filename + ".out"));
    this->csv->sep = ui->txt_separator->text().toStdString()[0];
    this->csv->quote = ui->txt_quote->text().toStdString()[0];
    this->csv->open_file(filename);
    Row header = csv->next_row();
    this->row_change(header);
}

void MainWindow::on_btn_open_file_clicked()
{
    spdlog::debug("Button open file clicked");
    auto path = QFileDialog::getOpenFileName();
    this->open(path.toStdString());
}

void MainWindow::row_change(const Row& r)
{
    spdlog::debug("Row received: " + r.str);
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
}

void MainWindow::on_btn_back_row_clicked()
{
    spdlog::debug("Button back clicked");
    Row r = csv->back_row();
    this->row_change(r);
}


void MainWindow::on_btn_next_row_clicked()
{
    spdlog::debug("Button next clicked");
    Row r = csv->next_row();
    this->row_change(r);
}


void MainWindow::on_btn_next_error_clicked()
{
    spdlog::debug("Button next error clicked");
    Row r = csv->next_error();
    this->row_change(r);
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


long count_rows(std::shared_ptr<CsvManager> csv)
{
    return csv->count_rows();
}

void MainWindow::on_btn_count_rows_clicked()
{
    spdlog::debug("Button count rows clicked");
    std::shared_ptr<CsvManager> count_csv{new CsvManager};
    if (!ui->txt_filename->text().isEmpty())
    {
        count_csv->open_file(ui->txt_filename->text().toStdString());
        long n_rows = 0;
        long long file_size = csv->get_size();
        std::future<long> counter = std::async(count_rows, count_csv);
        spdlog::debug("File size: {}", file_size);
        QProgressDialog progress("Copying files...", "Abort Copy", 0, (int)(file_size / 1000000), this);
        progress.setWindowModality(Qt::WindowModal);
        while(!progress.wasCanceled() && counter.wait_for(std::chrono::milliseconds(500)) != std::future_status::ready) {
            progress.setValue((int)(count_csv->get_position() / 1000000));
            spdlog::debug("Current pos: {}", count_csv->get_position());
        }
        progress.setValue(file_size);
        //this->waiting();
        ui->txt_rows_count->setText(QString::fromStdString(std::to_string(counter.get())));
    }
    else
    {
        spdlog::error("Filename is empty, anything to count");
    }
}


void MainWindow::on_btn_reopen_clicked()
{
    spdlog::debug("Button reopen clicked");
    if (!ui->txt_filename->text().isEmpty())
        this->open(ui->txt_filename->text().toStdString());
    else
        spdlog::error("Filename is empty, select file first");
}


void MainWindow::on_btn_save_row_clicked()
{
    spdlog::debug("Button save row clicked");
    long row = ui->txt_current_row->text().toLong();
    this->csv->replace_row(row, ui->txt_raw_row->toPlainText().toStdString());
}


void MainWindow::on_btn_save_clicked()
{
    spdlog::debug("Button save clicked");
    this->csv->save_file(ui->txt_out_filename->text().toStdString());
}

