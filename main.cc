#include "include/meet_window.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qSetMessagePattern(
        "%{time yyyy-MM-dd hh:mm:ss}--[%{type}]--%{function}:%{message}");
    MeetWindow main_window;
    main_window.show();
    return QApplication::exec();
}
