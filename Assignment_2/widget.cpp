#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    qDebug() << "GUI Thread" << QThread::currentThread();

    n = {2, 2, 1}; // number of workers for current color
    order_of_workers = {1, 3, 4, 5, 2}; // n[0] == 2 => (1, 3), n[1] == 2 => (4, 5), n[2] == 1 => (1,)
    n_of_producers = n.length();

    n_of_workers = 0;
    for (int i = 0; i < n_of_producers ; i++)
        n_of_workers += n[i];

    QVector<QString> v = {"pink", "yellow", "darkblue"};
    QString left("QLabel { background-color : "), right(";}");
    QString color_1 = left + v[0] + right;
    QString color_2 = left + v[1] + right;
    QString color_3 = left + v[2] + right;
    ui->lineEdit_1->setStyleSheet(color_1);
    ui->label_1->setStyleSheet(color_1);
    ui->label_3->setStyleSheet(color_1);
    ui->lineEdit_2->setStyleSheet(color_2);
    ui->label_4->setStyleSheet(color_2);
    ui->label_5->setStyleSheet(color_2);
    ui->lineEdit_3->setStyleSheet(color_3);
    ui->label_2->setStyleSheet(left + v[2] + "; color : white" + right);

    at_end_ = new bool[n_of_producers];
    for (int i = 0; i < n_of_producers; i++)
        at_end_[i] = false;

    at_end_consumers = new bool[n_of_producers];
    for (int i = 0; i < n_of_producers; i++)
        at_end_consumers[i] = false;

    free_elements_ = new QSemaphore*[n_of_producers];
    for (int i = 0; i < n_of_producers; i++)
        free_elements_[i] = new QSemaphore(n[i]);
    available_elements_ = new QSemaphore*[n_of_producers];
    for (int i = 0; i < n_of_producers; i++)
        available_elements_[i] = new QSemaphore(0);

    done = false;

    buffer = new Worker*[n_of_workers];
    producers = new Producer*[n_of_producers];
    consumers = new Consumer*[n_of_producers];
    Worker **current_buffer = new Worker*[n[0]];

    int j = 0;
    for (int i = 0; i < n[0]; i++){
        buffer[order_of_workers[i] - 1] = new Worker(available_elements_[0], &at_end_[0], order_of_workers[i] - 1);
        current_buffer[j] = buffer[order_of_workers[i] - 1];
        j++;
    }
    producers[0] = new Producer(n[0], current_buffer, free_elements_[0], available_elements_[0], &at_end_[0], 0);
    consumers[0] = new Consumer(n[0], current_buffer, free_elements_[0], available_elements_[0], 0);

    current_buffer = new Worker*[n[1]];
    j = 0;
    for (int i = n[0]; i < n[0] + n[1]; i++){
        buffer[order_of_workers[i] - 1] = new Worker(available_elements_[1], &at_end_[1], order_of_workers[i] - 1);
        current_buffer[j] = buffer[order_of_workers[i] - 1];
        j++;
    }
    producers[1] = new Producer(n[1], current_buffer, free_elements_[1], available_elements_[1], &at_end_[1], 1);
    consumers[1] = new Consumer(n[1], current_buffer, free_elements_[1], available_elements_[1], 1);

    current_buffer = new Worker*[n[2]];
    j = 0;
    for (int i = n[0] + n[1]; i < n[0] + n[1] + n[2]; i++){
        buffer[order_of_workers[i] - 1] = new Worker(available_elements_[2], &at_end_[2], order_of_workers[i] - 1);
        current_buffer[j] = buffer[order_of_workers[i] - 1];
        j++;
    }
    producers[2] = new Producer(n[2], current_buffer, free_elements_[2], available_elements_[2], &at_end_[2], 2);
    consumers[2] = new Consumer(n[2], current_buffer, free_elements_[2], available_elements_[2], 2);

    file_names = QVector<QString>(n_of_producers);

    for (int i = 0; i < n_of_producers; i++){
        qDebug() << "Consumer connect 1" << connect(consumers[i], &Consumer::data, this, &Widget::update_info_consumer);
        qDebug() << "Consumer connect 2" << connect(consumers[i], &Consumer::my_finished, this, &Widget::change_status);
        qDebug() << "Producer connect" << connect(producers[i], &Producer::update_progress, this, &Widget::update_progress);
    }

    for(int i = 0; i < n_of_workers; i++){
        qDebug() << "Worker connect" << connect(buffer[i], &Worker::data, this, &Widget::update_info_worker);
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::clear_output(){
    ui->textEdit_1->setText("");
    ui->textEdit_2->setText("");
    ui->textEdit_3->setText("");

    ui->label_1->setText("");
    ui->label_2->setText("");
    ui->label_3->setText("");
    ui->label_4->setText("");
    ui->label_5->setText("");

    ui->progressBar_1->setValue(0);
    ui->progressBar_2->setValue(0);
    ui->progressBar_3->setValue(0);
}

void Widget::on_start_clicked()
{
    if (done){
        clear_output();
        done = false;
    }

    for (int i = 0; i < n_of_workers; i++)
        buffer[i]->start();

    for (int i = 0; i < n_of_producers; i++){
        producers[i]->set_file_name(file_names[i]);
        producers[i]->start();
        consumers[i]->start();
    }
}

void Widget::on_choose_file_1_clicked()
{
    QString cur_file_name = QFileDialog::getOpenFileName(this,tr("Open Directory"),
                                                          file_names[0].isEmpty() ? "\\home" : file_names[0]);
    if(!cur_file_name.isEmpty()){
        ui->lineEdit_1->setText(cur_file_name);
        file_names[0] = cur_file_name;
    }
}

void Widget::on_choose_file_2_clicked()
{
    QString cur_file_name = QFileDialog::getOpenFileName(this,tr("Open Directory"),
                                                         file_names[1].isEmpty() ? "\\home" : file_names[1]);
    if(!cur_file_name.isEmpty()){
        ui->lineEdit_2->setText(cur_file_name);
        file_names[1] = cur_file_name;
    }
}

void Widget::on_choose_file_3_clicked()
{
    QString cur_file_name = QFileDialog::getOpenFileName(this,tr("Open Directory"),
                                                         file_names[2].isEmpty() ? "\\home" : file_names[2]);
    if(!cur_file_name.isEmpty()){
        ui->lineEdit_3->setText(cur_file_name);
        file_names[2] = cur_file_name;
    }
}

void Widget::update_info_consumer(int id, char c){
    QString c_ = QString(c);
    QString current_text;
    switch(id){
    case 0:
        current_text = ui->textEdit_1->toPlainText();
        ui->textEdit_1->setText(current_text + c_);
        break;
    case 1:
        current_text = ui->textEdit_2->toPlainText();
        ui->textEdit_2->setText(current_text + c_);
        break;
    case 2:
        current_text = ui->textEdit_3->toPlainText();
        ui->textEdit_3->setText(current_text + c_);
        break;
    }
}

void Widget::update_info_worker(int id, char c){
    QString c_ = QString(c);
    switch(id){
    case 0:
        ui->label_1->setText(c_);
        break;
    case 1:
        ui->label_2->setText(c_);
        break;
    case 2:
        ui->label_3->setText(c_);
        break;
    case 3:
        ui->label_4->setText(c_);
        break;
    case 4:
        ui->label_5->setText(c_);
        break;
    }
}

void Widget::update_progress(int id, int percent){
    switch(id){
    case 0:
        ui->progressBar_1->setValue(percent);
        break;
    case 1:
        ui->progressBar_2->setValue(percent);
        break;
    case 2:
        ui->progressBar_3->setValue(percent);
        break;
    }
}

bool Widget::is_full_done(){
    bool result = true;
    for (int i = 0; i < n_of_producers; i++){
        result &= at_end_consumers[i];
    }
    return result;
}

void Widget::change_status(int id){
    at_end_consumers[id] = true;
    if(is_full_done()){
        for (int i = 0; i < n_of_producers; i++){
            at_end_[i] = false;
            at_end_consumers[id] = false;
            for (int j = 0; j < n[i]; j++)
                consumers[i]->at_end_worker[j] = false;
        }
    }
    done = true;
}
