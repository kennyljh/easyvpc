TEMPLATE = app
TARGET = EasyVPC

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    loginwindow.cpp

HEADERS += \
    loginwindow.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += \
    /usr/local/include

LIBS += \
    -L/usr/local/lib -laws-cpp-sdk-ec2 -laws-cpp-sdk-core