#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QFileDialog>
#include <QTime>
#include <QStringList>
#include <QByteArray>
#include <QTextCodec>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_select_button_clicked();
    void on_run_button_clicked();
    void on_stop_button_clicked();
    void show_status();
    void show_stdout();
    void show_stderror();
    void check_file_edit();

private:
    void enable_buttons(bool);
    void show_all();

    Ui::MainWindow *ui;
    QProcess process;
    QString filename;
    QTime time;
};
#endif // MAINWINDOW_H
