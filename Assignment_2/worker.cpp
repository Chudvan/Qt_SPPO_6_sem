#include "worker.h"

Worker::Worker(QSemaphore *available_elements, bool *at_end, int id)
    : available_elements_(available_elements), at_end_(at_end), is_changed(false), was_read(false), id_(id)
{
    mutex = new QMutex;
}

void Worker::run(){
    while(!*at_end_ || is_changed){
        while(!is_changed){
            msleep(1);
        }
        mutex->lock();
        operation();
        is_changed = false;
        available_elements_->release();
        emit data(id_, c_);
        while(!was_read){
            msleep(1);
        }
        was_read = false;
        mutex->unlock();
    }
    emit my_finished(id_);
}

void  Worker::set_char(char c){
    c_ = c;
}

void Worker::operation(){
    QString s = QString(c_);
    for (int i = 0; i < NUMBER_LOWER_CASE; i++){
        s = s.toUpper();
        s = s.toLower();
    }
}
