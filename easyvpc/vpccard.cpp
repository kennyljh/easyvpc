#include "vpccard.h"
#include <QFrame>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <QLayoutItem>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>
#include "awsmanager.h"
#include "subnetcard.h"
#include "guiutil.h"
#include "rtcard.h"
#include "infrastructurecoordinator.h"

VPCCard::VPCCard(const QString &name, const QString &id,
                            const QString &ipv4cidr, const QString &state,
                            QWidget *parent)
        : QFrame (parent) {

    connect(&AWSManager::instance(), &AWSManager::subnetsReady,
                this, &VPCCard::processSubnets);
    connect(&AWSManager::instance(), &AWSManager::RTsByVPCIdReady,
                this, &VPCCard::processRTs);
    connect(&AWSManager::instance(), &AWSManager::IGWByVPCIdReady,
                this, &VPCCard::processIGW);

    vpcID = id;

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(3);
    setMidLineWidth(3);

    vpcFrameLayout = new QVBoxLayout(this);
    vpcFrameLayout->setAlignment(Qt::AlignTop);
        vpcTitleFrame = new QFrame(this);
        // vpcTitleFrame->setObjectName("123456");
        // vpcTitleFrame->setStyleSheet("#123456 {border: 1px solid red;}");
        vpcTitleLayout = new QHBoxLayout(vpcTitleFrame);
            vpcName = new QLabel(name, vpcTitleFrame);
            QFont qfont;
            qfont.setBold(true);
            qfont.setPointSize(15);
            vpcName->setFont(qfont);
            expandBtn = new QPushButton("Expand", vpcTitleFrame);
            connect(expandBtn, &QPushButton::clicked, this, &VPCCard::vpcExpandTriggered);
            minimizeBtn = new QPushButton("Minimize", vpcTitleFrame);
            connect(minimizeBtn, &QPushButton::clicked, this, &VPCCard::vpcMinimizeTriggered);
            minimizeBtn->hide();
            deleteBtn = new QPushButton("Delete", vpcTitleFrame);
            deleteBtn->hide();
            connect(deleteBtn, &QPushButton::clicked,
                        this, &VPCCard::deleteVPCButtonClicked);
        vpcTitleLayout->addWidget(vpcName);
        vpcTitleLayout->addStretch();
        vpcTitleLayout->addWidget(expandBtn);
        vpcTitleLayout->addWidget(minimizeBtn);
        vpcTitleLayout->addWidget(deleteBtn);
        titleHLine = new QFrame(this);
        vpcDetailsFrame = new QFrame(this);
        vpcDetailsLayout = new QHBoxLayout(vpcDetailsFrame);
            titleHLine ->setFrameStyle(QFrame::HLine | QFrame::Raised);
            vpcIDLabel = new QLabel("VPC ID: " + id, this);
            vpcIPv4CIDRLabel = new QLabel("IPv4 CIDR: " + ipv4cidr, this);
            vpcStateLabel = new QLabel("VPC State: " + state, this);
        vpcDetailsLayout->addWidget(vpcIDLabel, 1);
        vpcDetailsLayout->addWidget(vpcIPv4CIDRLabel, 1);
        vpcDetailsLayout->addWidget(vpcStateLabel);
    vpcFrameLayout->addWidget(vpcTitleFrame);
    vpcFrameLayout->addWidget(titleHLine );
    vpcFrameLayout->addWidget(vpcDetailsFrame);
    vpcFrameLayout->setSpacing(0);
}

