#ifndef RTCARD_H
#define RTCARD_H

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QString>
#include <QStringList>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

class RTCard : public QFrame{
    Q_OBJECT
    public:
        explicit RTCard(const QString &vpcid, const QString &name, const QString &rtid,
                            const QString &ownerId, const QStringList &subnetIds,
                            const QString &gatewayId, QWidget *parent = nullptr);

    private:
        QString vpcID;
        QString rtID;

        QVBoxLayout *mainLayout;

        QFrame *RTTopFrame;
        QHBoxLayout *RTTopLayout;
        QLabel *RTNameLabel;
        QPushButton *deleteRTBtn;

        QFrame *RTMiscFrame;
        QHBoxLayout *RTMiscLayout;

        QFrame *RTDetailsFrame;
        QVBoxLayout *RTDetailsLayout;
        QLabel *RTDetailsLabel;
        QLabel *RTIDLabel;
        QLabel *RTOwnerIDLabel;

        QFrame *RTSubnetsFrame;
        QVBoxLayout *RTSubnetsLayout;
        QFrame *subnetsTopFrame;
        QHBoxLayout *subnetsTopLayout;
        QLabel *subnetsLabel;
        QScrollArea *subnetsScrollArea;
        QFrame *subnetsFrame;
        QVBoxLayout *subnetsLayout;

        QFrame *RTRoutesFrame;
        QVBoxLayout *RTRoutesLayout;
        QFrame *routesTopFrame;
        QHBoxLayout *routesTopLayout;
        QLabel *routesLabel;
        QScrollArea *routesScrollArea;
        QFrame *routesFrame;
        QVBoxLayout *routesLayout;
};

#endif // RTCARD_H
