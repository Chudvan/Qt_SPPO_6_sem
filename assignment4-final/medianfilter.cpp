#include "medianfilter.h"
#include <QDebug>

MedianFilter* MedianFilter::run_static = NULL;

MedianFilter::MedianFilter(QObject *parent) : QObject(parent){
    futureWatcher = new QFutureWatcher<void>(this);
    connect(futureWatcher, SIGNAL(finished()), this, SLOT(finished_slot()));
    qDebug() << "MedianFilter: connected";
    run_static = this;
}

MedianFilter::~MedianFilter(){
    if (futureWatcher->isRunning()){
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }
    if(extensionImage){
        delete [] extensionImage;
    }
}

void MedianFilter::call_stopMedianFilter(){
    if (futureWatcher->isRunning()){
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }
}

void MedianFilter::getPixelValue(IndexPair &p){
    int k = 0;
    int window[windowSize];

    int workingWeight = originImageweight + 2 * radius;

    for (int i = p.i - radius; i < p.i + radius+1; i++){
        for (int j = p.j - radius; j < p.j + radius + 1; j++){
            window[k++] = this->extensionImage[i * (workingWeight) + j];
        }
    }

    for (int j = 0; j <= windowSizeHalf; j++)
    {
        int min = j;
        for (int l = j + 1; l < windowSize; l++){
            if (window[l] < window[min]){ min = l; }
        }
        const int  el = window[j];
        window[j] = window[min];
        window[min] = el;
    }

    resImage[(p.i - radius) * originImageweight + (p.j - radius)] = window[windowSizeHalf];
}

void MedianFilter::run_filtering(int  *i, int  *res, int imageHeight, int imageWidth, int r){
    if (!i || imageHeight < 1 || imageWidth < 1){
        return;
    }

    if (futureWatcher->isRunning()){
        futureWatcher->cancel();
        futureWatcher->waitForFinished();
    }

    this->originImageweight = imageWidth;
    this->radius = r;
    image = i;

    int extWidth = imageWidth + 2 * radius;
    int extHeight = imageHeight + 2*radius;
    windowSize = (2*radius+1) * (2*radius+1);
    windowSizeHalf = windowSize / 2;

    extensionImage = new int [extWidth * extHeight];
    if (!extensionImage){
        return;
    }
    resImage = res;

    for (int i = 0; i < imageHeight; i++){
        memcpy(extensionImage + extWidth * (i + radius) + radius, image + imageWidth * i, imageWidth * sizeof(int ));
        memcpy(extensionImage + extWidth * (i + radius), image + (imageWidth * i), radius * sizeof(int ));
        memcpy(extensionImage + (extWidth * (i + radius + 1) - radius), image + imageWidth * (i + 1) - radius, radius * sizeof(int ));
    }

    for(int i = 0; i < radius; i++){
        memcpy(extensionImage + i * extWidth, extensionImage + extWidth * (i + imageHeight - 1), extWidth * sizeof(int ));
        memcpy(extensionImage + extWidth * (imageHeight + radius + i), extensionImage + extWidth * (imageHeight + radius - i - 1), extWidth * sizeof(int ));
    }

    for (int i = radius; i < imageHeight + radius; i++){
        for (int j = radius; j < imageWidth + radius; j++){
            IndexPair p(i,j); indexes << p;
        }
    }
    //static void (MedianFilter::*func)(IndexPair &p);
    //func = &MedianFilter::getPixelValue;

    //(this->*func)(indexes[0]);



    qDebug() << "main::: pixel length" << indexes.length();
    auto future = QtConcurrent::map(indexes, runner);
    futureWatcher->setFuture( future );

    //futureWatcher->waitForFinished();
    // continue async; wait result later

}



