#include "myplugin.h"
#include <QTest>

bool Myplugin::filtration(const QFileInfo& file)
{
    QTest::qWait(1);
    bool result = true;

    if(!::filters.isEmpty()){
        result = false;
        foreach (QString filter, ::filters) {
            if(file.filePath().contains(filter.remove(0, 1))){
                result = true;
                break;
            }
        }
    }
    if(::withDays){
        result &= file.birthTime() > QDateTime::currentDateTime().addDays(-::days);
    }
    if(::withSize){
        if(::isLessThen){
            result &= file.size() < ::size;
        }
        else{
            result &= file.size() > ::size;
        }
    }
    return result;
}

Myplugin::Myplugin(QObject* parent)
{
    Q_UNUSED(parent);

    connect(&watcher, &QFutureWatcher<QFileInfo>::finished, this, &Myplugin::clearLists);
    connect(&watcher, &QFutureWatcher<QFileInfo>::canceled, this, &Myplugin::clearLists);
    connect(&watcher, &QFutureWatcher<QFileInfo>::resultReadyAt, this, &Myplugin::addToFilteredFiles);
}

Myplugin::~Myplugin(){

}

void Myplugin::test(QString name){
    qInfo() << "Test from plugin:" << name;
}
void Myplugin::findFilterFiles(QString path){
    ::filters = filters;
    ::withDays = withDays;
    ::days = days;
    ::withSize = withSize;
    ::size = size;
    ::isLessThen = isLessThen;
    isStopped = false;
    isFilesGet = false;

    QFileInfoList dirs;
    QFileInfo currentDirFileInfo(path);
    dirs.append(currentDirFileInfo);
    QDir currentDir;

    timer.start();
    qDebug() << "Start find files:";

    while(!isStopped && !dirs.isEmpty()){
        currentDir = QDir(dirs.takeFirst().filePath());
        files += currentDir.entryInfoList(QDir::Files);
        dirs += currentDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    }

    if(dirs.isEmpty())
        isFilesGet = true;

    qDebug() << "Find not filter files:" << files.length()  << "isStopped:" << isStopped << "isFilesGet:" << isFilesGet;

    qDebug() << "Time elapsed:" << timer.elapsed() << "ms";

    if(!isStopped){
        timer.start();
        qDebug() << "Start filter files:";
        future = QtConcurrent::filtered(files, filtration);
        watcher.setFuture(future);
    }
}

void Myplugin::stop(){
    isStopped = true;
    if(!watcher.isFinished())watcher.cancel();
}

void Myplugin::clearLists(){
    if(watcher.isCanceled()){
        qDebug() << "Was canceled:";
    }
    else if(watcher.isFinished()){
        qDebug() << "Was finished:";
    }
    qDebug() << "Time elapsed:" << timer.elapsed() << "ms";
    qDebug() << "Find filter files:" << filteredFiles.length();
    files = QFileInfoList();
    filteredFiles = QFileInfoList();
}

void Myplugin::addToFilteredFiles(int index){
    filteredFiles.append(watcher.resultAt(index));
}
