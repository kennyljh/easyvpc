#ifndef AWSMANAGER_H
#define AWSMANAGER_H

#include <QObject>
#include <vector>
#include <QString>
#include <QMap>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

/**
 * @brief The AWSManager class - singleton, is responsible for all
 * asynchronous API calls using the AWS SDK.
 *
 * Emits signals on async call completion, both successes and
 * failures.
 */
class AWSManager : public QObject {
    Q_OBJECT
    public:
        static AWSManager &instance();

        AWSManager(const AWSManager&) = delete;
        AWSManager &operator=(const AWSManager&) = delete;

        struct vpcDetails{

            std::vector<Aws::EC2::Model::Subnet> subnets;
            std::vector<Aws::EC2::Model::RouteTable> routeTables;
            std::vector<Aws::EC2::Model::InternetGateway> igw;
            // std::vector<Aws::EC2::Model::NatGateway> natGateways;
            // std::vector<Aws::EC2::Model::SecurityGroup> securityGroups;
            // std::vector<Aws::EC2::Model::NetworkAcl> acls;
        };

        /**
         * @brief vpcIDCache vpc id to vpc details cache for all vpcs
         * discovered
         */
        QMap<QString, vpcDetails> vpcIDCache;

        /**
         * @brief initSDK - initializes SDK, should only be called
         * once
         */
        void initSDK();

        /**
         * @brief terminateSDK - terminates SDK, should ony be called
         * once
         */
        void terminateSDK();

        /**
         * @brief setSelectedProfile - sets current profile
         * @param selectedProfile
         */
        void setSelectedProfile(QString selectedProfile);

        /**
         * @brief getSelectedProfile - gets current profile
         * @return "default" if no profile setup
         */
        QString getSelectedProfile();

        /**
         * @brief setSelectedRegion - sets current region
         * @param selectedRegion
         */
        void setSelectedRegion(QString selectedRegion);

        /**
         * @brief getSelectedRegion - gets current region
         * @return "us-east-1" if no region setup
         */
        QString getSelectedRegion();

        /**
         * @brief getRegionsAsync - makes async call to get regions
         */
        void getRegionsAsync();

        /**
         * @brief getVPCs - makes async call to get vpcs for current
         * profile and region
         */
        void getVPCsAsync();

        /**
         * @brief getSubnetsAsync - makes async call to get subnets for
         * current profile, region, and vpc id
         */
        void getSubnetsAsync(QString vpcID);

        /**
         * @brief getEC2sAsync - makes async call to get reservations for
         * current profile, region, and subnet id
         * @param subnetID
         */
        void getReservationsAsync(QString subnetID);

        /**
         * @brief getRTAsync - makes async call to get RTs for current
         * profile, region, and subnet id
         * @param subnetID
         */
        void getRTAsync(QString subnetID);

        /**
         * @brief getACLsAsync - makes async call to get ACLs for
         * current profile, region, and subnet id
         * @param subnetID
         */
        void getACLsAsync(QString subnetID);

        /**
         * @brief getRTsByVPCIdAsync - makes async call to get RTs for
         * current profile, region, and vpc id
         * @param vpcID
         */
        void getRTsByVPCIdAsync(QString vpcID);

        /**
         * @brief getIGWByVPCIdAsync - makes async call to get IGW for
         * current profile, region, and vpc id
         * @param vpcID
         */
        void getIGWByVPCIdAsync(QString vpcID);

        /**
         * @brief getZonesAsync - makes async call to get AZs for
         * current region
         */
        void getZonesAsync();

        /**
         * @brief createVPCAsync - makes async call to create VPC
         * for current region & profile
         * @param vpcName
         * @param vpcCIDR
         */
        void createVPCAsync(QString vpcName, QString vpcCIDR);

        /**
         * @brief createSubnetAsync - makes async call to create
         * subnet for current region, profile, and VPC
         * @param vpcID
         * @param subnetName
         * @param zone
         * @param CIDR
         */
        void createSubnetAsync(
            QString vpcID,
            QString subnetName,
            QString zone,
            QString CIDR
        );

        /**
         * @brief createIGWAsync - makes async call to create
         * IGW for current region, profile, and VPC
         * @param vpcID
         * @param igwName
         */
        void createIGWAsync(
            QString vpcID,
            QString igwName
        );

        /**
         * @brief createRTAsync - makes async call to create
         * RT for current region, profile, and VPC
         * @param vpcID
         * @param RTName
         * @param igwID
         * @param subnetIDs
         */
        void createRTAsync(
            QString vpcID,
            QString RTName,
            QString igwID,
            QStringList subnetIDs
        );

        /**
         * @brief deleteRTsByVPCIdAsync - makes async call
         * to delete RTs for current VPC
         * @param vpcID
         */
        void deleteRTsByVPCIdAsync(QString vpcID);

