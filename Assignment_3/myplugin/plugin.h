#ifndef PLUGIN_H
#define PLUGIN_H
#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QFileInfo>
#include <QString>

class Plugin : public QObject{

    Q_OBJECT
public:
    virtual ~Plugin(){};
    virtual void test(QString name) = 0;
    virtual void findFilterFiles(QString path) = 0;
    virtual void stop() = 0;

    QFutureWatcher<QFileInfo> watcher;
    QStringList filters;
    bool withDays;
    int days;
    bool withSize;
    int size;
    bool isLessThen;
private:
    QFuture<QFileInfo> future;
};

Q_DECLARE_INTERFACE(Plugin, "ru.mephi.mtp")

#endif // PLUGIN_H
