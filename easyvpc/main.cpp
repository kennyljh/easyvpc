#include <QApplication>
#include <vpcwindow.h>
#include <QFile>
#include <QObject>
#include "loginwindow.h"
#include "awsmanager.h"

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    QFile file(":/styles/styles.qss");
    if (file.open(QFile::ReadOnly)) app.setStyleSheet(QLatin1String(file.readAll()));

    AWSManager::instance().initSDK();

    LoginWindow loginWindow;
    VPCWindow *vpcwindow = nullptr;

    loginWindow.show();

    // window switching on successful login
    QObject::connect(&loginWindow, &LoginWindow::loginIsSuccessful,
                        [&](){
        loginWindow.close();

        vpcwindow = new VPCWindow();
        vpcwindow->show();
    });

    return app.exec();
}