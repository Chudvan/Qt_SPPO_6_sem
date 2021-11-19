#ifndef PRODUCER_H
#define PRODUCER_H
#include <QThread>
#include <QSemaphore>
#include <QString>
#include <QDebug>
#include "worker.h"
#include <QFile>
#include <QTextStream>
const int MAX_FILE_SIZE = 10000;


class Producer : public QThread
{
    Q_OBJECT
public:
    Producer(int buffer_size, Worker **buffer, QSemaphore *free_elements,
             QSemaphore *available_elements, bool *at_end, int id);

    void set_file_name(QString file_name);
    void run() override;
    QByteArray text_;

signals:
    void update_progress(int id, int p);

private:
    int buffer_size_;
    Worker **buffer_;
    QString file_name_;
    QSemaphore *free_elements_;
    QSemaphore *available_elements_;
    bool *at_end_;
    int len_text;
    int id_;
    bool is_always_percent;
};

#endif // PRODUCER_H
