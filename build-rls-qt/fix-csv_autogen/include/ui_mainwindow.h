/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btn_open_file;
    QLineEdit *txt_out_filename;
    QLabel *label_fname;
    QLabel *label_out_fname;
    QLabel *label_rows_qtt;
    QTextEdit *txt_raw_row;
    QLabel *label_row;
    QPushButton *btn_back_row;
    QPushButton *btn_next_row;
    QLabel *txt_filename;
    QLabel *txt_rows_count;
    QLabel *txt_current_row;
    QPushButton *btn_next_error;
    QLabel *label_header_count;
    QLabel *txt_header_count;
    QLabel *label_cur_fields_count;
    QLabel *txt_cur_fields_count;
    QLabel *label_count_err;
    QTableWidget *table_row;
    QPushButton *btn_close_file;
    QPushButton *btn_count_rows;
    QLineEdit *txt_separator;
    QLineEdit *txt_quote;
    QLabel *label_fname_2;
    QLabel *label_fname_3;
    QFrame *frame_csv;
    QPushButton *btn_reopen;
    QPushButton *btn_save;
    QPushButton *btn_save_row;
    QTextEdit *txt_logs;
    QLabel *label_logs;
    QPushButton *btn_delete_row;
    QFrame *frame_stats;
    QLabel *label_fc2;
    QLabel *label_fc1;
    QLabel *label_stat;
    QLabel *txt_fc1;
    QLabel *txt_p_fc1;
    QLabel *txt_fc2;
    QLabel *txt_p_fc2;
    QLabel *txt_p_fc3;
    QLabel *txt_fc3;
    QLabel *label_fc3;
    QPushButton *btn_calc_stats;
    QFrame *frame_next_error;
    QCheckBox *cb_err_field_count;
    QCheckBox *cb_err_non_print;
    QCheckBox *cb_err_bad_quote;
    QPushButton *btn_table_to_row;
    QCheckBox *cb_err_type;
    QLineEdit *txt_err_type;
    QComboBox *comb_err_type;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1200, 842);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btn_open_file = new QPushButton(centralwidget);
        btn_open_file->setObjectName("btn_open_file");
        btn_open_file->setGeometry(QRect(10, 10, 89, 25));
        txt_out_filename = new QLineEdit(centralwidget);
        txt_out_filename->setObjectName("txt_out_filename");
        txt_out_filename->setGeometry(QRect(280, 50, 641, 25));
        label_fname = new QLabel(centralwidget);
        label_fname->setObjectName("label_fname");
        label_fname->setGeometry(QRect(192, 11, 71, 20));
        label_out_fname = new QLabel(centralwidget);
        label_out_fname->setObjectName("label_out_fname");
        label_out_fname->setGeometry(QRect(170, 50, 101, 20));
        label_rows_qtt = new QLabel(centralwidget);
        label_rows_qtt->setObjectName("label_rows_qtt");
        label_rows_qtt->setGeometry(QRect(950, 10, 41, 20));
        txt_raw_row = new QTextEdit(centralwidget);
        txt_raw_row->setObjectName("txt_raw_row");
        txt_raw_row->setEnabled(true);
        txt_raw_row->setGeometry(QRect(10, 220, 1171, 121));
        txt_raw_row->setReadOnly(false);
        label_row = new QLabel(centralwidget);
        label_row->setObjectName("label_row");
        label_row->setGeometry(QRect(20, 180, 41, 20));
        btn_back_row = new QPushButton(centralwidget);
        btn_back_row->setObjectName("btn_back_row");
        btn_back_row->setGeometry(QRect(938, 350, 111, 25));
        btn_next_row = new QPushButton(centralwidget);
        btn_next_row->setObjectName("btn_next_row");
        btn_next_row->setGeometry(QRect(1068, 350, 111, 25));
        txt_filename = new QLabel(centralwidget);
        txt_filename->setObjectName("txt_filename");
        txt_filename->setGeometry(QRect(280, 10, 641, 25));
        txt_filename->setFrameShape(QFrame::Box);
        txt_filename->setFrameShadow(QFrame::Sunken);
        txt_rows_count = new QLabel(centralwidget);
        txt_rows_count->setObjectName("txt_rows_count");
        txt_rows_count->setGeometry(QRect(1000, 10, 181, 25));
        txt_rows_count->setFrameShape(QFrame::Box);
        txt_rows_count->setFrameShadow(QFrame::Sunken);
        txt_current_row = new QLabel(centralwidget);
        txt_current_row->setObjectName("txt_current_row");
        txt_current_row->setGeometry(QRect(70, 180, 181, 25));
        txt_current_row->setFrameShape(QFrame::Box);
        txt_current_row->setFrameShadow(QFrame::Sunken);
        btn_next_error = new QPushButton(centralwidget);
        btn_next_error->setObjectName("btn_next_error");
        btn_next_error->setGeometry(QRect(16, 350, 111, 25));
        label_header_count = new QLabel(centralwidget);
        label_header_count->setObjectName("label_header_count");
        label_header_count->setGeometry(QRect(380, 180, 121, 20));
        txt_header_count = new QLabel(centralwidget);
        txt_header_count->setObjectName("txt_header_count");
        txt_header_count->setGeometry(QRect(500, 180, 61, 25));
        txt_header_count->setFrameShape(QFrame::Box);
        txt_header_count->setFrameShadow(QFrame::Sunken);
        label_cur_fields_count = new QLabel(centralwidget);
        label_cur_fields_count->setObjectName("label_cur_fields_count");
        label_cur_fields_count->setGeometry(QRect(610, 180, 131, 20));
        txt_cur_fields_count = new QLabel(centralwidget);
        txt_cur_fields_count->setObjectName("txt_cur_fields_count");
        txt_cur_fields_count->setGeometry(QRect(740, 180, 61, 25));
        txt_cur_fields_count->setFrameShape(QFrame::Box);
        txt_cur_fields_count->setFrameShadow(QFrame::Sunken);
        label_count_err = new QLabel(centralwidget);
        label_count_err->setObjectName("label_count_err");
        label_count_err->setGeometry(QRect(810, 183, 16, 17));
        table_row = new QTableWidget(centralwidget);
        table_row->setObjectName("table_row");
        table_row->setGeometry(QRect(10, 400, 1171, 241));
        QFont font;
        font.setPointSize(9);
        table_row->setFont(font);
        table_row->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
        table_row->horizontalHeader()->setDefaultSectionSize(100);
        btn_close_file = new QPushButton(centralwidget);
        btn_close_file->setObjectName("btn_close_file");
        btn_close_file->setGeometry(QRect(10, 50, 89, 25));
        btn_count_rows = new QPushButton(centralwidget);
        btn_count_rows->setObjectName("btn_count_rows");
        btn_count_rows->setGeometry(QRect(1090, 40, 89, 25));
        txt_separator = new QLineEdit(centralwidget);
        txt_separator->setObjectName("txt_separator");
        txt_separator->setGeometry(QRect(100, 100, 16, 25));
        txt_separator->setMaxLength(1);
        txt_quote = new QLineEdit(centralwidget);
        txt_quote->setObjectName("txt_quote");
        txt_quote->setGeometry(QRect(100, 130, 16, 25));
        txt_quote->setMaxLength(1);
        label_fname_2 = new QLabel(centralwidget);
        label_fname_2->setObjectName("label_fname_2");
        label_fname_2->setGeometry(QRect(20, 130, 71, 20));
        label_fname_3 = new QLabel(centralwidget);
        label_fname_3->setObjectName("label_fname_3");
        label_fname_3->setGeometry(QRect(20, 100, 71, 20));
        frame_csv = new QFrame(centralwidget);
        frame_csv->setObjectName("frame_csv");
        frame_csv->setGeometry(QRect(10, 88, 211, 80));
        frame_csv->setFrameShape(QFrame::StyledPanel);
        frame_csv->setFrameShadow(QFrame::Raised);
        btn_reopen = new QPushButton(centralwidget);
        btn_reopen->setObjectName("btn_reopen");
        btn_reopen->setGeometry(QRect(124, 100, 89, 25));
        btn_save = new QPushButton(centralwidget);
        btn_save->setObjectName("btn_save");
        btn_save->setGeometry(QRect(830, 80, 89, 25));
        btn_save_row = new QPushButton(centralwidget);
        btn_save_row->setObjectName("btn_save_row");
        btn_save_row->setGeometry(QRect(740, 350, 151, 25));
        txt_logs = new QTextEdit(centralwidget);
        txt_logs->setObjectName("txt_logs");
        txt_logs->setEnabled(true);
        txt_logs->setGeometry(QRect(10, 670, 1171, 121));
        txt_logs->setReadOnly(true);
        label_logs = new QLabel(centralwidget);
        label_logs->setObjectName("label_logs");
        label_logs->setGeometry(QRect(10, 650, 41, 20));
        btn_delete_row = new QPushButton(centralwidget);
        btn_delete_row->setObjectName("btn_delete_row");
        btn_delete_row->setGeometry(QRect(630, 350, 91, 25));
        frame_stats = new QFrame(centralwidget);
        frame_stats->setObjectName("frame_stats");
        frame_stats->setGeometry(QRect(930, 80, 251, 131));
        frame_stats->setFrameShape(QFrame::StyledPanel);
        frame_stats->setFrameShadow(QFrame::Raised);
        label_fc2 = new QLabel(centralwidget);
        label_fc2->setObjectName("label_fc2");
        label_fc2->setGeometry(QRect(940, 131, 111, 20));
        label_fc1 = new QLabel(centralwidget);
        label_fc1->setObjectName("label_fc1");
        label_fc1->setGeometry(QRect(940, 109, 101, 20));
        label_stat = new QLabel(centralwidget);
        label_stat->setObjectName("label_stat");
        label_stat->setGeometry(QRect(940, 80, 71, 20));
        txt_fc1 = new QLabel(centralwidget);
        txt_fc1->setObjectName("txt_fc1");
        txt_fc1->setGeometry(QRect(1050, 107, 41, 25));
        txt_fc1->setFrameShape(QFrame::Box);
        txt_fc1->setFrameShadow(QFrame::Sunken);
        txt_p_fc1 = new QLabel(centralwidget);
        txt_p_fc1->setObjectName("txt_p_fc1");
        txt_p_fc1->setGeometry(QRect(1100, 107, 71, 25));
        txt_p_fc1->setFrameShape(QFrame::Box);
        txt_p_fc1->setFrameShadow(QFrame::Sunken);
        txt_p_fc1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txt_fc2 = new QLabel(centralwidget);
        txt_fc2->setObjectName("txt_fc2");
        txt_fc2->setGeometry(QRect(1050, 130, 41, 25));
        txt_fc2->setFrameShape(QFrame::Box);
        txt_fc2->setFrameShadow(QFrame::Sunken);
        txt_p_fc2 = new QLabel(centralwidget);
        txt_p_fc2->setObjectName("txt_p_fc2");
        txt_p_fc2->setGeometry(QRect(1100, 130, 71, 25));
        txt_p_fc2->setFrameShape(QFrame::Box);
        txt_p_fc2->setFrameShadow(QFrame::Sunken);
        txt_p_fc2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txt_p_fc3 = new QLabel(centralwidget);
        txt_p_fc3->setObjectName("txt_p_fc3");
        txt_p_fc3->setGeometry(QRect(1100, 153, 71, 25));
        txt_p_fc3->setFrameShape(QFrame::Box);
        txt_p_fc3->setFrameShadow(QFrame::Sunken);
        txt_p_fc3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        txt_fc3 = new QLabel(centralwidget);
        txt_fc3->setObjectName("txt_fc3");
        txt_fc3->setGeometry(QRect(1050, 153, 41, 25));
        txt_fc3->setFrameShape(QFrame::Box);
        txt_fc3->setFrameShadow(QFrame::Sunken);
        label_fc3 = new QLabel(centralwidget);
        label_fc3->setObjectName("label_fc3");
        label_fc3->setGeometry(QRect(940, 153, 101, 20));
        btn_calc_stats = new QPushButton(centralwidget);
        btn_calc_stats->setObjectName("btn_calc_stats");
        btn_calc_stats->setGeometry(QRect(1000, 182, 151, 25));
        frame_next_error = new QFrame(centralwidget);
        frame_next_error->setObjectName("frame_next_error");
        frame_next_error->setGeometry(QRect(10, 345, 541, 51));
        frame_next_error->setFrameShape(QFrame::StyledPanel);
        frame_next_error->setFrameShadow(QFrame::Raised);
        cb_err_field_count = new QCheckBox(centralwidget);
        cb_err_field_count->setObjectName("cb_err_field_count");
        cb_err_field_count->setGeometry(QRect(140, 350, 92, 23));
        cb_err_field_count->setChecked(true);
        cb_err_non_print = new QCheckBox(centralwidget);
        cb_err_non_print->setObjectName("cb_err_non_print");
        cb_err_non_print->setGeometry(QRect(140, 370, 121, 23));
        cb_err_bad_quote = new QCheckBox(centralwidget);
        cb_err_bad_quote->setObjectName("cb_err_bad_quote");
        cb_err_bad_quote->setGeometry(QRect(280, 350, 92, 23));
        btn_table_to_row = new QPushButton(centralwidget);
        btn_table_to_row->setObjectName("btn_table_to_row");
        btn_table_to_row->setGeometry(QRect(570, 370, 41, 25));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("go-up");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        btn_table_to_row->setIcon(icon);
        cb_err_type = new QCheckBox(centralwidget);
        cb_err_type->setObjectName("cb_err_type");
        cb_err_type->setGeometry(QRect(280, 370, 201, 23));
        txt_err_type = new QLineEdit(centralwidget);
        txt_err_type->setObjectName("txt_err_type");
        txt_err_type->setGeometry(QRect(340, 372, 51, 20));
        txt_err_type->setMaxLength(5);
        txt_err_type->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        comb_err_type = new QComboBox(centralwidget);
        comb_err_type->setObjectName("comb_err_type");
        comb_err_type->setGeometry(QRect(441, 371, 101, 21));
        MainWindow->setCentralWidget(centralwidget);
        frame_csv->raise();
        btn_open_file->raise();
        txt_out_filename->raise();
        label_fname->raise();
        label_out_fname->raise();
        label_rows_qtt->raise();
        txt_raw_row->raise();
        label_row->raise();
        btn_back_row->raise();
        btn_next_row->raise();
        txt_filename->raise();
        txt_rows_count->raise();
        txt_current_row->raise();
        label_header_count->raise();
        txt_header_count->raise();
        label_cur_fields_count->raise();
        txt_cur_fields_count->raise();
        label_count_err->raise();
        table_row->raise();
        btn_close_file->raise();
        btn_count_rows->raise();
        txt_separator->raise();
        txt_quote->raise();
        label_fname_2->raise();
        label_fname_3->raise();
        btn_reopen->raise();
        btn_save->raise();
        btn_save_row->raise();
        txt_logs->raise();
        label_logs->raise();
        btn_delete_row->raise();
        frame_stats->raise();
        label_fc2->raise();
        label_fc1->raise();
        label_stat->raise();
        txt_fc1->raise();
        txt_p_fc1->raise();
        txt_fc2->raise();
        txt_p_fc2->raise();
        txt_p_fc3->raise();
        txt_fc3->raise();
        label_fc3->raise();
        btn_calc_stats->raise();
        frame_next_error->raise();
        btn_next_error->raise();
        cb_err_field_count->raise();
        cb_err_non_print->raise();
        cb_err_bad_quote->raise();
        btn_table_to_row->raise();
        cb_err_type->raise();
        txt_err_type->raise();
        comb_err_type->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "fix-csv", nullptr));
        btn_open_file->setText(QCoreApplication::translate("MainWindow", "Open File", nullptr));
        label_fname->setText(QCoreApplication::translate("MainWindow", "File name:", nullptr));
        label_out_fname->setText(QCoreApplication::translate("MainWindow", "Output name:", nullptr));
        label_rows_qtt->setText(QCoreApplication::translate("MainWindow", "rows:", nullptr));
        label_row->setText(QCoreApplication::translate("MainWindow", "Row:", nullptr));
        btn_back_row->setText(QCoreApplication::translate("MainWindow", "<< Back row", nullptr));
        btn_next_row->setText(QCoreApplication::translate("MainWindow", "Next row >>", nullptr));
        txt_filename->setText(QString());
        txt_rows_count->setText(QString());
        txt_current_row->setText(QString());
        btn_next_error->setText(QCoreApplication::translate("MainWindow", "Next error >>", nullptr));
        label_header_count->setText(QCoreApplication::translate("MainWindow", "Fields in header:", nullptr));
        txt_header_count->setText(QString());
        label_cur_fields_count->setText(QCoreApplication::translate("MainWindow", "Fields in this row:", nullptr));
        txt_cur_fields_count->setText(QString());
        label_count_err->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        btn_close_file->setText(QCoreApplication::translate("MainWindow", "Close File", nullptr));
        btn_count_rows->setText(QCoreApplication::translate("MainWindow", "Count rows", nullptr));
        txt_separator->setText(QCoreApplication::translate("MainWindow", ",", nullptr));
        txt_quote->setText(QCoreApplication::translate("MainWindow", "\"", nullptr));
        label_fname_2->setText(QCoreApplication::translate("MainWindow", "Quote:", nullptr));
        label_fname_3->setText(QCoreApplication::translate("MainWindow", "Separator:", nullptr));
        btn_reopen->setText(QCoreApplication::translate("MainWindow", "Reset File", nullptr));
        btn_save->setText(QCoreApplication::translate("MainWindow", "Save File", nullptr));
        btn_save_row->setText(QCoreApplication::translate("MainWindow", "Save modified row", nullptr));
        label_logs->setText(QCoreApplication::translate("MainWindow", "Logs:", nullptr));
        btn_delete_row->setText(QCoreApplication::translate("MainWindow", "Delete row", nullptr));
        label_fc2->setText(QCoreApplication::translate("MainWindow", "num fields 2nd:", nullptr));
        label_fc1->setText(QCoreApplication::translate("MainWindow", "num fields 1st:", nullptr));
        label_stat->setText(QCoreApplication::translate("MainWindow", "statistics", nullptr));
        txt_fc1->setText(QString());
        txt_p_fc1->setText(QString());
        txt_fc2->setText(QString());
        txt_p_fc2->setText(QString());
        txt_p_fc3->setText(QString());
        txt_fc3->setText(QString());
        label_fc3->setText(QCoreApplication::translate("MainWindow", "num fields 3rd:", nullptr));
        btn_calc_stats->setText(QCoreApplication::translate("MainWindow", "Calculate statistics", nullptr));
        cb_err_field_count->setText(QCoreApplication::translate("MainWindow", "field count", nullptr));
        cb_err_non_print->setText(QCoreApplication::translate("MainWindow", "non print char", nullptr));
        cb_err_bad_quote->setText(QCoreApplication::translate("MainWindow", "bad quote", nullptr));
        btn_table_to_row->setText(QString());
        cb_err_type->setText(QCoreApplication::translate("MainWindow", "fld #:                   type:", nullptr));
        txt_err_type->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
