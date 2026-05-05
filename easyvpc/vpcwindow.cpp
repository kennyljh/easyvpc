#include "vpcwindow.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QFont>
#include "vpccard.h"
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>

VPCWindow::VPCWindow(QWidget *parent) : QMainWindow(parent){

    resize(1280, 720);

    centralWindow = new QWidget(this);
    centralLayout = new QVBoxLayout(centralWindow);

        topBarFrame = new QFrame(centralWindow);
        topBarFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
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
            sortBtn = new QPushButton("Sort", myVPCBarFrame);
            expandAllBtn = new QPushButton("Expand All", myVPCBarFrame);
        myVPCBarLayout->addWidget(welcomeName);
        myVPCBarLayout->addStretch();
        myVPCBarLayout->addWidget(sortBtn);
        myVPCBarLayout->addWidget(expandAllBtn);

        myVPCScrollArea = new QScrollArea(centralWindow);
            myVPCWindow = new QWidget(myVPCScrollArea);
            // myVPCWindow->setStyleSheet("border: 1px solid green");
            myVPCLayout = new QVBoxLayout(myVPCWindow);
            myVPCLayout->setAlignment(Qt::AlignTop);
                myVPCLayout->addWidget(new VPCCard("temp", "123456", "10.2.4.5"));
                myVPCLayout->addWidget(new VPCCard("temp2", "123456", "10.2.4.5"));
                myVPCLayout->addWidget(new VPCCard("temp2", "123456", "10.2.4.5"));
                myVPCLayout->addWidget(new VPCCard("temp2", "123456", "10.2.4.5"));
                myVPCLayout->addWidget(new VPCCard("temp2", "123456", "10.2.4.5"));
                myVPCLayout->addWidget(new VPCCard("temp2", "123456", "10.2.4.5"));
                myVPCLayout->addWidget(new VPCCard("temp2", "123456", "10.2.4.5"));
        myVPCScrollArea->setWidget(myVPCWindow);
        myVPCScrollArea->setWidgetResizable(true);
        myVPCScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    centralLayout->addWidget(topBarFrame);
    centralLayout->addWidget(myVPCBarFrame);
    centralLayout->addWidget(myVPCScrollArea, 1);

    setCentralWidget(centralWindow);

}

void VPCWindow::processVPCs(){

}