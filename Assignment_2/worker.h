#ifndef WORKER_H
#define WORKER_H
#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QSemaphore>
const int NUMBER_LOWER_CASE = 100000;


class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(QSemaphore *available_elements, bool *at_end, int id);
    void run() override;
    void set_char(char c);
    void operation();
    int get_id(){return id_;};
signals:
   void data(int id, char c);
   void my_finished(int id);
public:
    QMutex *mutex;
    char c_;
    bool is_changed;
    bool was_read;
private:
    QSemaphore *available_elements_;
    bool *at_end_;
    int id_;
};


#endif // WORKER_H
