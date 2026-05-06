#include "subnetcard.h"
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

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
        subnetMiscLayout->addWidget(subnetDetailsFrame);
    mainLayout->addWidget(subnetTopFrame);
    mainLayout->addWidget(subnetMiscFrame);
}
