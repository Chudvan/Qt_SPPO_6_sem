#include "producer.h"

Producer::Producer(int buffer_size, Worker **buffer, QSemaphore *free_elements,
                   QSemaphore *available_elements, bool *at_end, int id)
    : buffer_size_(buffer_size), buffer_(buffer),
      free_elements_(free_elements), available_elements_(available_elements),
      at_end_(at_end), id_(id), is_always_percent(true)
{

}

void Producer::set_file_name(QString file_name){
    file_name_ = file_name;
}

void Producer::run(){
    QFile f(file_name_);
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);
    text_ = ts.readAll().toLocal8Bit();
    len_text = text_.length();

    Worker *current_woker;
    int percent = 0;

    if (len_text > MAX_FILE_SIZE)is_always_percent = false;

    for (int i = 0; i < len_text; i++){
        free_elements_->acquire();
        current_woker = buffer_[i % buffer_size_];
        current_woker->mutex->lock();
        current_woker->c_ = text_[i];
        current_woker->is_changed = true;

        if (!is_always_percent){
            if(i % 100 == 0){
                percent = QVariant::fromValue(((double)(i + 1) / len_text) * 100).toInt();
                emit update_progress(id_, percent);
            }
        }
        else{
            percent = QVariant::fromValue(((double)(i + 1) / len_text) * 100).toInt();
            emit update_progress(id_, percent);
        }
        if (i == len_text - 1) *at_end_ = true;
        current_woker->mutex->unlock();
    }
    f.close();
}
