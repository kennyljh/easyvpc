#include "loginwindow.h"
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QSize>
#include <QComboBox>
#include <QStringList>
#include <QString>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent){

    foundProfiles = new QStringList();

    resize(1280, 720);

    centralWindow = new QWidget(this);
        centralLayout = new QVBoxLayout(centralWindow);
            connectBtn = new QPushButton("Connect to AWS", centralWindow);
            connectBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            connect(connectBtn, &QPushButton::clicked, this, &LoginWindow::loginButtonClicked);

            selectProfileWindow = new QWidget(centralWindow);
            selectProfileWindow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                selectProfileLayout = new QVBoxLayout(selectProfileWindow);
                    profilesLineEdt = new QLabel("Profiles found:");
                    profilesLineEdt->setStyleSheet("padding-left: 10px");
                    profilesLineEdt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                    CBoxBtnWindow = new QWidget(selectProfileWindow);
                        CBoxBtnLayout = new QHBoxLayout(CBoxBtnWindow);
                            profilesCBox = new QComboBox(CBoxBtnWindow);
                            profilesCBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                            goBtn = new QPushButton("Go", CBoxBtnWindow);
                        CBoxBtnLayout->addWidget(profilesCBox, 0);
                        CBoxBtnLayout->addWidget(goBtn, 0);
                selectProfileLayout->addWidget(profilesLineEdt, 0);
                selectProfileLayout->addWidget(CBoxBtnWindow, 0);
            selectProfileWindow->setHidden(true);
        centralLayout->addWidget(connectBtn, 0, Qt::AlignCenter);
        centralLayout->addWidget(selectProfileWindow, 0, Qt::AlignCenter);
        centralLayout->setSpacing(0);
    setCentralWidget(centralWindow);
}

void LoginWindow::loginButtonClicked(){

    getAWSProfiles(*foundProfiles);

    if (!foundProfiles->isEmpty()) {
        profilesCBox->clear();
        profilesCBox->addItems(*foundProfiles);
        selectProfileWindow->setHidden(false);
    }
}

void LoginWindow::getAWSProfiles(QStringList &profiles){

    QString configPath = QDir::homePath() + "/.aws/config";
    QFileInfo file(configPath);
    if (!file.exists()) return;

    QSettings settings(configPath, QSettings::IniFormat);
    QStringList keys = settings.allKeys();

    for (const QString &key : keys){

        qDebug() << "found profile: " << key;
        if (key.startsWith("profile ")){

            QString profileName = key.section(".", 0, 0);
            profileName.remove("profile ");
            profileName = profileName.section('/', 0, 0);
            qDebug() << "filered profile name: " << profileName;
            if (!profiles.contains(profileName)) profiles.append(profileName);
        }
    }

    if (settings.allKeys().contains("default/region") || settings.allKeys().contains("output")){
        if (!profiles.contains("default")) profiles.prepend("default");
    }
}














