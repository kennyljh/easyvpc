#include "vpccard.h"
#include <QFrame>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFont>

VPCCard::VPCCard(const QString &name, const QString &id,
                            const QString &ipv4cidr, const QString &state,
                            QWidget *parent)
        : QFrame (parent) {

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(3);
    setMidLineWidth(3);

    vpcFrameLayout = new QVBoxLayout(this);
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
            deleteBtn = new QPushButton("Delete", vpcTitleFrame);
            deleteBtn->setStyleSheet("color: #ff2929");
        vpcTitleLayout->addWidget(vpcName);
        vpcTitleLayout->addStretch();
        vpcTitleLayout->addWidget(expandBtn);
        vpcTitleLayout->addWidget(deleteBtn);
        QFrame *hline = new QFrame(this);
        vpcDetailsFrame = new QFrame(this);
        vpcDetailsLayout = new QHBoxLayout(vpcDetailsFrame);
            hline->setFrameStyle(QFrame::HLine | QFrame::Raised);
            vpcIDLabel = new QLabel("VPC ID: " + id, this);
            vpcIPv4CIDRLabel = new QLabel("IPv4 CIDR: " + ipv4cidr, this);
            vpcStateLabel = new QLabel("VPC State: " + state, this);
        vpcDetailsLayout->addWidget(vpcIDLabel, 1);
        vpcDetailsLayout->addWidget(vpcIPv4CIDRLabel, 1);
        vpcDetailsLayout->addWidget(vpcStateLabel);
    vpcFrameLayout->addWidget(vpcTitleFrame);
    vpcFrameLayout->addWidget(hline);
    vpcFrameLayout->addWidget(vpcDetailsFrame);
}
