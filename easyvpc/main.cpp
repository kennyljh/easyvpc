#include <QApplication>
#include <vpcwindow.h>
#include <QFile>
#include "loginwindow.h"
#include "awsmanager.h"

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    QFile file(":/styles/styles.qss");
    if (file.open(QFile::ReadOnly)) app.setStyleSheet(QLatin1String(file.readAll()));

    AWSManager::instance().initSDK();
    AWSManager::instance().setSelectedProfile("testing-profile");

    // LoginWindow loginWindow;
    // loginWindow.show();

    VPCWindow vpcwindow;
    vpcwindow.show();

    return app.exec();
}