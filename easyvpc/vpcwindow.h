#ifndef VPCWINDOW_H
#define VPCWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QStatusBar>
#include <QMap>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>
#include "vpccreationdialog.h"

class VPCWindow : public QMainWindow{
    Q_OBJECT
    public:
        explicit VPCWindow(QWidget *parent = 0);

    private:
        struct vpcDetails{
            QString name;
            QString id;
            QString ipv4cidr;
            QString state;

            std::vector<Aws::EC2::Model::Subnet> subnets;
            std::vector<Aws::EC2::Model::RouteTable> routeTables;
            std::vector<Aws::EC2::Model::InternetGateway> igws;
            std::vector<Aws::EC2::Model::NatGateway> natGateways;
            std::vector<Aws::EC2::Model::SecurityGroup> securityGroups;
            std::vector<Aws::EC2::Model::NetworkAcl> acls;
        };

        QMap<QString, vpcDetails*> vpcCache;

        QWidget *centralWindow;
        QVBoxLayout *centralLayout;

        QPushButton *backToHubBtn;
        QComboBox *regionsCBox;
        QFrame *topBarFrame;
        QHBoxLayout *topBarLayout;

        QFrame *myVPCBarFrame;
        QHBoxLayout *myVPCBarLayout;
        QLabel *welcomeName;
        QPushButton *refreshBtn;
        QPushButton *expandAllBtn;
        QPushButton *createBtn;

        QScrollArea *myVPCScrollArea;
        QWidget *myVPCWindow;
        QVBoxLayout *myVPCLayout;

    private slots:
        void processRegions(const std::vector<Aws::EC2::Model::Region> &regions);

        /**
         * @brief processVPCs - inserts vpcs into appropriate widgets, cache
         * vpcs based on id
         * @param vpcs
         */
        void processVPCs(const std::vector<Aws::EC2::Model::Vpc> &vpcs);

        /**
         * @brief setStatusBar - sets status bar message
         * @param msg
         */
        void setStatusBar(QString msg);

        /**
         * @brief cacheSubnets - cache subnets to vpc cache
         * @param subnets
         */
        void cacheSubnets(const std::vector<Aws::EC2::Model::Subnet> &subnets);

        void refreshButtonClicked();

        void createVPCButtonClicked();

        void VPCCreationDataDebug(
            QString &vpcName,
            QString &vpcCIDR,
            const QList<VPCCreationDialog::subnetInfo> &subnetInfos,
            QString &igwName,
            const QList<VPCCreationDialog::RTInfo> &RTInfos
        );
};

#endif // VPCWINDOW_H
