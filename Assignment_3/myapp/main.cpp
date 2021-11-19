#include "widget.h"

#include <QApplication>
#include "../myplugin/myplugin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    QDir currentDir = QDir::current();
    currentDir.cdUp();
    currentDir.cd("myplugin");
    currentDir.cd("debug");
    qDebug() << currentDir.absolutePath();
    QStringList plugins = w.getPlugins(currentDir.absolutePath());
    w.testPlugins(plugins);

    w.show();
    return a.exec();
}
