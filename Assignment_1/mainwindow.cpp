#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stop_button->setEnabled(false);

    connect(&process, SIGNAL(finished(int)), this, SLOT(show_status()));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(show_stdout()));
    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(show_stderror()));
    connect(ui->file_edit, SIGNAL(editingFinished()), this, SLOT(check_file_edit()));

    setWindowTitle("Assignment 1");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_select_button_clicked()
{
    QString new_filename = QFileDialog::getOpenFileName(this, QString("Open file"));
    filename = new_filename.isEmpty() ? filename : new_filename;
    ui->file_edit->setText(filename);
}

void MainWindow::on_run_button_clicked()
{
    filename = ui->file_edit->text();
    time.start();
    QStringList argv = ui->args_edit->text().split(" ");
    argv.isEmpty() ? process.start(filename) : process.start(filename, argv);
    if(process.state() != QProcess::NotRunning) enable_buttons(false);
}

void MainWindow::on_stop_button_clicked()
{
    process.kill();
}

void MainWindow::show_status(){
    int exit_code = process.exitCode();
    ui->exit_code_edit->setText(QString::number(exit_code));
    int elapsed_time = time.elapsed();
    ui->duration_edit->setText(QString::number(static_cast<double>(elapsed_time) / 1000));
    enable_buttons(true);
};

void MainWindow::show_stdout(){
    process.setReadChannel(QProcess::StandardOutput);
    show_all();
};

void MainWindow::show_stderror(){
    process.setReadChannel(QProcess::StandardError);
    show_all();
};

void MainWindow::check_file_edit(){
    filename = ui->file_edit->text();
}

void MainWindow::enable_buttons(bool status){
    ui->run_button->setEnabled(status);
    ui->stop_button->setEnabled(!status);
};

void MainWindow::show_all(){
    QByteArray data = process.readAll();
    QTextCodec *codec = QTextCodec::codecForName("IBM866");
    //    qDebug() << QTextCodec::availableCodecs();
    QString output = codec->toUnicode(data);

    #ifdef Q_OS_WIN
    output.remove(output.count() - 2, 2);
    #else
    output.remove(output.count() - 1, 1);
    #endif

    ui->console_edit->append(output);
};
