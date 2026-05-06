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
#include "awsmanager.h"
#include "subnetcard.h"
#include "guiutil.h"

VPCCard::VPCCard(const QString &name, const QString &id,
                            const QString &ipv4cidr, const QString &state,
                            QWidget *parent)
        : QFrame (parent) {

    connect(&AWSManager::instance(), &AWSManager::subnetsReady,
                this, &VPCCard::processSubnets);

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
            deleteBtn->setStyleSheet("color: #ff1414");
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
}

void VPCCard::expandCard(){

    GUIUtil util;

    subnetMainFrame = new QFrame(this);
    subnetMainLayout = new QVBoxLayout(subnetMainFrame);
        subnetTopFrame = new QFrame(subnetMainFrame);
        subnetTopLayout = new QHBoxLayout(subnetTopFrame);
            subnetLabel = new QLabel("Subnets", subnetTopFrame);
            QFont qfont;
            qfont.setPointSize(15);
            subnetLabel->setFont(qfont);
            addSubnetBtn = new QPushButton("Add", subnetTopFrame);
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
    subnetMainLayout->addWidget(subnetTopFrame);
    subnetMainLayout->addWidget(subnetScrollArea);

    util.applyWidgetFade(subnetMainFrame, 300);

    // todo - expand other frames as well

    vpcFrameLayout->addWidget(subnetMainFrame);
}

void VPCCard::processSubnets(const std::vector<Aws::EC2::Model::Subnet> &subnets){

    if (subnets.empty()) return;

    // since each instance of VPCCard is connected to the signal, all of them
    // will be prompted to update subnets frame. This check is necessary
    // to ensure that the correct subnet frame is updated, otherwise
    // may encounter nullptr error
    if (vpcID != subnets.front().GetVpcId()) return;

    QLayoutItem *item;
    while ((item = subnetsLayout->takeAt(0)) != nullptr){
        if (QWidget *widget = item->widget()) widget->deleteLater();
        delete item;
    }

    for (const Aws::EC2::Model::Subnet &subnet : subnets){

        QString name = "No subnet name";
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

        SubnetCard *card = new SubnetCard(name, id, ipv4cidr,
                                        ipAddrCount, zoneID, zone,
                                        state, subnetMainFrame);
        subnetsLayout->addWidget(card);
        GUIUtil util;
        util.applyWidgetFade(card, 300);
        // todo - caching subnets
    }
}

void VPCCard::vpcExpandTriggered(){

    expandBtn->hide();
    minimizeBtn->show();

    // fill up with placeholder frame first
    expandCard();

    qDebug() << "Finding VPC details with id: " + vpcID;
    AWSManager::instance().getSubnetsAsync(vpcID);

    //todo - add other async calls
}

void VPCCard::vpcMinimizeTriggered(){

    minimizeBtn->hide();
    expandBtn->show();

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
