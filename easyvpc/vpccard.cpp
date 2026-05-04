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

    setFrameShape(QFrame::StyledPanel);
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(1);
    setMidLineWidth(3);

    vpcFrameWindow = new QWidget(this);
    vpcFrameLayout = new QVBoxLayout(vpcFrameWindow);
        vpcTitleWindow = new QWidget(vpcFrameWindow);
        vpcTitleLayout = new QHBoxLayout(vpcTitleWindow);
            vpcName = new QLabel(name, vpcTitleWindow);
            expandBtn = new QPushButton("Expand", vpcTitleWindow);
            deleteBtn = new QPushButton("Delete", vpcTitleWindow);
        vpcTitleLayout->addWidget(vpcName, 0, Qt::AlignLeft);
        vpcTitleLayout->addWidget(expandBtn, 0, Qt::AlignRight);
        vpcTitleLayout->addWidget(deleteBtn, 0, Qt::AlignRight);

        vpcID = new QLabel(id, vpcFrameWindow);
        vpcIPv4CIDR = new QLabel(ipv4cidr, vpcFrameWindow);
    vpcFrameLayout->addWidget(vpcTitleWindow);
    vpcFrameLayout->addWidget(vpcID);
    vpcFrameLayout->addWidget(vpcIPv4CIDR);
}