        /**
         * @brief deleteIGWByVPCIdAsync - makes async call
         * to delete IGW for current VPC
         * @param vpcID
         */
        void deleteIGWByVPCIdAsync(QString vpcID);

        /**
         * @brief deleteSubnetsByVPCIdAsync - makes async
         * call to delete subnets for current VPC
         * @param vpcID
         */
        void deleteSubnetsByVPCIdAsync(QString vpcID);

        /**
         * @brief deleteVPCByVPCIdAsync - makes async call
         * to delete VPC by id
         * @param vpcID
         */
        void deleteVPCByVPCIdAsync(QString vpcID);


    signals:
        /**
         * @brief regionsReady - signals when list of regions retrieved
         * @param regions
         */
        void regionsReady(const std::vector<Aws::EC2::Model::Region> &regions);

        /**
         * @brief vpcsReady - signals when list of vpcs retrieved
         * @param vpcs
         */
        void vpcsReady(const std::vector<Aws::EC2::Model::Vpc> &vpcs);

        /**
         * @brief subnetsReady - signals when list of subnets retrieved for vpc id
         * @param subnets
         */
        void subnetsReady(const QString &vpcId,
                            const std::vector<Aws::EC2::Model::Subnet> &subnets);

        /**
         * @brief reservationsByIdReady - signals when reservations retrieved for subnet id
         * @param subnetId
         * @param reservations
         */
        void reservationsByIdReady(const QString &subnetId,
                                    const std::vector<Aws::EC2::Model::Reservation> &reservations);

        /**
         * @brief RTByIdReady - signals when RT retrieved for subnet id
         * @param subnetId
         * @param RT
         */
        void RTByIdReady(const QString &subnetId,
                            const std::vector<Aws::EC2::Model::RouteTable> &RT);

        /**
         * @brief ACLsByIdReady - signals when ACLs retrieved for subnet id
         * @param subnetId
         * @param ACLs
         */
        void ACLsByIdReady(const QString &subnetId,
                            const std::vector<Aws::EC2::Model::NetworkAcl> &ACLs);

        /**
         * @brief RTsByVPCIdReady - signals when RT retrieved for vpc id
         * @param vpcId
         * @param RTs
         */
        void RTsByVPCIdReady(const QString &vpcId,
                                const std::vector<Aws::EC2::Model::RouteTable> &RTs);

        /**
         * @brief IGWByVPCIdReady - signals when IGW retrieved for vpc id
         * @param vpcId
         * @param IGW
         */
        void IGWByVPCIdReady(const QString &vpcId,
                                const std::vector<Aws::EC2::Model::InternetGateway> &IGW);

        /**
         * @brief zonesReady - signals when retrieved availability
         * zones for current region
         * @param zones
         */
        void zonesReady(const std::vector<Aws::EC2::Model::AvailabilityZone> &zones);

        /**
         * @brief apiError - signals when there is an api call error
         * @param err
         */
        void apiError(const QString &err);

        /**
         * @brief notifyStatus - signals to notify status change
         * @param status
         */
        void notifyStatus(const QString &status);

        /**
         * @brief vpcCreated - signals when vpc is created
         * @param vpcID
         */
        void vpcCreated(const QString &vpcID);

        /**
         * @brief subnetCreated - signals when subnet for
         * current vpc is created and associated
         * @param subnetID
         * @param subnetName
         */
        void subnetCreated(const QString &subnetID,
                            const QString &subnetName);

        /**
         * @brief igwCreated - signals when IGW for
         * current vpc is created and associated
         * @param igwID
         */
        void igwCreated(const QString &igwID);

        /**
         * @brief routeTableCreated - signals when RT
         * for current vpc is created and associated
         * @param rtID
         */
        void routeTableCreated(const QString &rtID);

        /**
         * @brief RTsDeletionCompleted - signals when
         * RTs for current vpc are deleted
         * @param vpcID
         */
        void RTsDeletionCompleted(const QString &vpcID);

        /**
         * @brief IGWDeletionCompleted - signals when IGW
         * for current vpc is deleted
         * @param vpcID
         */
        void IGWDeletionCompleted(const QString &vpcID);

        /**
         * @brief SubnetsDeletionCompleted - signals when subnets
         * for current vpc are deleted
         * @param vpcID
         */
        void SubnetsDeletionCompleted(const QString &vpcID);

        /**
         * @brief VPCDeletionCompleted - signals when vpc is
         * deleted
         * @param vpcID
         */
        void VPCDeletionCompleted(const QString &vpcID);

    private:
        explicit AWSManager(QObject *parent = nullptr);
        ~AWSManager();

        /**
         * @brief selectedProfile - configured current profile
         */
        QString selectedProfile = "default";

        /**
         * @brief selectedRegion - configured current region
         */
        QString selectedRegion = "us-east-1";

        Aws::SDKOptions options;
};

#endif // AWSMANAGER_H
