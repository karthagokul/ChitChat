#include "widget.h"
#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile sfile(":/style/stylesheet.qss");
    sfile.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(sfile.readAll());
    sfile.close();
    a.setStyleSheet(StyleSheet);
    MainWindow m;

    m.show();

    return a.exec();
}
