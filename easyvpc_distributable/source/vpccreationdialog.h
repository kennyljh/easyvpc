#ifndef VPCCREATIONDIALOG_H
#define VPCCREATIONDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <vector>
#include <QList>
#include <QStringList>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

class VPCCreationDialog : public QDialog{
    Q_OBJECT
    public:
        explicit VPCCreationDialog(QWidget *parent = 0);

        struct subnetInfo{
            QString name;
            QString zone;
            QString ipv4;
        };

        struct RTInfo{
            QString name;
            QStringList subnets;
        };

    private:
        struct subnetBundle{
            QLineEdit *name;
            QComboBox *zone;
            QLineEdit *ipv4;
        };

        struct RTBundle{
            QLineEdit *name;
            QListWidget *subnets;
        };

        QStringList AZones;

        std::vector<subnetBundle> subnetBundles;
        std::vector<RTBundle> RTBundles;

        QList<subnetInfo> subnetInfos;
        QList<RTInfo> RTInfos;

        QVBoxLayout *mainLayout;
        QScrollArea *createVPCScrollArea;
        QFrame *createVPCFrame;
        QVBoxLayout *createVPCLayout;

        QLabel *vpcNameLabel;
        QLineEdit *vpcNameEdt;
        QLabel *vpcIPv4CIDRLabel;
        QLineEdit *vpcIPv4CIDREdt;

        QLabel *subnetsLabel;
        QFrame *subnetsFrame;
        QVBoxLayout *subnetsLayout;
        QLabel *subnetNameLabel;
        QLineEdit *subnetNameEdt;
        QLabel *AZsLabel;
        QComboBox *regionsCBox;
        QLabel *subnetIPv4CIDRLabel;
        QLineEdit *subnetIPv4CIDREdt;
        QPushButton *addSubnetBtn;

        QLabel *igwLabel;
        QLineEdit *igwEdt;

        QLabel *RTsLabel;
        QFrame *RTsFrame;
        QVBoxLayout *RTsLayout;
        QLabel *RTNameLabel;
        QLineEdit *RTNameEdt;
        QPushButton *refreshSubnetsBtn;
        QListWidget *subnetsLWidget;
        QPushButton *addRTBtn;

        QPushButton *createBtn;

        /**
         * @brief createVPCRequest - bundles all vpc related information
         * into creation request
         */
        void createVPCRequest();

    private slots:
        void subnetsRefreshButtonClicked();

        /**
         * @brief processZones - processes retrieved list availability zones
         * for current region into frame
         * @param zones
         */
        void processZones(const std::vector<Aws::EC2::Model::AvailabilityZone> &zones);

        /**
         * @brief addSubnetFrame - appends another widget for subnet
         * creation
         */
        void addSubnetFrame();

        /**
         * @brief addRTFrame - appends another widget for RT
         * creation
         */
        void addRTFrame();

    signals:
        /**
         * @brief VPCCreationRequested - emits signal for VPC creation, includes
         * all vpc related information
         * @param vpcName
         * @param vpcCIDR
         * @param subnetInfos
         * @param igwName
         * @param RTInfos
         */
        void VPCCreationRequested(
            QString &vpcName,
            QString &vpcCIDR,
            const QList<VPCCreationDialog::subnetInfo> &subnetInfos,
            QString &igwName,
            const QList<VPCCreationDialog::RTInfo> &RTInfos
        );
};

#endif // VPCCREATIONDIALOG_H
