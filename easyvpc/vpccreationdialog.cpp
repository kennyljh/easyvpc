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
#include <QFont>
#include <QList>
#include <awsmanager.h>

VPCCreationDialog::VPCCreationDialog(QWidget *parent)
                  : QDialog(parent){

    connect(&AWSManager::instance(), &AWSManager::zonesReady,
                this, &VPCCreationDialog::processZones);

    setWindowTitle("Create VPC");
    resize(650, 680);

    QFont qfontB11;
    qfontB11.setPointSize(11);
    qfontB11.setBold(true);

    mainLayout = new QVBoxLayout(this);
        createVPCScrollArea = new QScrollArea(this);
        createVPCScrollArea->setWidgetResizable(true);
        createVPCFrame = new QFrame(createVPCScrollArea);
        createVPCLayout = new QVBoxLayout(createVPCFrame);
        createVPCLayout->setSpacing(10);
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
        createVPCLayout->addSpacing(15);

            subnetsLabel = new QLabel("Subnets", createVPCFrame);
            subnetsFrame = new QFrame(createVPCFrame);
            subnetsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            subnetsFrame->setLineWidth(1);
            subnetsLayout = new QVBoxLayout(subnetsFrame);
                subnetNameLabel = new QLabel("Subnet Name", subnetsFrame);
                subnetNameEdt = new QLineEdit(subnetsFrame);
                subnetNameEdt->setPlaceholderText("my-subnet-01");
                AZsLabel = new QLabel("Availability Zones", subnetsFrame);
                regionsCBox = new QComboBox(subnetsFrame);
                subnetIPv4CIDRLabel = new QLabel("Subnet IPv4 CIDR", subnetsFrame);
                subnetIPv4CIDREdt = new QLineEdit(subnetsFrame);
                subnetIPv4CIDREdt->setPlaceholderText("10.0.1.0/24");

                subnetBundle *subnet = new subnetBundle();
                subnet->name = subnetNameEdt;
                subnet->zone = regionsCBox;
                subnet->ipv4 = subnetIPv4CIDREdt;
                subnetBundles.push_back(*subnet);

                addSubnetBtn = new QPushButton("Add", subnetsFrame);
            subnetsLayout->addWidget(subnetNameLabel);
            subnetsLayout->addWidget(subnetNameEdt);
            subnetsLayout->addWidget(AZsLabel);
            subnetsLayout->addWidget(regionsCBox);
            subnetsLayout->addWidget(subnetIPv4CIDRLabel);
            subnetsLayout->addWidget(subnetIPv4CIDREdt);
            subnetsLayout->addWidget(addSubnetBtn);
        createVPCLayout->addWidget(subnetsLabel);
        createVPCLayout->addWidget(subnetsFrame);
        createVPCLayout->addSpacing(15);

            igwLabel = new QLabel("IGW Name", createVPCFrame);
            igwEdt = new QLineEdit(createVPCFrame);
            igwEdt->setPlaceholderText("my-igw-01");
        createVPCLayout->addWidget(igwLabel);
        createVPCLayout->addWidget(igwEdt);
        createVPCLayout->addSpacing(15);

            RTsLabel = new QLabel("Route Tables", createVPCFrame);
            RTsFrame = new QFrame(createVPCFrame);
            RTsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            RTsFrame->setLineWidth(1);
            RTsLayout = new QVBoxLayout(RTsFrame);
                RTNameLabel = new QLabel("Route Table Name", RTsFrame);
                RTNameEdt = new QLineEdit(RTsFrame);
                RTNameEdt->setPlaceholderText("my-rt-01");
                refreshSubnetsBtn = new QPushButton("Refresh Subnets");
                connect(refreshSubnetsBtn, &QPushButton::clicked,
                            this, &VPCCreationDialog::subnetsRefreshButtonClicked);
                subnetsLWidget = new QListWidget(RTsFrame);
                subnetsLWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
                subnetsLWidget->setSelectionMode(QAbstractItemView::MultiSelection);

                RTBundle *RT = new RTBundle();
                RT->name = RTNameEdt;
                RT->subnets = subnetsLWidget;
                RTBundles.push_back(*RT);

                addRTBtn = new QPushButton("Add", RTsFrame);
            RTsLayout->addWidget(RTNameLabel);
            RTsLayout->addWidget(RTNameEdt);
            RTsLayout->addWidget(refreshSubnetsBtn);
            RTsLayout->addWidget(subnetsLWidget);
            RTsLayout->addWidget(addRTBtn);
        createVPCLayout->addWidget(RTsLabel);
        createVPCLayout->addWidget(RTsFrame);
        createVPCLayout->addSpacing(25);

            createBtn = new QPushButton("Create VPC", createVPCFrame);
            connect(createBtn, &QPushButton::clicked,
                        this, &VPCCreationDialog::createVPCRequest);
        createVPCLayout->addWidget(createBtn);
        createVPCScrollArea->setWidget(createVPCFrame);
    mainLayout->addWidget(createVPCScrollArea);

    // api calls
    AWSManager::instance().getZonesAsync();
}

void VPCCreationDialog::subnetsRefreshButtonClicked(){

    subnetsLWidget->clear();
    for (const auto &subnetBundle : subnetBundles){
        subnetsLWidget->addItem(subnetBundle.name->text());
    }
}

void VPCCreationDialog::createVPCRequest(){

    QString vpcName = vpcNameEdt->text();
    QString vpcCIDR = vpcIPv4CIDREdt->text();

    for (const auto &subnetBundle : subnetBundles){

        subnetInfo *info = new subnetInfo;
        info->name = subnetBundle.name->text();
        info->zone = subnetBundle.zone->currentText();
        info->ipv4 = subnetBundle.ipv4->text();
        subnetInfos.append(*info);
        qDebug() << "Found subnet bundle: " + info->name + " " + info->zone + " " + info->ipv4;
    }

    QString igwName = igwEdt->text();

    for (const auto &RTBundle : RTBundles){

        RTInfo *info = new RTInfo;
        info->name = RTBundle.name->text();
        qDebug() << "Found RT: " + info->name;
        QList<QListWidgetItem*> selected = RTBundle.subnets->selectedItems();
        QStringList list;
        for (const auto &item : selected){
            list.append(item->text());
            qDebug() << "Found subnet: " + item->text() + " for " + info->name;
        }
        info->subnets = list;
        RTInfos.append(*info);
    }

    emit VPCCreationRequested(
        vpcName,
        vpcCIDR,
        subnetInfos,
        igwName,
        RTInfos
    );
    accept();
}

void VPCCreationDialog::processZones(const std::vector<Aws::EC2::Model::AvailabilityZone> &zones){

    for (const auto &zone : zones){
        regionsCBox->addItem(QString::fromStdString(zone.GetZoneName()));
    }
}
