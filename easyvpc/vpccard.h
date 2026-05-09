#ifndef VPCCARD_H
#define VPCCARD_H

#include <QFrame>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

class VPCCard : public QFrame{
    Q_OBJECT
    public:
        explicit VPCCard(const QString &name, const QString &id,
                            const QString &ipv4cidr, const QString &state,
                            QWidget *parent = nullptr);
    private:
        /**
         * @brief vpcID - id associated with this vpc card
         */
        QString vpcID;

        QVBoxLayout *vpcFrameLayout;

        QFrame *vpcTitleFrame;
        QFrame *titleHLine;
        QHBoxLayout *vpcTitleLayout;

        QPushButton *expandBtn;
        QPushButton *minimizeBtn;
        QPushButton *deleteBtn;
        QLabel *vpcName;

        QFrame *vpcDetailsFrame;
        QHBoxLayout *vpcDetailsLayout;
        QLabel *vpcIDLabel;
        QLabel *vpcIPv4CIDRLabel;
        QLabel *vpcStateLabel;

        QFrame *subnetMainFrame;
        QVBoxLayout *subnetMainLayout;
        QFrame *subnetTopFrame;
        QHBoxLayout *subnetTopLayout;
        QLabel *subnetLabel;
        QPushButton *addSubnetBtn;
        QScrollArea *subnetScrollArea;
        QWidget *subnetsWindow;
        QVBoxLayout *subnetsLayout;

        QFrame *RTMainFrame;
        QVBoxLayout *RTMainLayout;
        QFrame *RTTopFrame;
        QHBoxLayout *RTTopLayout;
        QLabel *RTLabel;
        QPushButton *addRTBtn;
        QScrollArea *RTScrollArea;
        QWidget *RTsWindow;
        QVBoxLayout *RTsLayout;

        QFrame *IGWMainFrame;
        QVBoxLayout *IGWMainLayout;
        QFrame *IGWTopFrame;
        QHBoxLayout *IGWTopLayout;
        QLabel *IGWLabel;
        QPushButton *IGWAddBtn;

        QFrame *IGWDetailsFrame;
        QVBoxLayout *IGWDetailsLayout;
        QFrame *nameAndBtnFrame;
        QHBoxLayout *nameAndBtnLayout;
        QLabel *IGWNameLabel;
        QPushButton *deleteIGWBtn;
        QFrame *idAndOwnerIdFrame;
        QHBoxLayout *idAndOwnerIdLayout;
        QLabel *IGWIdLabel;
        QLabel *IGWOwnerIdLabel;

        /**
         * @brief expandCard - expands vpc card with placeholder frames, recommended
         * to run first before async requests
         * @return 0 - if complete
         */
        void expandCard();

    private slots:
        /**
         * @brief processSubnets - adds subnets into appropriate subnet frame for given vpc id
         * @param subnets
         */
        void processSubnets(const QString &vpcId,
                                const std::vector<Aws::EC2::Model::Subnet> &subnets);

        /**
         * @brief processRTs - adds route tables into appropriate rt frame for given vpc id
         * @param vpcId
         * @param RTs
         */
        void processRTs(const QString &vpcId,
                            const std::vector<Aws::EC2::Model::RouteTable> &RTs);

        /**
         * @brief processIGW - adds internet gateway into appropriate igw frame for given vpc id
         * @param vpcId
         * @param IGW
         */
        void processIGW(const QString &vpcId,
                            const std::vector<Aws::EC2::Model::InternetGateway> &IGW);

        /**
         * @brief expandTriggered - expand current vpc card to include
         * detailed view
         */
        void vpcExpandTriggered();

        /**
         * @brief vpcMinimizeTriggered - minimize current vpc card to
         * simple view
         */
        void vpcMinimizeTriggered();

        /**
         * @brief deleteVPCButtonClicked - delete current vpc button
         * triggered
         */
        void deleteVPCButtonClicked();
};

#endif // VPCCARD_H
