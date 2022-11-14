#include <spdlog/spdlog.h>
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    #ifndef NDBUG
        spdlog::info("Initializing debug mode");
        spdlog::set_level(spdlog::level::debug);
    #else
        spdlog::info("Initializing release mode");
        spdlog::set_level(spdlog::level::info);
    #endif
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
