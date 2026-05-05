#include "awsmanager.h"
#include <vector>
#include <QString>
#include <QDebug>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/ec2/EC2Client.h>

AWSManager &AWSManager::instance(){
    static AWSManager instance;
    return instance;
}

AWSManager::AWSManager() {}

AWSManager::~AWSManager() {}

void AWSManager::initSDK(){

    Aws::SDKOptions options;
    Aws::InitAPI(options);
}

void AWSManager::setSelectedProfile(QString profile){
    *selectedProfile = profile;
}

QString AWSManager::getSelectedProfile(){
    return *selectedProfile;
}

std::vector<Aws::EC2::Model::Vpc> AWSManager::getVPCs(QString selectedProfile){

    Aws::Client::ClientConfiguration config;
    config.region = "us-east-1";
    config.profileName = selectedProfile.toStdString();

    Aws::EC2::EC2Client ec2(config);

    Aws::EC2::Model::DescribeVpcsRequest request;
    auto outcome = ec2.DescribeVpcs(request);

    if (!outcome.IsSuccess()){
        qDebug() << outcome.GetError().GetMessage();
    }
    return outcome.GetResult().GetVpcs();
}
