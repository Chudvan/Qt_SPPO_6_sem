#include "object_call.h"
#include <QPixmap>
#include <QGraphicsObject>
#include <QGraphicsItem>


void Object_call::btn_start_call(QString path, QString st_number_thread, QString st_radius)
{
    thread_number = st_number_thread.toUInt();
    radius = st_radius.toUInt();
    QThreadPool::globalInstance()->setMaxThreadCount(thread_number);
    qDebug() << "New thread max number" << thread_number;
    qDebug() << "New radius " << radius;

    path = path.split(":///")[1];
    qDebug() << "object_call" << path;
    QImage image(path);
    this->image = image;

    if (radius > 30 || radius <= 0){
        qDebug() << "bad filterRadius" << radius;
        return;
    }

    if (image.isNull()){
        qDebug() << "bad image sorce";
        return;
    }

    imageHeight = image.height();
    imageWeight = image.width();
    qDebug() << "size: w, h" << imageHeight << imageWeight;

    if (radius > imageHeight/2 || radius >imageWeight/2)    {
        qDebug() << "bad image size, radius" << radius;
        return;
    }

    median_filter = new MedianFilter();


    connect(median_filter->futureWatcher, SIGNAL(progressRangeChanged(int,int)), this, SLOT(progress_setRange(int,int)));
    connect(this, SIGNAL(canceled()), median_filter, SLOT(cancelMedianFilter()));
    qDebug() << "connected";

    connect(this, SIGNAL(run_cancel()), median_filter, SLOT(call_stopMedianFilter()));
    connect(median_filter->futureWatcher, SIGNAL(progressValueChanged(int)), this, SLOT(progress_setValue(int)));

    resImageBits = new int [imageHeight * imageWeight];

    if (!resImageBits){
        return;
    }

    connect(median_filter->futureWatcher, SIGNAL(finished()), this, SLOT(futureFinished()));
    median_filter->run_filtering((int *)image.bits(),
                                 resImageBits,
                                 imageHeight,
                                 imageWeight,
                                 radius);
    qDebug() << "run async";
}

void Object_call::btn_stop_call()
{
    qDebug() << "Cancel filter call";
    emit run_cancel();
}


void Object_call::futureFinished()
{
    qDebug() << "futureFinished";
    if(!resImageBits){
        qDebug() << "!bad resImageBits";
        return;
    }
    QImage returnImage((uchar*)resImageBits, imageWeight, imageHeight, image.format());
    this->returnImage = returnImage;

    //qDebug() << "Save image: " << QQmlProperty(this, "destImage").write(this->returnImage);

    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    this->returnImage.save(&buffer, "JPEG");

    QString image_bytes("data:image/jpg;base64,");
    image_bytes.append(QString::fromLatin1(bArray.toBase64().data()));

    qDebug() << "!!out result bytes:" << image_bytes.length();
    this->f_image_bytes = image_bytes;
    emit image_bytesChange(image_bytes);

    delete [] resImageBits;
    delete median_filter;
    //return image_bytes;
}

Object_call::Object_call(QObject *parent)
    : QObject(parent)
{
}

void Object_call::progress_setValue(int value)
{
    this->f_progress = value;
    //qDebug() << "progress_setValue" << value;
    emit progressChanged(value);
}

void Object_call::progress_setRange(int from, int to)
{
     qDebug() << "progress_setRange" << from << to;
     this->f_from = from;
     this->f_count = to;
     emit fromChanged(from);
     emit countChanged(to);
}

