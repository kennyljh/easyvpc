#ifndef AWSMANAGER_H
#define AWSMANAGER_H

#include <QObject>
#include <vector>
#include <QString>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

/**
 * @brief The AWSManager class - singleton, is responsible for all
 * asynchronous API calls using the AWS SDK
 */
class AWSManager : public QObject {
    Q_OBJECT
    public:
        static AWSManager &instance();

        AWSManager(const AWSManager&) = delete;
        AWSManager &operator=(const AWSManager&) = delete;

        /**
         * @brief initSDK - initializes SDK, should only be called
         * once
         */
        void initSDK();

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
         * @brief getRegionsAsync - returns vector of available
         * regions
         */
        void getRegionsAsync();

        /**
         * @brief getVPCs - returns vector of VPC objects based on
         * current profile and region setup
         */
        void getVPCsAsync();

        /**
         * @brief getSubnetsAsync - returns vector of subnets based
         * on profile, region, and vpc id
         */
        void getSubnetsAsync(QString vpcID);

        void getEC2sAsync(QString subnetID);

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
         * @brief subnetsReady - signals when list of subnets retrieved
         * @param subnets
         */
        void subnetsReady(const std::vector<Aws::EC2::Model::Subnet> &subnets);

        void ec2sByIdReady(const std::vector<Aws::EC2::Model::Reservation> &ec2s);

        /**
         * @brief apiError - signals when there is an api call error
         * @param err
         */
        void apiError(const QString &err);

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
