#ifndef SUBNETCARD_H
#define SUBNETCARD_H

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

class SubnetCard : public QFrame{
    Q_OBJECT
    public:
        explicit SubnetCard(const QString &vpcid, const QString &name, const QString &subnetid,
                                const QString &ipv4cidr, const QString &ipAddrCount,
                                const QString &zoneID, const QString &zone,
                                const QString &state, QWidget *parent = nullptr);
    private:
        QString vpcID;
        QString subnetID;

        QVBoxLayout *mainLayout;

        QFrame *subnetTopFrame;
        QHBoxLayout *subnetTopLayout;
        QLabel *subnetNameLabel;
        QPushButton *deleteSubnetBtn;

        QFrame *subnetMiscFrame;
        QHBoxLayout *subnetMiscLayout;

        QFrame *subnetDetailsFrame;
        QVBoxLayout *subnetDetailsLayout;
        QLabel *subnetDetailsLabel;
        QLabel *subnetIDLabel;
        QLabel *subnetIPv4CIDRLabel;
        QLabel *subnetIPAddrCountLabel;
        QLabel *subnetZoneLabel;
        QLabel *subnetStateLabel;

        QFrame *subnetEC2sFrame;
        QVBoxLayout *subnetEC2sLayout;
        QFrame *ec2sTopFrame;
        QHBoxLayout *ec2sTopLayout;
        QLabel *ec2sLabel;
        QPushButton *ec2sManageBtn;
        QScrollArea *ec2sScrollArea;
        QFrame *ec2sFrame;
        QVBoxLayout *ec2sLayout;

        QFrame *subnetRTAndACLsFrame;
        QVBoxLayout *subnetRTAndACLsLayout;
        QFrame *subnetRTFrame;
        QVBoxLayout *subnetRTLayout;
        QLabel *subnetRTLabel;
        QFrame *subnetACLsFrame;
        QVBoxLayout *subnetACLsLayout;
        QLabel *subnetACLsLabel;

    private slots:
        /**
         * @brief processEC2s - process EC2s into appropriate subnet widget
         * @param subnetId
         * @param reservations
         */
        void processEC2s(const QString &subnetId,
                            const std::vector<Aws::EC2::Model::Reservation> &reservations);

        /**
         * @brief processRT - process RTs into approproiate subnet widget
         * @param subnetId
         * @param RT
         */
        void processRT(const QString &subnetId,
                        const std::vector<Aws::EC2::Model::RouteTable> &RT);

        /**
         * @brief processACLs - process ACLs into appropriate subnet widget
         * @param subnetId
         * @param ACLs
         */
        void processACLs(const QString &subnetId,
                            const std::vector<Aws::EC2::Model::NetworkAcl> &ACLs);
};

#endif // SUBNETCARD_H
