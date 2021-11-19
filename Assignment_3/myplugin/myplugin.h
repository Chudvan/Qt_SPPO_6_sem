#ifndef MYPLUGIN_H
#define MYPLUGIN_H

#include "myplugin_global.h"
#include "plugin.h"
#include <QString>
#include <QObject>
#include <QTime>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QtConcurrent/QtConcurrentFilter>

static QStringList filters;
static bool withDays;
static int days;
static bool withSize;
static int size;
static bool isLessThen;

class MYPLUGIN_EXPORT Myplugin : public Plugin
{

    Q_OBJECT

    Q_PLUGIN_METADATA(IID "ru.mephi.mtp")
    Q_INTERFACES(Plugin)

    static bool filtration(const QFileInfo& file);

    bool isFilesGet;
    QFileInfoList files;
    QFileInfoList filteredFiles;

    QTime timer;
public:
    explicit Myplugin(QObject* parent = nullptr);
    ~Myplugin();
    void test(QString name);
    void findFilterFiles(QString path);
    void stop();

    bool isStopped;
    QFuture<QFileInfo> future;

public slots:
    void clearLists();
    void addToFilteredFiles(int index);
};

#endif // MYPLUGIN_H
