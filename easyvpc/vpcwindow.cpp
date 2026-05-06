#include "vpcwindow.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QFont>
#include <QStatusBar>
#include <vector>
#include "vpccard.h"
#include "awsmanager.h"
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

VPCWindow::VPCWindow(QWidget *parent) : QMainWindow(parent){

    connect(&AWSManager::instance(), &AWSManager::vpcsReady,
                this, &VPCWindow::processVPCs);
    connect(&AWSManager::instance(), &AWSManager::apiError,
                this, &VPCWindow::setStatusBar);

    resize(1280, 720);

    centralWindow = new QWidget(this);
    centralLayout = new QVBoxLayout(centralWindow);

        topBarFrame = new QFrame(centralWindow);
        topBarFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
        topBarLayout = new QHBoxLayout(topBarFrame);
            backToHubBtn = new QPushButton("EasyVPC", topBarFrame);
            regionsCBox = new QComboBox(topBarFrame);
        topBarLayout->addWidget(backToHubBtn);
        topBarLayout->addStretch();
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
            sortBtn = new QPushButton("Sort", myVPCBarFrame);
            expandAllBtn = new QPushButton("Expand All", myVPCBarFrame);
        myVPCBarLayout->addWidget(welcomeName);
        myVPCBarLayout->addStretch();
        myVPCBarLayout->addWidget(refreshBtn);
        myVPCBarLayout->addWidget(sortBtn);
        myVPCBarLayout->addWidget(expandAllBtn);

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

        myVPCLayout->addWidget(new VPCCard(name, id, ipv4cidr, state, myVPCWindow));
    }
    statusBar()->showMessage("Found " + QString::number(vpcs.size()) + " VPCs");
}

void VPCWindow::setStatusBar(QString msg){
    statusBar()->showMessage(msg);
}

void VPCWindow::refreshButtonClicked(){

    statusBar()->showMessage("Retrieving list of VPCs...");
    AWSManager::instance().getVPCsAsync();
}