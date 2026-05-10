TEMPLATE = app
TARGET = EasyVPC

QT = core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    awsmanager.cpp \
    guiutil.cpp \
    infrastructurecoordinator.cpp \
    main.cpp \
    loginwindow.cpp \
    rtcard.cpp \
    setupinstructiondialog.cpp \
    subnetcard.cpp \
    vpccard.cpp \
    vpccreationdialog.cpp \
    vpcwindow.cpp

HEADERS += \
    awsmanager.h \
    guiutil.h \
    infrastructurecoordinator.h \
    loginwindow.h \
    rtcard.h \
    setupinstructiondialog.h \
    subnetcard.h \
    vpccard.h \
    vpccreationdialog.h \
    vpcwindow.h

RESOURCES += \
    resources.qrc

INCLUDEPATH += \
    /usr/local/include

LIBS += \
    -L/usr/local/lib -laws-cpp-sdk-ec2 -laws-cpp-sdk-core