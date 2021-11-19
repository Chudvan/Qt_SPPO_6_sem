#include "consumer.h"

Consumer::Consumer(int buffer_size, Worker **buffer, QSemaphore *free_elements,
                   QSemaphore *available_elements, int id)
    : buffer_size_(buffer_size), buffer_(buffer),
      free_elements_(free_elements), available_elements_(available_elements), i(0), id_(id)
{
    at_end_worker = new bool[buffer_size_];
    workers_id = new int[buffer_size_];
    for(int j = 0; j < buffer_size_; j++){
        at_end_worker[j] = false;
        workers_id[j] = buffer[j]->get_id();
    }

    for(int j = 0; j < buffer_size_; j++)
        connect(buffer[j], &Worker::my_finished, this, &Consumer::change_workers_status);
}

bool Consumer::is_workers_finished(){
    bool result = true;
    for (int j = 0; j < buffer_size_; j++){
        result &= at_end_worker[j];
    }
    return result;
}

void Consumer::run(){
    while(!is_workers_finished() || available_elements_->available()){
        available_elements_->acquire();
        Worker *current_worker;
        current_worker = buffer_[i % buffer_size_];
        emit data(id_, current_worker->c_);
        current_worker->was_read = true;
        i++;
        free_elements_->release();
    }
}

void Consumer::change_workers_status(int id){
    int index;
    for(int j = 0; j < buffer_size_; j++)
        if (workers_id[j] == id) index = j;
    at_end_worker[index] = true;
    if(is_workers_finished() && !available_elements_->available()){
        emit my_finished(id_);
        i = 0;
    }
}
