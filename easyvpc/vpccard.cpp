#include "vpccard.h"

#include <QFrame>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

VPCCard::VPCCard(const QString &name, const QString &id,
                            const QString &ipv4cidr, QWidget *parent)
        : QFrame (parent) {

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(3);
    setMidLineWidth(3);

    vpcFrameLayout = new QVBoxLayout(this);
        vpcTitleFrame = new QFrame(this);
        vpcTitleFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);
        vpcTitleLayout = new QHBoxLayout(vpcTitleFrame);
            vpcName = new QLabel("VPC Name: " + name, vpcTitleFrame);
            expandBtn = new QPushButton("Expand", vpcTitleFrame);
            deleteBtn = new QPushButton("Delete", vpcTitleFrame);
        vpcTitleLayout->addWidget(vpcName);
        vpcTitleLayout->addStretch();
        vpcTitleLayout->addWidget(expandBtn);
        vpcTitleLayout->addWidget(deleteBtn);

        vpcID = new QLabel("VPC ID: " + id, this);
        vpcIPv4CIDR = new QLabel("IPv4 CIDR: " + ipv4cidr, this);
    vpcFrameLayout->addWidget(vpcTitleFrame);
    vpcFrameLayout->addWidget(vpcID);
    vpcFrameLayout->addWidget(vpcIPv4CIDR);

    setFixedHeight(150);
}
