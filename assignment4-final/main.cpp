#include <QtGui>

#include <QQmlApplicationEngine>
#include <QtGui/QGuiApplication>
#include <QtQuick>

#include "object_call.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{

    QLocale::setDefault(QLocale(QLocale::English,QLocale::UnitedStates));
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QString path = "qrc:/Main.qml";
    qmlRegisterType<Object_call>("com.iban", 1, 0, "Object_call");
    Object_call object_call;

    //object_call.setProperty("data", object_call.data);
    engine.rootContext()->setContextProperty("Object_call", &object_call);
    //engine.rootContext()->setContextObject(&object_call);
    engine.load(QUrl(path));
    return app.exec();

}