void VPCCard::expandCard(){

    GUIUtil util;
    QFont qfontB15;
    qfontB15.setPointSize(15);
    qfontB15.setBold(true);

    subnetMainFrame = new QFrame(this);
    subnetMainLayout = new QVBoxLayout(subnetMainFrame);
    subnetMainLayout->setAlignment(Qt::AlignTop);
        subnetTopFrame = new QFrame(subnetMainFrame);
        subnetTopFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        subnetTopLayout = new QHBoxLayout(subnetTopFrame);
            subnetLabel = new QLabel("Subnets", subnetTopFrame);
            subnetLabel->setFont(qfontB15);
            addSubnetBtn = new QPushButton("Add", subnetTopFrame);
            addSubnetBtn->hide();
        subnetTopLayout->addWidget(subnetLabel);
        subnetTopLayout->addStretch();
        subnetTopLayout->addWidget(addSubnetBtn);

        subnetScrollArea = new QScrollArea(subnetMainFrame);
            subnetsWindow = new QWidget(subnetScrollArea);
            subnetsLayout = new QVBoxLayout(subnetsWindow);
            subnetsLayout->setAlignment(Qt::AlignTop);
        subnetScrollArea->setWidget(subnetsWindow);
        subnetScrollArea->setWidgetResizable(true);
        subnetScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        subnetScrollArea->setFixedHeight(300);
        subnetScrollArea->setAlignment(Qt::AlignTop);
    subnetMainLayout->addWidget(subnetTopFrame);
    subnetMainLayout->addWidget(subnetScrollArea);
    subnetMainLayout->setSpacing(0);

    util.applyWidgetFade(subnetMainFrame, 300);

    RTMainFrame = new QFrame(this);
    RTMainLayout = new QVBoxLayout(RTMainFrame);
    RTMainLayout->setAlignment(Qt::AlignTop);
        RTTopFrame = new QFrame(RTMainFrame);
        RTTopFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        RTTopLayout = new QHBoxLayout(RTTopFrame);
            RTLabel = new QLabel("Route Tables", RTTopFrame);
            RTLabel->setFont(qfontB15);
            addRTBtn = new QPushButton("Add", RTTopFrame);
            addRTBtn->hide();
        RTTopLayout->addWidget(RTLabel);
        RTTopLayout->addStretch();
        RTTopLayout->addWidget(addRTBtn);

        RTScrollArea = new QScrollArea(RTMainFrame);
            RTsWindow = new QWidget(RTScrollArea);
            RTsLayout = new QVBoxLayout(RTsWindow);
            RTsLayout->setAlignment(Qt::AlignTop);
        RTScrollArea->setWidget(RTsWindow);
        RTScrollArea->setWidgetResizable(true);
        RTScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        RTScrollArea->setFixedHeight(300);
        RTScrollArea->setAlignment(Qt::AlignTop);
    RTMainLayout->addWidget(RTTopFrame);
    RTMainLayout->addWidget(RTScrollArea);

    util.applyWidgetFade(RTMainFrame, 300);

    RTMainLayout->setSpacing(0);

    util.applyWidgetFade(RTMainFrame, 300);

    IGWMainFrame = new QFrame(this);
    IGWMainLayout = new QVBoxLayout(IGWMainFrame);
    IGWMainLayout->setAlignment(Qt::AlignTop);
        IGWTopFrame = new QFrame(IGWMainFrame);
        IGWTopFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        IGWTopLayout = new QHBoxLayout(IGWTopFrame);
            IGWLabel = new QLabel("Internet Gateway", IGWTopFrame);
            IGWLabel->setFont(qfontB15);
            IGWAddBtn = new QPushButton("Add", IGWTopFrame);
        IGWTopLayout->addWidget(IGWLabel);
        IGWTopLayout->addStretch();
        IGWTopLayout->addWidget(IGWAddBtn);

        IGWDetailsFrame = new QFrame(IGWMainFrame);
        IGWDetailsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
        IGWDetailsFrame->setLineWidth(3);
        IGWDetailsLayout = new QVBoxLayout(IGWDetailsFrame);
    IGWMainLayout->addWidget(IGWTopFrame);
    IGWMainLayout->addWidget(IGWDetailsFrame);

    util.applyWidgetFade(IGWMainFrame, 300);

    vpcFrameLayout->addWidget(subnetMainFrame);
    vpcFrameLayout->addWidget(RTMainFrame);
    vpcFrameLayout->addWidget(IGWMainFrame);
}

