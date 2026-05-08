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
#include <QProcess>
#include <QStatusBar>
#include <QFont>
#include <QFrame>
#include "awsmanager.h"
#include "guiutil.h"
#include "setupinstructiondialog.h"

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent){

    GUIUtil util;
    foundProfiles = new QStringList();

    QFont qfontB20;
    qfontB20.setPointSize(20);
    qfontB20.setBold(true);

    QFont qfont12;
    qfont12.setPointSize(12);

    resize(1280, 720);

    centralWindow = new QWidget(this);
        centralLayout = new QVBoxLayout(centralWindow);
            topBarFrame = new QFrame(centralWindow);
            topBarFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            topBarLayout = new QHBoxLayout(topBarFrame);
                helpBtn = new QPushButton("Help", topBarFrame);
                connect(helpBtn, &QPushButton::clicked,
                            this, &LoginWindow::helpButtonClicked);
                helpBtn->setFont(qfont12);
            topBarLayout->addWidget(helpBtn);
            topBarLayout->setAlignment(Qt::AlignRight);

            connectFrame = new QFrame(centralWindow);
            connectLayout = new QHBoxLayout(connectFrame);
                connectBtn = new QPushButton("Connect to AWS", centralWindow);
                connectBtn->setFont(qfontB20);
                connectBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                connect(connectBtn, &QPushButton::clicked, this, &LoginWindow::loginButtonClicked);
            connectLayout->addWidget(connectBtn);
            connectLayout->setAlignment(Qt::AlignCenter);

            selectProfileWindow = new QWidget(centralWindow);
                selectProfileLayout = new QVBoxLayout(selectProfileWindow);
                    profilesLabel = new QLabel("Profiles found:");
                    profilesLabel->setFont(qfont12);
                    profilesLabel->setStyleSheet("padding-left: 5px");
                    profilesLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                    CBoxBtnWindow = new QWidget(selectProfileWindow);
                        CBoxBtnLayout = new QHBoxLayout(CBoxBtnWindow);
                            profilesCBox = new QComboBox(CBoxBtnWindow);
                            profilesCBox->setFont(qfont12);
                            profilesCBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                            goBtn = new QPushButton("Go", CBoxBtnWindow);
                            goBtn->setFont(qfont12);
                            connect(goBtn, &QPushButton::clicked, this, &LoginWindow::goButtonClicked);
                        CBoxBtnLayout->addWidget(profilesCBox, 0);
                        CBoxBtnLayout->addWidget(goBtn, 0);
                selectProfileLayout->addWidget(profilesLabel);
                selectProfileLayout->addWidget(CBoxBtnWindow);
                selectProfileLayout->setAlignment(Qt::AlignCenter);
            selectProfileWindow->setHidden(true);
        centralLayout->addWidget(topBarFrame);
        centralLayout->addWidget(connectFrame, 1);
        centralLayout->addWidget(selectProfileWindow, 1);
        centralLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    setCentralWidget(centralWindow);
    util.applyWidgetFade(centralWindow, 300);
}

void LoginWindow::loginButtonClicked(){

    GUIUtil util;
    getAWSProfiles(*foundProfiles);

    if (!foundProfiles->isEmpty()) {
        profilesCBox->clear();
        profilesCBox->addItems(*foundProfiles);
        selectProfileWindow->setHidden(false);
        util.applyWidgetFade(selectProfileWindow, 300);
    }
}

void LoginWindow::goButtonClicked(){

    loginAWSProfile(profilesCBox->currentText());
}

void LoginWindow::helpButtonClicked(){

    SetupInstructionDialog dialog(this);
    dialog.exec();
}

void LoginWindow::loginAWSProfile(QString selectedProfile){

    int result = QProcess::execute("aws", {"sso", "login", "--profile", selectedProfile});
    if (result == 0) {
        AWSManager::instance().setSelectedProfile(selectedProfile);
        emit loginIsSuccessful();
    }
    else statusBar()->showMessage("Login failed");
}

/**
 * @brief LoginWindow::getAWSProfiles Begins async QProcess to login selected
 * AWS profile
 * @param profiles
 */
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














