#include "vpcwindow.h"
#include <QMainWindow>
#include <QDockWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QFont>
#include <QStatusBar>
#include <vector>
#include <QDialog>
#include <QList>
#include <QProgressBar>
#include "vpccard.h"
#include "awsmanager.h"
#include "guiutil.h"
#include "vpccreationdialog.h"
#include "vpccreationdialog.h"
#include "infrastructurecoordinator.h"
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

VPCWindow::VPCWindow(QWidget *parent) : QMainWindow(parent){

    connect(&AWSManager::instance(), &AWSManager::vpcsReady,
                this, &VPCWindow::processVPCs);
    connect(&AWSManager::instance(), &AWSManager::apiError,
                this, &VPCWindow::setStatusBar);
    connect(&AWSManager::instance(), &AWSManager::regionsReady,
                this, &VPCWindow::processRegions);
    connect(&AWSManager::instance(), &AWSManager::notifyStatus,
                this, &VPCWindow::setStatusBar);

    resize(1280, 720);

    centralWindow = new QWidget(this);
    centralLayout = new QVBoxLayout(centralWindow);

        topBarFrame = new QFrame(centralWindow);
        topBarFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
        topBarLayout = new QHBoxLayout(topBarFrame);
            easyVPCLabel = new QLabel("EasyVPC", topBarFrame);
            easyVPCLabel->setObjectName("easyVPCLabel");
            taskProgressBar = new QProgressBar(topBarFrame);
            taskProgressBar->setFormat("Idling");
            taskProgressBar->setMinimum(0);
            taskProgressBar->setMaximum(100);
            taskProgressBar->setValue(0);
            regionsCBox = new QComboBox(topBarFrame);
            connect(regionsCBox, &QComboBox::textActivated,
                        this, &VPCWindow::regionChangeTriggered);
        topBarLayout->addWidget(easyVPCLabel);
        topBarLayout->addWidget(taskProgressBar, 1);
        topBarLayout->addWidget(regionsCBox);

        myVPCBarFrame = new QFrame(centralWindow);
        myVPCBarLayout = new QHBoxLayout(myVPCBarFrame);
            welcomeName = new QLabel("My VPCs", myVPCBarFrame);
            QFont qfont;
            qfont.setBold(true);
            qfont.setPointSize(22);
            welcomeName->setFont(qfont);
            refreshBtn = new QPushButton("Refresh", myVPCBarFrame);
            connect(refreshBtn, &QPushButton::clicked, this, &VPCWindow::refreshButtonClicked);
            createBtn = new QPushButton("Create", myVPCBarFrame);
            connect(createBtn, &QPushButton::clicked,
                        this, &VPCWindow::createVPCButtonClicked);
        myVPCBarLayout->addWidget(welcomeName);
        myVPCBarLayout->addStretch();
        myVPCBarLayout->addWidget(refreshBtn);
        myVPCBarLayout->addWidget(createBtn);

        myVPCScrollArea = new QScrollArea(centralWindow);
            myVPCWindow = new QWidget(myVPCScrollArea);
            // myVPCWindow->setStyleSheet("border: 1px solid green");
            myVPCLayout = new QVBoxLayout(myVPCWindow);
            myVPCLayout->setAlignment(Qt::AlignTop);
        myVPCScrollArea->setWidget(myVPCWindow);
        myVPCScrollArea->setWidgetResizable(true);
        myVPCScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    centralLayout->addWidget(topBarFrame);
    centralLayout->addWidget(myVPCBarFrame);
    centralLayout->addWidget(myVPCScrollArea, 1);

    setCentralWidget(centralWindow);

    //initial api call
    AWSManager::instance().getRegionsAsync();
}

void VPCWindow::processRegions(const std::vector<Aws::EC2::Model::Region> &regions){

    regionsCBox->clear();
    for (const Aws::EC2::Model::Region &region : regions){
        regionsCBox->addItem(QString::fromStdString(region.GetRegionName()));
        qDebug() << "Found region " + region.GetRegionName();
    }
    regionsCBox->setCurrentText("us-east-1");
    AWSManager::instance().getVPCsAsync();
}

