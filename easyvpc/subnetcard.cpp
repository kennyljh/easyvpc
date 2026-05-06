#include "subnetcard.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

SubnetCard::SubnetCard(const QString &name, const QString &id,
                                const QString &ipv4cidr, const QString &ipAddrCount,
                                const QString &zoneID, const QString &zone,
                                const QString &state, QWidget *parent)
            : QFrame (parent) {

    QFont qfont;
    qfont.setPointSize(13);

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(3);
    setMidLineWidth(3);

    mainLayout = new QVBoxLayout(this);

        subnetTopFrame = new QFrame(this);
        subnetTopLayout = new QHBoxLayout(subnetTopFrame);
            subnetNameLabel = new QLabel(name, subnetTopFrame);
            subnetNameLabel->setFont(qfont);
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
                subnetDetailsLabel->setFont(qfont);
                subnetIDLabel = new QLabel("ID: " + id);
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
                    ec2sLabel->setFont(qfont);
                    ec2sCreateBtn = new QPushButton("Create", ec2sTopFrame);
                ec2sTopLayout->addWidget(ec2sLabel);
                ec2sTopLayout->addStretch();
                ec2sTopLayout->addWidget(ec2sCreateBtn);

                ec2sScrollArea = new QScrollArea(subnetEC2sFrame);
                    ec2sFrame = new QFrame(subnetEC2sFrame);
                    ec2sLayout = new QVBoxLayout(ec2sFrame);
                ec2sScrollArea->setWidget(ec2sFrame);
                ec2sScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            subnetEC2sLayout->addWidget(ec2sTopFrame);
            subnetEC2sLayout->addWidget(ec2sScrollArea);

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
                    subnetRTLabel->setFont(qfont);
                subnetRTLayout->addWidget(subnetRTLabel);
                subnetACLsFrame = new QFrame(subnetRTAndACLsFrame);
                subnetACLsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
                subnetACLsFrame->setLineWidth(1);
                subnetACLsLayout = new QVBoxLayout(subnetACLsFrame);
                subnetACLsLayout->setAlignment(Qt::AlignTop);
                    subnetACLsLabel = new QLabel("ACLs");
                    subnetACLsLabel->setFont(qfont);
                subnetACLsLayout->addWidget(subnetACLsLabel);
            subnetRTAndACLsLayout->addWidget(subnetRTFrame, 1);
            subnetRTAndACLsLayout->addWidget(subnetACLsFrame, 1);

        subnetMiscLayout->addWidget(subnetDetailsFrame, 1);
        subnetMiscLayout->addWidget(subnetEC2sFrame, 1);
        subnetMiscLayout->addWidget(subnetRTAndACLsFrame, 1);
    mainLayout->addWidget(subnetTopFrame);
    mainLayout->addWidget(subnetMiscFrame);
}

void SubnetCard::processEC2s(const std::vector<Aws::EC2::Model::Reservation> &ec2s){

}
