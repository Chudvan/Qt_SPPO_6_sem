#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSemaphore>
#include <QFileDialog>
#include "producer.h"
#include "consumer.h"
#include "worker.h"
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void clear_output();

    bool is_full_done();

private slots:

    void on_start_clicked();

    void on_choose_file_1_clicked();

    void on_choose_file_2_clicked();

    void on_choose_file_3_clicked();

    void update_info_consumer(int, char);

    void update_info_worker(int, char);

    void update_progress(int, int);

    void change_status(int);

private:
    QVector<int> n;
    QVector<int> order_of_workers;
    int n_of_producers;
    int n_of_workers;
    Ui::Widget *ui;
    QVector<QString> file_names;
    Producer **producers;
    Consumer **consumers;
    Worker **buffer;

    QSemaphore **free_elements_;
    QSemaphore **available_elements_;
    bool *at_end_;
    bool *at_end_consumers;
    bool done;
};
#endif // WIDGET_H
