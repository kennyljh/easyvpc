#include "subnetcard.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QDebug>
#include "awsmanager.h"
#include "guiutil.h"
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

SubnetCard::SubnetCard(const QString &vpcid, const QString &name, const QString &subnetid,
                                const QString &ipv4cidr, const QString &ipAddrCount,
                                const QString &zoneID, const QString &zone,
                                const QString &state, QWidget *parent)
            : QFrame (parent) {

    connect(&AWSManager::instance(), &AWSManager::reservationsByIdReady,
                this, &SubnetCard::processEC2s);
    connect(&AWSManager::instance(), &AWSManager::RTByIdReady,
                this, &SubnetCard::processRT);
    connect(&AWSManager::instance(), &AWSManager::ACLsByIdReady,
                this, &SubnetCard::processACLs);

    vpcID = vpcid;
    subnetID = subnetid;

    QFont qfontB13;
    qfontB13.setBold(true);
    qfontB13.setPointSize(13);

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(3);
    setMidLineWidth(3);

    mainLayout = new QVBoxLayout(this);

        subnetTopFrame = new QFrame(this);
        subnetTopLayout = new QHBoxLayout(subnetTopFrame);
            subnetNameLabel = new QLabel(name, subnetTopFrame);
            subnetNameLabel->setFont(qfontB13);
            deleteSubnetBtn = new QPushButton("Delete", subnetTopFrame);
        subnetTopLayout->addWidget(subnetNameLabel);
        subnetTopLayout->addStretch();
        subnetTopLayout->addWidget(deleteSubnetBtn);

        subnetMiscFrame = new QFrame(this);
        subnetMiscLayout = new QHBoxLayout(subnetMiscFrame);
            subnetDetailsFrame = new QFrame(subnetMiscFrame);
            subnetDetailsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            subnetDetailsFrame->setLineWidth(2);
            subnetDetailsLayout = new QVBoxLayout(subnetDetailsFrame);
                subnetDetailsLabel = new QLabel("Details");
                QFrame *hline = new QFrame(subnetDetailsFrame);
                hline->setFrameStyle(QFrame::HLine | QFrame::Raised);
                subnetDetailsLabel->setFont(qfontB13);
                subnetIDLabel = new QLabel("ID: " + subnetid);
                subnetIPv4CIDRLabel = new QLabel("IPv4 CIDR: " + ipv4cidr);
                subnetIPAddrCountLabel = new QLabel("Available IPv4 Addresses: " + ipAddrCount);
                subnetZoneLabel = new QLabel("Zone: " + zone + " (" + zoneID + ")" );
                subnetStateLabel = new QLabel("State: " + state);
            subnetDetailsLayout->addWidget(subnetDetailsLabel);
            subnetDetailsLayout->addWidget(hline);
            subnetDetailsLayout->addWidget(subnetIDLabel);
            subnetDetailsLayout->addWidget(subnetIPv4CIDRLabel);
            subnetDetailsLayout->addWidget(subnetIPAddrCountLabel);
            subnetDetailsLayout->addWidget(subnetZoneLabel);
            subnetDetailsLayout->addWidget(subnetStateLabel);

            subnetEC2sFrame = new QFrame(subnetMiscFrame);
            subnetEC2sFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            subnetEC2sFrame->setLineWidth(2);
            subnetEC2sLayout = new QVBoxLayout(subnetEC2sFrame);
                ec2sTopFrame = new QFrame(subnetEC2sFrame);
                ec2sTopLayout = new QHBoxLayout(ec2sTopFrame);
                    ec2sLabel = new QLabel("EC2s", ec2sTopFrame);
                    ec2sLabel->setFont(qfontB13);
                    ec2sManageBtn = new QPushButton("Manage", ec2sTopFrame);
                ec2sTopLayout->addWidget(ec2sLabel);
                ec2sTopLayout->addStretch();
                ec2sTopLayout->addWidget(ec2sManageBtn);

                ec2sScrollArea = new QScrollArea(subnetEC2sFrame);
                    ec2sFrame = new QFrame(subnetEC2sFrame);
                    ec2sLayout = new QVBoxLayout(ec2sFrame);
                ec2sScrollArea->setWidget(ec2sFrame);
                ec2sScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                ec2sScrollArea->setWidgetResizable(true);
            subnetEC2sLayout->addWidget(ec2sTopFrame);
            subnetEC2sLayout->addWidget(ec2sScrollArea);
            subnetEC2sLayout->setSpacing(0);

            subnetRTAndACLsFrame = new QFrame(subnetMiscFrame);
            subnetRTAndACLsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            subnetRTAndACLsFrame->setLineWidth(2);
            subnetRTAndACLsLayout = new QVBoxLayout(subnetRTAndACLsFrame);
                subnetRTFrame = new QFrame(subnetRTAndACLsFrame);
                subnetRTFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
                subnetRTFrame->setLineWidth(1);
                subnetRTLayout = new QVBoxLayout(subnetRTFrame);
                subnetRTLayout->setAlignment(Qt::AlignTop);
                    subnetRTLabel = new QLabel("Route Table");
                    subnetRTLabel->setFont(qfontB13);
                subnetRTLayout->addWidget(subnetRTLabel);
                subnetACLsFrame = new QFrame(subnetRTAndACLsFrame);
                subnetACLsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
                subnetACLsFrame->setLineWidth(1);
                subnetACLsLayout = new QVBoxLayout(subnetACLsFrame);
                subnetACLsLayout->setAlignment(Qt::AlignTop);
                    subnetACLsLabel = new QLabel("Network ACLs");
                    subnetACLsLabel->setFont(qfontB13);
                subnetACLsLayout->addWidget(subnetACLsLabel);
            subnetRTAndACLsLayout->addWidget(subnetRTFrame, 1);
            subnetRTAndACLsLayout->addWidget(subnetACLsFrame, 1);

        subnetMiscLayout->addWidget(subnetDetailsFrame, 1);
        subnetMiscLayout->addWidget(subnetEC2sFrame, 1);
        subnetMiscLayout->addWidget(subnetRTAndACLsFrame, 1);
    mainLayout->addWidget(subnetTopFrame);
    mainLayout->addWidget(subnetMiscFrame);
    mainLayout->setSpacing(0);

    // api calls
    AWSManager::instance().getReservationsAsync(subnetID);
    AWSManager::instance().getRTAsync(subnetID);
    AWSManager::instance().getACLsAsync(subnetID);
}

