#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QThread>
#include "spdlog/sinks/stdout_sinks.h"
#include "qtextlogsink.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , csv{new CsvManager{}}
{
    ui->setupUi(this);
    this->setup_log();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setup_log()
{
    auto qtextlog_sink = std::make_shared<QTextLogSing_mt>(ui->txt_logs);
    auto console_sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
    auto logger = std::make_shared<spdlog::logger>("logger");
    logger->sinks().push_back(qtextlog_sink);
    logger->sinks().push_back(console_sink);
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [thrd: %t]: %v");
    #if DEBUG
        spdlog::info("Initializing debug mode");
        spdlog::set_level(spdlog::level::debug);
    #else
        spdlog::info("Initializing release mode");
        spdlog::set_level(spdlog::level::info);
    #endif
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

void MainWindow::open(const std::string& filename)
{
    QString sep = ui->txt_separator->text();
    QString quote = ui->txt_quote->text();
    if (sep.size() == 1 && quote.size() == 1)
    {
        try
        {
            this->reset();
            ui->txt_filename->setText(QString::fromStdString(filename));
            ui->txt_out_filename->setText(QString::fromStdString(filename + ".out"));
            this->csv->sep = sep.toStdString()[0];
            this->csv->quote = quote.toStdString()[0];
            this->csv->open_file(filename);
            Row header = csv->next_row();
            this->row_change(header);
        }
        catch (const std::exception& e)
        {
            spdlog::error("Error trying to open file: {}", e.what());
        }
    }
    else
    {
        spdlog::error("Separator and quote shall not be empty");
    }
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
    if (!r.fields.empty())
        this->row_change(r);
    else
        spdlog::warn("No row to show");
}


void MainWindow::on_btn_next_row_clicked()
{
    spdlog::debug("Button next clicked");
    Row r = csv->next_row();
    if (!r.fields.empty())
        this->row_change(r);
    else
        spdlog::warn("No row to show");
}


void MainWindow::on_btn_next_error_clicked()
{
    spdlog::debug("Button next error clicked");
    long long file_size = this->csv->get_size();
    std::future<Row> get_next_error = std::async(
            [](std::shared_ptr<CsvManager> csv){return csv->next_error();},
            this->csv
    );
    this->waiting<Row>(this->csv, file_size, get_next_error);
    try
    {
        Row r = get_next_error.get();
        if (!r.fields.empty())
            this->row_change(r);
        else
            spdlog::warn("No more rows to show");
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error getting next error: {}", e.what());
    }
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

void MainWindow::on_btn_count_rows_clicked()
{
    spdlog::debug("Button count rows clicked");
    std::shared_ptr<CsvManager> count_csv{new CsvManager};
    if (!ui->txt_filename->text().isEmpty())
    {
        count_csv->open_file(ui->txt_filename->text().toStdString());
        long long file_size = csv->get_size();
        std::future<long> counter = std::async(
                [](std::shared_ptr<CsvManager> csv){return csv->count_rows();},
                count_csv
        );
        this->waiting<long>(count_csv, file_size, counter);
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
    try
    {
        long row = ui->txt_current_row->text().toLong();
        std::string str_row = ui->txt_raw_row->toPlainText().toStdString();
        this->csv->replace_row(row, str_row);
        spdlog::info("Row {} saved as: {}", row, str_row);
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error saving row: {}", e.what());
    }
}


void MainWindow::on_btn_save_clicked()
{
    spdlog::debug("Button save clicked");
    long long file_size = this->csv->get_size();
    std::future<void> file_saver = std::async(
            [](std::shared_ptr<CsvManager> _csv, const std::string& path){_csv->save_file(path);},
            this->csv,
            ui->txt_out_filename->text().toStdString()
    );
    this->waiting<void>(this->csv, file_size, file_saver);
    try
    {
        file_saver.get();
        spdlog::info("File saved: {}", ui->txt_out_filename->text().toStdString());
    }
    catch (const std::exception& e)
    {
        spdlog::error("Error trying to save file: {}", e.what());
    }
}

void MainWindow::fill_statistics(const RowsReport& report)
{
    ui->txt_rows_count->setText(QString::number(report.row_count));
    ui->txt_fc1->setText(QString::number(report.field_count_first));
    ui->txt_p_fc1->setText(QString::number(report.perc_field_count_first));
    ui->txt_fc2->setText(QString::number(report.field_count_second));
    ui->txt_p_fc2->setText(QString::number(report.perc_field_count_second));
    ui->txt_fc3->setText(QString::number(report.field_count_third));
    ui->txt_p_fc3->setText(QString::number(report.perc_field_count_third));
}

void MainWindow::on_btn_calc_stats_clicked()
{
    spdlog::debug("Button calculate stats clicked");
    QString sep = ui->txt_separator->text();
    QString quote = ui->txt_quote->text();
    if (!ui->txt_filename->text().isEmpty() && sep.size() == 1 && quote.size() == 1)
    {
        std::shared_ptr<CsvManager> stats_csv{new CsvManager{}};
        stats_csv->sep = sep.toStdString()[0];
        stats_csv->quote = quote.toStdString()[0];
        stats_csv->open_file(ui->txt_filename->text().toStdString());
        long long file_size = stats_csv->get_size();
        std::future<RowsReport> reporter = std::async(
            [](std::shared_ptr<CsvManager> _csv)
            {
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
        this->waiting<RowsReport>(stats_csv, file_size,reporter);
        try
        {
            RowsReport report = reporter.get();
            this->fill_statistics(report);
        }
        catch (const std::exception& e)
        {
            spdlog::error("Error trying calculate statistics: {}", e.what());
        }
    }
    else
        spdlog::warn("No file opened, cant calculate statistics");
}

