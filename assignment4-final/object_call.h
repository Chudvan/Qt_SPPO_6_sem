#ifndef OBJECT_CALL_H
#define OBJECT_CALL_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QImage>

#include "medianfilter.h"

#include <QQmlApplicationEngine>
#include <QtGui/QGuiApplication>
#include <QtQuick>


class Object_call : public QObject{
    Q_OBJECT
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(float from READ from NOTIFY fromChanged)
    Q_PROPERTY(float count READ count NOTIFY countChanged)
    Q_PROPERTY(QString image_bytes READ image_bytes NOTIFY image_bytesChange)
public:
    explicit Object_call (QObject* parent = 0);
    ~Object_call(){};
    Q_INVOKABLE void btn_start_call(QString path, QString st_number_thread, QString st_radius);
    Q_INVOKABLE QString data;
    Q_INVOKABLE void btn_stop_call();
    //Q_INVOKABLE void save(QObject *item, const QString &path);

    int progress(){ return f_progress; };
    int from() { return f_from; };
    int count() { return f_count; };
    QString image_bytes() { return f_image_bytes; }
signals:
    void progressChanged(int progress);
    void fromChanged(int from);
    void countChanged(int count);
    void image_bytesChange(QString image_bytes);
    void run_cancel();
private slots:
    void futureFinished();
    void progress_setValue(int);
    void progress_setRange(int,int);
private:
    int f_progress = 0;
    int f_from = 0;
    int f_count = 10;
    QString f_image_bytes;

    QImage image;
    MedianFilter* median_filter;
    int * resImageBits;
    int imageHeight = 0;
    int imageWeight = 0;
    QImage returnImage;
    int thread_number = 10;
    int radius = 3;

};


#endif // OBJECT_CALL_H
