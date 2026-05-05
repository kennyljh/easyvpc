#ifndef AWSMANAGER_H
#define AWSMANAGER_H

#include <vector>
#include <QString>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

class AWSManager {
    public:
        AWSManager(const AWSManager&) = delete;
        AWSManager &operator=(const AWSManager&) = delete;
        AWSManager(const AWSManager&&) = delete;
        AWSManager &operator=(const AWSManager&&) = delete;

        static AWSManager &instance();

        void initSDK();

        // void terminateSDK();

        void setSelectedProfile(QString selectedProfile);

        QString getSelectedProfile();

        /**
         * @brief getVPCs - returns vector of VPC objects for current
         * SDK profile
         * @return
         */
        std::vector<Aws::EC2::Model::Vpc> getVPCs(QString selectedProfile);

    private:
        AWSManager();
        ~AWSManager();

        QString *selectedProfile = new QString("default");
};

#endif // AWSMANAGER_H
