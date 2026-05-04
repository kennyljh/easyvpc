#include <QApplication>
#include "loginwindow.h"
#include <vpcwindow.h>
#include <QFile>

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    QFile file(":/styles/styles.qss");
    if (file.open(QFile::ReadOnly)) app.setStyleSheet(QLatin1String(file.readAll()));

    // LoginWindow loginWindow;
    // loginWindow.show();

    VPCWindow vpcwindow;
    vpcwindow.show();

    return app.exec();
}