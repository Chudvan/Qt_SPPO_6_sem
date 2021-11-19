#ifndef CONSUMER_H
#define CONSUMER_H
#include <QThread>
#include <QSemaphore>
#include <QString>
#include <QDebug>
#include "worker.h"

class Consumer : public QThread
{
    Q_OBJECT
public:
    Consumer(int buffer_size, Worker **buffer, QSemaphore *free_elements,
             QSemaphore *available_elements, int id);
    void run() override;
    bool is_workers_finished();

bool *at_end_worker;
signals:
    void data(int id, char c);
    void my_finished(int id);
public slots:
    void change_workers_status(int id);
private:
    int buffer_size_;
    Worker **buffer_;
    QString file_name_;
    QSemaphore *free_elements_;
    QSemaphore *available_elements_;
    int i;
    int id_;
    int *workers_id;
};

#endif // CONSUMER_H