void VPCCard::processSubnets(const QString &vpcId, const std::vector<Aws::EC2::Model::Subnet> &subnets){

    GUIUtil util;
    QFont qfont11;
    qfont11.setPointSize(11);

    // since each instance of VPCCard is connected to the signal, all of them
    // will be prompted to update subnets frame. This check is necessary
    // to ensure that the correct subnet frame is updated, otherwise
    // may encounter nullptr error
    if (vpcID != vpcId) return;

    if (subnets.empty()){

        QLabel *label = new QLabel("No subnets found", subnetMainFrame);
        label->setFont(qfont11);
        subnetsLayout->addWidget(label);
        subnetsLayout->setAlignment(Qt::AlignCenter);
        return;
    }

    QLayoutItem *item;
    while ((item = subnetsLayout->takeAt(0)) != nullptr){
        if (QWidget *widget = item->widget()) widget->deleteLater();
        delete item;
    }

    for (const Aws::EC2::Model::Subnet &subnet : subnets){

        QString name = "default-subnet-name";
        for (const auto &tag : subnet.GetTags()){
            if (tag.GetKey() == "Name") name = QString::fromStdString(tag.GetValue());
        }
        QString id = QString::fromStdString(subnet.GetSubnetId());
        QString ipv4cidr = QString::fromStdString(subnet.GetCidrBlock());
        QString ipAddrCount = QString::number(subnet.GetAvailableIpAddressCount());
        QString zoneID = QString::fromStdString(subnet.GetAvailabilityZoneId());
        QString zone = QString::fromStdString(subnet.GetAvailabilityZone());
        QString state = QString::fromStdString(
                            Aws::EC2::Model::SubnetStateMapper
                            ::GetNameForSubnetState(subnet.GetState())
        );
        qDebug() << "Subnet found: " + name + ", " + id + ", " + ipv4cidr +
                    ", " + ipAddrCount + ", " + zoneID + ", " + zone +
                    ", " + state;

        SubnetCard *card = new SubnetCard(vpcID, name, id, ipv4cidr,
                                            ipAddrCount, zoneID, zone,
                                            state, subnetsWindow);
        subnetsLayout->addWidget(card);
        util.applyWidgetFade(card, 300);
        // todo - caching subnets
    }
}

void VPCCard::processRTs(const QString &vpcId, const std::vector<Aws::EC2::Model::RouteTable> &RTs){

    GUIUtil util;
    QFont qfont11;
    qfont11.setPointSize(11);

    if (vpcID != vpcId) return;

    if (RTs.empty()){

        QLabel *label = new QLabel("No route tables found", RTMainFrame);
        label->setFont(qfont11);
        RTsLayout->addWidget(label);
        RTsLayout->setAlignment(Qt::AlignCenter);
        return;
    }

    QLayoutItem *item;
    while ((item = RTsLayout->takeAt(0)) != nullptr){
        if (QWidget *widget = item->widget()) widget->deleteLater();
        delete item;
    }

    for (const auto &rt : RTs){

        QString name = "default-RT-name";
        for (const auto &tag : rt.GetTags()){
            if (tag.GetKey() == "Name") name = QString::fromStdString(tag.GetValue());
        }
        QString id = QString::fromStdString(rt.GetRouteTableId());
        QString ownerId = QString::fromStdString(rt.GetOwnerId());
        qDebug() << "Found route table: " + name + " " + id + " " + ownerId;

        QStringList subnetsIds;
        QString gatewayId;

        for (const auto &assoc : rt.GetAssociations()){
            if (!assoc.GetMain()){
                subnetsIds.append(QString::fromStdString(assoc.GetSubnetId()));
                qDebug() << "Found subnet: " + QString::fromStdString(assoc.GetSubnetId());
            }
        }
        for (const auto &route : rt.GetRoutes()){
            if (route.GetGatewayId().length() > 0){
                gatewayId = QString::fromStdString(route.GetGatewayId());
                qDebug() << "Found gateway: " + QString::fromStdString(route.GetGatewayId());
            }
            else gatewayId = "N/A";
        }

        RTCard *card = new RTCard(vpcId, name, id, ownerId, subnetsIds,
                                    gatewayId, RTsWindow);
        RTsLayout->addWidget(card);
        util.applyWidgetFade(card, 300);
    }
}

