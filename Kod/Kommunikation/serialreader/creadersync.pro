QT = core

greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += serialport
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

CONFIG += console
CONFIG -= app_bundle

TARGET = creaderasync
TEMPLATE = app

HEADERS += \
    serialport.h \
    order.h

SOURCES += \
    main.cpp \
    serialport.cpp \
    order.cpp
