#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include <QtConcurrent>
#include <QObject>

struct IndexPair{
    int i;
    int j;
    IndexPair(int a, int b)
    {
        i = a;
        j = b;
    }
};

class MedianFilter : public QObject{
    Q_OBJECT
public:
    void getPixelValue(IndexPair &p);
    static void runner(IndexPair &p){
        (run_static->getPixelValue)(p);
    }
    explicit MedianFilter(QObject *parent = 0);
    ~MedianFilter();
    void run_filtering(int  *i,
                       int  *res,
                       const int imageHeight,
                       const int imageWidth,
                       const int r);

    QFutureWatcher<void> *futureWatcher;

private:
    static MedianFilter *run_static;
    QList<IndexPair> indexes;
    int windowSize;

    int originImageweight;
    int *resImage;
    int radius;

public slots:
    void call_stopMedianFilter();
    void finished_slot(){
        qDebug() << "finished";
    };

protected:
    int windowSizeHalf;
    int *extensionImage;
    int *image;
};



#endif
