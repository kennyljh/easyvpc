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
        vpcTitleLayout->addWidget(vpcName);
        vpcTitleLayout->addStretch();
        vpcTitleLayout->addWidget(expandBtn);
        vpcTitleLayout->addWidget(deleteBtn);
        QFrame *hline = new QFrame(this);
        hline->setFrameStyle(QFrame::HLine | QFrame::Raised);
        vpcID = new QLabel("VPC ID: " + id, this);
        vpcIPv4CIDR = new QLabel("IPv4 CIDR: " + ipv4cidr, this);
        vpcState = new QLabel("VPC State: " + state, this);
    vpcFrameLayout->addWidget(vpcTitleFrame);
    vpcFrameLayout->addWidget(hline);
    vpcFrameLayout->addWidget(vpcID);
    vpcFrameLayout->addWidget(vpcIPv4CIDR);
    vpcFrameLayout->addWidget(vpcState);

    setFixedHeight(150);
}
