QT      += core \
            gui \
            widgets \
            qml \
            quick\
            concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    medianfilter.cpp \
    object_call.cpp

HEADERS += \
    medianfilter.h \
    object_call.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