void VPCWindow::processVPCs(const std::vector<Aws::EC2::Model::Vpc> &vpcs){

    QLayoutItem *item;
    while ((item = myVPCLayout->takeAt(0)) != nullptr){
        if (QWidget *widget = item->widget()) widget->deleteLater();
        delete item;
    }

    for (const Aws::EC2::Model::Vpc &vpc : vpcs){

        QString name = "default VPC";
        for (const auto& tag : vpc.GetTags()){
            if (tag.GetKey() == "Name") name = QString::fromStdString(tag.GetValue());
        }
        QString id = QString::fromStdString(vpc.GetVpcId());
        QString ipv4cidr = QString::fromStdString(vpc.GetCidrBlock());
        QString state = QString::fromStdString(
                            Aws::EC2::Model::VpcStateMapper::GetNameForVpcState(vpc.GetState())
                        );
        qDebug() << "VPC found: " + name + " " + id + " " + ipv4cidr + " " + state;

        VPCCard *card = new VPCCard(name, id, ipv4cidr, state, myVPCWindow);
        myVPCLayout->addWidget(card);
        GUIUtil util;
        util.applyWidgetFade(card, 300);
    }
    statusBar()->showMessage("Found " + QString::number(vpcs.size()) + " VPCs");
}

void VPCWindow::setStatusBar(QString msg){
    qDebug() << "Status Bar: " + msg;
    statusBar()->showMessage(msg);
}

void VPCWindow::refreshButtonClicked(){

    statusBar()->showMessage("Retrieving list of VPCs...");
    AWSManager::instance().getVPCsAsync();
}

void VPCWindow::createVPCButtonClicked(){

    VPCCreationDialog *dialog = new VPCCreationDialog(this);
    InfrastructureCoordinator *coordinator = new InfrastructureCoordinator(this);

    // for debuggin purposes
    connect(dialog, &VPCCreationDialog::VPCCreationRequested,
                this, &VPCWindow::VPCCreationDataDebug);

    // connect to coordinator
    connect(dialog, &VPCCreationDialog::VPCCreationRequested,
                coordinator, &InfrastructureCoordinator::coordinateVPCCreation);

    // inform vpc infrastructure done building
    connect(coordinator, &InfrastructureCoordinator::vpcInfrastructureFinished,
                this, &VPCWindow::setStatusBar);

    // update progress bar on provisioning stage
    connect(coordinator, &InfrastructureCoordinator::coordinatorStageChanged,
                this, &VPCWindow::coordinatorUpdated);

    dialog->exec();
}

void VPCWindow::regionChangeTriggered(const QString &region){

    AWSManager::instance().setSelectedRegion(region);
    statusBar()->showMessage("Changing region to " + region);
    AWSManager::instance().getVPCsAsync();
}

void VPCWindow::coordinatorUpdated(const QString &msg, const int &val){

    taskProgressBar->setFormat(msg);
    taskProgressBar->setValue(val);
}

void VPCWindow::vpcDeletionRequested(const QString &vpcID){

    InfrastructureCoordinator *coordinator = new InfrastructureCoordinator(this);
    coordinator->coordinateVPCDeletion(vpcID);

    // update progress bar on provisioning stage
    connect(coordinator, &InfrastructureCoordinator::coordinatorStageChanged,
                this, &VPCWindow::coordinatorUpdated);
}

void VPCWindow::VPCCreationDataDebug(
            QString &vpcName,
            QString &vpcCIDR,
            const QList<VPCCreationDialog::subnetInfo> &subnetInfos,
            QString &igwName,
            const QList<VPCCreationDialog::RTInfo> &RTInfos){

    qDebug() << "VPC Name:" << vpcName;
    qDebug() << "VPC CIDR:" << vpcCIDR;
    qDebug() << "IGW Name:" << igwName;

    qDebug() << "Subnets:";
    for (const auto &subnet : subnetInfos) {
        qDebug() << "  Name:" << subnet.name
                 << "Zone:" << subnet.zone
                 << "IPv4:" << subnet.ipv4;
    }

    qDebug() << "Route Tables:";
    for (const auto &rt : RTInfos) {
        qDebug() << "  RT Name:" << rt.name
                 << "Subnets:" << rt.subnets.join(", ");
    }
}