void SubnetCard::processEC2s(const QString &subnetId, const std::vector<Aws::EC2::Model::Reservation> &reservations){

    GUIUtil util;
    QFont qfontB11;
    qfontB11.setBold(true);
    qfontB11.setPointSize(11);

    if (subnetID != subnetId) return;

    if (reservations.empty()) return;

    QLayoutItem *item;
    while ((item = ec2sLayout->takeAt(0)) != nullptr){
        if (QWidget *widget = item->widget()) widget->deleteLater();
        delete item;
    }

    for (const auto &reservation : reservations){
        for (const auto &instance : reservation.GetInstances()){

            QFrame *ec2Frame = new QFrame(ec2sFrame);
            ec2Frame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            ec2Frame->setLineWidth(1);
            QVBoxLayout *ec2Layout = new QVBoxLayout(ec2Frame);

            QString name = "default-EC2-name";
            for (const auto &tag : instance.GetTags()){
                if (tag.GetKey() == "Name") name = QString::fromStdString(tag.GetValue());
            }
            QString ec2ID = QString::fromStdString(instance.GetInstanceId());
            QString state = QString::fromStdString(
                                Aws::EC2::Model::InstanceStateNameMapper
                                ::GetNameForInstanceStateName(instance.GetState().GetName())
            );
            QString privateIP = QString::fromStdString(instance.GetPrivateIpAddress());
            QString publicIP = QString::fromStdString(instance.GetPublicIpAddress());

            qDebug() << "Found EC2: " + name + " " + ec2ID + " " + state + " " +
                        privateIP + " " + publicIP;

            QLabel *nameAndID = new QLabel(name + " / " + ec2ID, ec2Frame);
            nameAndID->setFont(qfontB11);
            ec2Layout->addWidget(nameAndID);
            ec2Layout->addWidget(new QLabel("State: " + state, ec2Frame));
            ec2Layout->addWidget(new QLabel("Private IP: " + privateIP, ec2Frame));
            ec2Layout->addWidget(new QLabel("Public IP: " + publicIP, ec2Frame));

            ec2sLayout->addWidget(ec2Frame);
            util.applyWidgetFade(ec2Frame, 300);
        }
    }
}

void SubnetCard::processRT(const QString &subnetId, const std::vector<Aws::EC2::Model::RouteTable> &RT){

    GUIUtil util;
    QFont qfontB10;
    qfontB10.setBold(true);
    qfontB10.setPointSize(10);

    if (subnetID != subnetId) return;

    if (RT.empty()) return;

    for (const auto &rt : RT){

        QFrame *RTFrame = new QFrame(subnetRTFrame);
        RTFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
        RTFrame->setLineWidth(1);
        QVBoxLayout *RTLayout = new QVBoxLayout(RTFrame);

        QString name = "default-RT-name";
        for (const auto &tag : rt.GetTags()){
            if (tag.GetKey() == "Name") name = QString::fromStdString(tag.GetValue());
        }
        QString RTId = QString::fromStdString(rt.GetRouteTableId());

        qDebug() << "Found RT: " + name + " " + RTId;

        QLabel *RTLabel = new QLabel(name + " / " + RTId);
        RTLabel->setFont(qfontB10);
        RTLayout->addWidget(RTLabel);
        subnetRTLayout->addWidget(RTFrame);
        util.applyWidgetFade(RTFrame, 300);
    }
}

void SubnetCard::processACLs(const QString &subnetId, const std::vector<Aws::EC2::Model::NetworkAcl> &ACLs){

    GUIUtil util;
    QFont qfontB10;
    qfontB10.setBold(true);
    qfontB10.setPointSize(10);

    if (subnetID != subnetId) return;

    if (ACLs.empty()) return;

    for (const auto &acl : ACLs){

        QFrame *ACLFrame = new QFrame(subnetACLsFrame);
        ACLFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
        ACLFrame->setLineWidth(1);
        QVBoxLayout *ACLLayout = new QVBoxLayout(ACLFrame);

        QString name = "default-ACL-name";
        for (const auto &tag : acl.GetTags()){
            if (tag.GetKey() == "Name") name = QString::fromStdString(tag.GetValue());
        }
        QString ACLId = QString::fromStdString(acl.GetNetworkAclId());

        qDebug() << "Found Network ACL: " + name + " " + ACLId;

        QLabel *ACLLabel = new QLabel(name + " / " + ACLId);
        ACLLabel->setFont(qfontB10);
        ACLLayout->addWidget(ACLLabel);
        subnetACLsLayout->addWidget(ACLFrame);
    }
}