void VPCCard::processIGW(const QString &vpcId, const std::vector<Aws::EC2::Model::InternetGateway> &IGW){

    GUIUtil util;
    QFont qfont11;
    qfont11.setPointSize(11);
    QFont qfontB13;
    qfontB13.setBold(true);
    qfontB13.setPointSize(13);

    if (vpcID != vpcId) return;

    if (IGW.empty()){

        QLabel *label = new QLabel("No IGW found", IGWDetailsFrame);
        label->setFont(qfont11);
        IGWDetailsLayout->addWidget(label);
        IGWDetailsLayout->setAlignment(Qt::AlignCenter);
        IGWAddBtn->show();
        return;
    }
    else {
        IGWAddBtn->hide();
    }

    QLayoutItem *item;
    while ((item = IGWDetailsLayout->takeAt(0)) != nullptr){
        if (QWidget *widget = item->widget()) widget->deleteLater();
        delete item;
    }

    const auto &igw = IGW.front();

    QString name = "default-IGW-name";
    for (const auto &tag : igw.GetTags()){
        if (tag.GetKey() == "Name") name = QString::fromStdString(tag.GetValue());
    }
    QString IGWId = QString::fromStdString(igw.GetInternetGatewayId());
    QString ownerId = QString::fromStdString(igw.GetOwnerId());
    qDebug() << "Found IGW: " + name + " " + IGWId + " " + ownerId;

    nameAndBtnFrame = new QFrame(IGWDetailsFrame);
    nameAndBtnLayout = new QHBoxLayout(nameAndBtnFrame);
        IGWNameLabel = new QLabel(name);
        IGWNameLabel->setFont(qfontB13);
        deleteIGWBtn = new QPushButton("Delete", nameAndBtnFrame);
    nameAndBtnLayout->addWidget(IGWNameLabel);
    nameAndBtnLayout->addStretch();
    nameAndBtnLayout->addWidget(deleteIGWBtn);

    idAndOwnerIdFrame = new QFrame(IGWDetailsFrame);
    idAndOwnerIdFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
    idAndOwnerIdFrame->setLineWidth(2);
    idAndOwnerIdLayout = new QHBoxLayout(idAndOwnerIdFrame);
        IGWIdLabel = new QLabel("IGW ID: " + IGWId, idAndOwnerIdFrame);
        IGWOwnerIdLabel = new QLabel("Owner ID: " + ownerId, idAndOwnerIdFrame);
    idAndOwnerIdLayout->addWidget(IGWIdLabel);
    idAndOwnerIdLayout->addWidget(IGWOwnerIdLabel);

    IGWDetailsLayout->addWidget(nameAndBtnFrame);
    IGWDetailsLayout->addWidget(idAndOwnerIdFrame);
}

void VPCCard::vpcExpandTriggered(){

    expandBtn->hide();
    minimizeBtn->show();
    deleteBtn->show();

    // fill up with placeholder frame first
    expandCard();

    qDebug() << "Finding VPC details with id: " + vpcID;
    AWSManager::instance().getSubnetsAsync(vpcID);
    AWSManager::instance().getRTsByVPCIdAsync(vpcID);
    AWSManager::instance().getIGWByVPCIdAsync(vpcID);
}

void VPCCard::vpcMinimizeTriggered(){

    minimizeBtn->hide();
    expandBtn->show();
    deleteBtn->hide();

    for (int i = vpcFrameLayout->count() - 1; i >= 0; i--){

        QLayoutItem *item = vpcFrameLayout->itemAt(i);
        QWidget *widget = item->widget();

        if (widget == vpcTitleFrame ||
            widget == titleHLine ||
            widget == vpcDetailsFrame) continue;

        item = vpcFrameLayout->takeAt(i);

        if (widget) widget->deleteLater();
        delete item;
    }
}

void VPCCard::deleteVPCButtonClicked(){

    InfrastructureCoordinator *coordinator = new InfrastructureCoordinator(this);
    coordinator->coordinateVPCInfrastructureDeletion(vpcID);
}




