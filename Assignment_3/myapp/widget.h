#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentRun>
#include <QPluginLoader>
#include "../myplugin/myplugin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QStringList getPlugins(QString path);
    void testPlugins(QStringList plugins);

private slots:
    void on_browseButton_clicked();

    void on_startButton_clicked();

    void updateListView(int index);

    void on_cancelButton_clicked();

private:
    Ui::Widget *ui;
    Plugin* plugin;
    QString filename;
};
#endif // WIDGET_H
