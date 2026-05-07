#include "rtcard.h"
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

RTCard::RTCard(const QString &vpcid, const QString &name, const QString &rtid,
                            const QString &ownerId, QWidget *parent)
        : QFrame (parent) {

    vpcID = vpcid;
    rtID = rtid;

    QFont qfontB13;
    qfontB13.setBold(true);
    qfontB13.setPointSize(13);

    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setLineWidth(3);
    setMidLineWidth(3);

    mainLayout = new QVBoxLayout(this);
        RTTopFrame = new QFrame(this);
        RTTopLayout = new QHBoxLayout(RTTopFrame);
            RTNameLabel = new QLabel(name, RTTopFrame);
            RTNameLabel->setFont(qfontB13);
            deleteRTBtn = new QPushButton("Delete", RTTopFrame);
        RTTopLayout->addWidget(RTNameLabel);
        RTTopLayout->addStretch();
        RTTopLayout->addWidget(deleteRTBtn);

        RTMiscFrame = new QFrame(this);
        RTMiscLayout = new QHBoxLayout(RTMiscFrame);
            RTDetailsFrame = new QFrame(RTMiscFrame);
            RTDetailsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            RTDetailsFrame->setLineWidth(2);
            RTDetailsLayout = new QVBoxLayout(RTDetailsFrame);
                RTDetailsLabel = new QLabel("Details");
                QFrame *hline = new QFrame(RTDetailsFrame);
                hline->setFrameStyle(QFrame::HLine | QFrame::Raised);
                RTDetailsLabel->setFont(qfontB13);
                RTIDLabel = new QLabel("ID: " + rtid);
                RTOwnerIDLabel = new QLabel("Owner ID: " + ownerId);
            RTDetailsLayout->addWidget(RTDetailsLabel);
            RTDetailsLayout->addWidget(hline);
            RTDetailsLayout->addWidget(RTIDLabel);
            RTDetailsLayout->addWidget(RTOwnerIDLabel);

            RTSubnetsFrame = new QFrame(RTMiscFrame);
            RTSubnetsFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            RTSubnetsFrame->setLineWidth(2);
            RTSubnetsLayout = new QVBoxLayout(RTSubnetsFrame);
                subnetsTopFrame = new QFrame(RTSubnetsFrame);
                subnetsTopLayout = new QHBoxLayout(subnetsTopFrame);
                    subnetsLabel = new QLabel("Subnets", subnetsTopFrame);
                    subnetsLabel->setFont(qfontB13);
                subnetsTopLayout->addWidget(subnetsLabel);

                subnetsScrollArea = new QScrollArea(RTSubnetsFrame);
                    subnetsFrame = new QFrame(RTSubnetsFrame);
                    subnetsLayout = new QVBoxLayout(subnetsFrame);
                subnetsScrollArea->setWidget(subnetsFrame);
                subnetsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                subnetsScrollArea->setWidgetResizable(true);
            RTSubnetsLayout->addWidget(subnetsTopFrame);
            RTSubnetsLayout->addWidget(subnetsScrollArea);

            RTRoutesFrame = new QFrame(RTMiscFrame);
            RTRoutesFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
            RTRoutesFrame->setLineWidth(2);
            RTRoutesLayout = new QVBoxLayout(RTRoutesFrame);
                routesTopFrame = new QFrame(RTRoutesFrame);
                routesTopLayout = new QHBoxLayout(routesTopFrame);
                    routesLabel = new QLabel("Routes", routesTopFrame);
                    routesLabel->setFont(qfontB13);
                routesTopLayout->addWidget(routesLabel);

                routesScrollArea = new QScrollArea(RTRoutesFrame);
                    routesFrame = new QFrame(RTRoutesFrame);
                    routesLayout = new QVBoxLayout(routesFrame);
                routesScrollArea->setWidget(routesFrame);
                routesScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
                routesScrollArea->setWidgetResizable(true);
            RTRoutesLayout->addWidget(routesTopFrame);
            RTRoutesLayout->addWidget(routesScrollArea);
        RTMiscLayout->addWidget(RTDetailsFrame, 1);
        RTMiscLayout->addWidget(RTSubnetsFrame, 1);
        RTMiscLayout->addWidget(RTRoutesFrame, 1);
    mainLayout->addWidget(RTTopFrame);
    mainLayout->addWidget(RTMiscFrame);

}
