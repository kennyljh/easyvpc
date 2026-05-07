#include "vpccreationdialog.h"

#include <QWidget>
#include <QDialog>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QAbstractItemView>

VPCCreationDialog::VPCCreationDialog(QWidget *parent)
                  : QDialog(parent){

    setWindowTitle("Create  VPC");
    resize(650, 680);

    mainLayout = new QVBoxLayout(this);
        createVPCScrollArea = new QScrollArea(this);
        createVPCFrame = new QFrame(createVPCScrollArea);
        createVPCLayout = new QVBoxLayout(createVPCFrame);
            vpcNameLabel = new QLabel("VPC Name", createVPCFrame);
            vpcNameEdt = new QLineEdit(createVPCFrame);
            vpcNameEdt->setPlaceholderText("my-vpc-01");
            vpcIPv4CIDRLabel = new QLabel("VPC IPv4 CIDR", createVPCFrame);
            vpcIPv4CIDREdt = new QLineEdit(createVPCFrame);
            vpcIPv4CIDREdt->setPlaceholderText("10.0.0.0/24");
        createVPCLayout->addWidget(vpcNameLabel);
        createVPCLayout->addWidget(vpcNameEdt);
        createVPCLayout->addWidget(vpcIPv4CIDRLabel);
        createVPCLayout->addWidget(vpcIPv4CIDREdt);

            subnetsLabel = new QLabel("Subnets", createVPCFrame);
            subnetsFrame = new QFrame(createVPCFrame);
            subnetsLayout = new QVBoxLayout(subnetsFrame);
                subnetNameLabel = new QLabel("Subnet Name", subnetsFrame);
                subnetNameEdt = new QLineEdit(subnetsFrame);
                subnetNameEdt->setPlaceholderText("my-subnet-01");
                regionsCBox = new QComboBox(subnetsFrame);
                // todo - retrieve from api call
                regionsCBox->addItems({"us-east-1a", "us-east-1b"});
                subnetIPv4CIDRLabel = new QLabel("Subnet IPv4 CIDR", subnetsFrame);
                subnetIPv4CIDREdt = new QLineEdit(subnetsFrame);
                subnetIPv4CIDREdt->setPlaceholderText("10.0.1.0/24");
                addSubnetBtn = new QPushButton("Add", subnetsFrame);
            subnetsLayout->addWidget(subnetNameLabel);
            subnetsLayout->addWidget(subnetNameEdt);
            subnetsLayout->addWidget(regionsCBox);
            subnetsLayout->addWidget(subnetIPv4CIDRLabel);
            subnetsLayout->addWidget(subnetIPv4CIDREdt);
            subnetsLayout->addWidget(addSubnetBtn);
        createVPCLayout->addWidget(subnetsLabel);
        createVPCLayout->addWidget(subnetsFrame);

            igwLabel = new QLabel("IGW Name", createVPCFrame);
            igwEdt = new QLineEdit(createVPCFrame);
            igwEdt->setPlaceholderText("my-igw-01");
        createVPCLayout->addWidget(igwLabel);
        createVPCLayout->addWidget(igwEdt);

            RTsLabel = new QLabel("Route Tables", createVPCFrame);
            RTsFrame = new QFrame(createVPCFrame);
            RTsLayout = new QVBoxLayout(RTsFrame);
                RTNameLabel = new QLabel("Route Table Name", RTsFrame);
                RTNameEdt = new QLineEdit(RTsFrame);
                RTNameEdt->setPlaceholderText("my-rt-01");
                subnetsLWidget = new QListWidget(RTsFrame);
                // todo - retrieve from existing subnets
                subnetsLWidget->addItems({"subnet-1", "subnet-2"});
                subnetsLWidget->setSelectionMode(QAbstractItemView::MultiSelection);
                addRTBtn = new QPushButton("Add", RTsFrame);
            RTsLayout->addWidget(RTNameLabel);
            RTsLayout->addWidget(RTNameEdt);
            RTsLayout->addWidget(subnetsLWidget);
            RTsLayout->addWidget(addRTBtn);
        createVPCLayout->addWidget(RTsLabel);
        createVPCLayout->addWidget(RTsFrame);
        createVPCScrollArea->setWidget(createVPCFrame);
}























