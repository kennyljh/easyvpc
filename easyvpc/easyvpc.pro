TEMPLATE = app
TARGET = EasyVPC

QT = core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    awsmanager.cpp \
    guiutil.cpp \
    main.cpp \
    loginwindow.cpp \
    subnetcard.cpp \
    vpccard.cpp \
    vpcwindow.cpp

HEADERS += \
    awsmanager.h \
    guiutil.h \
    loginwindow.h \
    subnetcard.h \
    vpccard.h \
    vpcwindow.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += \
    /usr/local/include

LIBS += \
    -L/usr/local/lib -laws-cpp-sdk-ec2 -laws-cpp-sdk-core