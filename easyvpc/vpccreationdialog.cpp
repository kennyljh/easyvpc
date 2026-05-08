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
            vpcIPv4CIDREdt->setPlaceholderText("10.0.0.0/16");
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

                subnetBundle subnet;
                subnet.name = subnetNameEdt;
                subnet.zone = regionsCBox;
                subnet.ipv4 = subnetIPv4CIDREdt;
                subnetBundles.push_back(subnet);

                addSubnetBtn = new QPushButton("Add", subnetsFrame);
                connect(addSubnetBtn, &QPushButton::clicked,
                            this, &VPCCreationDialog::addSubnetFrame);
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

                RTBundle RT;
                RT.name = RTNameEdt;
                RT.subnets = subnetsLWidget;
                RTBundles.push_back(RT);

                addRTBtn = new QPushButton("Add", RTsFrame);
                connect(addRTBtn, &QPushButton::clicked,
                            this, &VPCCreationDialog::addRTFrame);
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

    // clear old data, otherwise trigger bugs
    subnetInfos.clear();
    RTInfos.clear();

    for (const auto &subnetBundle : subnetBundles){

        subnetInfo info;
        info.name = subnetBundle.name->text();
        info.zone = subnetBundle.zone->currentText();
        info.ipv4 = subnetBundle.ipv4->text();
        if (info.name.isEmpty() || info.zone.isEmpty() || info.ipv4.isEmpty()){
            qDebug() << "Found subnet with incomplete info";
            continue;
        }
        subnetInfos.append(info);
        qDebug() << "Found subnet bundle: " + info.name + " " + info.zone + " " + info.ipv4;
    }

    QString igwName = igwEdt->text();

    for (const auto &RTBundle : RTBundles){

        RTInfo info;
        info.name = RTBundle.name->text();
        qDebug() << "Found RT: " + info.name;
        QList<QListWidgetItem*> selected = RTBundle.subnets->selectedItems();
        QStringList list;
        for (const auto &item : selected){

            bool validSubnet = false;
            for (const auto &subnet : subnetInfos){
                if (subnet.name == item->text()) validSubnet = true;
            }
            if (!validSubnet){
                qDebug() << "Invalid subnet name chosen for RT";
                continue;
            }
            list.append(item->text());
            qDebug() << "Found subnet: " + item->text() + " for " + info.name;
        }
        if (info.name.isEmpty()){
            qDebug() << "Found route table with incomplete info";
            continue;
        }
        info.subnets = list;
        RTInfos.append(info);
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
        AZones.append(QString::fromStdString(zone.GetZoneName()));
    }
}

void VPCCreationDialog::addSubnetFrame(){

    // removing add button
    QLayoutItem *item = subnetsLayout->takeAt(subnetsLayout->count() - 1);
    if (item->widget()) delete item->widget();
    delete item;

    QLabel *subnetNameLabel = new QLabel("Subnet Name", subnetsFrame);
    QLineEdit *subnetNameEdt = new QLineEdit(subnetsFrame);
    subnetNameEdt->setPlaceholderText("my-subnet-01");
    QLabel *AZsLabel = new QLabel("Availability Zones", subnetsFrame);
    QComboBox *regionsCBox = new QComboBox(subnetsFrame);
    for (const auto &zone : AZones) regionsCBox->addItem(zone);
    QLabel *subnetIPv4CIDRLabel = new QLabel("Subnet IPv4 CIDR", subnetsFrame);
    QLineEdit *subnetIPv4CIDREdt = new QLineEdit(subnetsFrame);
    subnetIPv4CIDREdt->setPlaceholderText("10.0.1.0/24");

    subnetBundle *subnet = new subnetBundle();
    subnet->name = subnetNameEdt;
    subnet->zone = regionsCBox;
    subnet->ipv4 = subnetIPv4CIDREdt;
    subnetBundles.push_back(*subnet);

    QPushButton *addSubnetBtn = new QPushButton("Add", subnetsFrame);
    connect(addSubnetBtn, &QPushButton::clicked,
                this, &VPCCreationDialog::addSubnetFrame);

    subnetsLayout->addSpacing(15);
    subnetsLayout->addWidget(subnetNameLabel);
    subnetsLayout->addWidget(subnetNameEdt);
    subnetsLayout->addWidget(AZsLabel);
    subnetsLayout->addWidget(regionsCBox);
    subnetsLayout->addWidget(subnetIPv4CIDRLabel);
    subnetsLayout->addWidget(subnetIPv4CIDREdt);
    subnetsLayout->addWidget(addSubnetBtn);
}

void VPCCreationDialog::addRTFrame(){

    // removing add button
    QLayoutItem *item = RTsLayout->takeAt(RTsLayout->count() - 1);
    if (item->widget()) delete item->widget();
    delete item;

    QLabel *RTNameLabel = new QLabel("Route Table Name", RTsFrame);
    QLineEdit *RTNameEdt = new QLineEdit(RTsFrame);
    RTNameEdt->setPlaceholderText("my-rt-01");
    QPushButton *refreshSubnetsBtn = new QPushButton("Refresh Subnets");
    QListWidget *subnetsLWidget = new QListWidget(RTsFrame);
    connect(refreshSubnetsBtn, &QPushButton::clicked,
                this, [this, subnetsLWidget](){
                    subnetsLWidget->clear();
                    for (const auto &subnetBundle : subnetBundles){
                        subnetsLWidget->addItem(subnetBundle.name->text());
                    }
    });
    subnetsLWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    subnetsLWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    RTBundle *RT = new RTBundle();
    RT->name = RTNameEdt;
    RT->subnets = subnetsLWidget;
    RTBundles.push_back(*RT);

    QPushButton *addRTBtn = new QPushButton("Add", RTsFrame);
    connect(addRTBtn, &QPushButton::clicked,
                this, &VPCCreationDialog::addRTFrame);

    RTsLayout->addSpacing(15);
    RTsLayout->addWidget(RTNameLabel);
    RTsLayout->addWidget(RTNameEdt);
    RTsLayout->addWidget(refreshSubnetsBtn);
    RTsLayout->addWidget(subnetsLWidget);
    RTsLayout->addWidget(addRTBtn);
}