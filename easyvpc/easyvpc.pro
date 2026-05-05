TEMPLATE = app
TARGET = EasyVPC

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    awsmanager.cpp \
    main.cpp \
    loginwindow.cpp \
    vpccard.cpp \
    vpcwindow.cpp

HEADERS += \
    awsmanager.h \
    loginwindow.h \
    vpccard.h \
    vpcwindow.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += \
    /usr/local/include

LIBS += \
    -L/usr/local/lib -laws-cpp-sdk-ec2 -laws-cpp-sdk-core