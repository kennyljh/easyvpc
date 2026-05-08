#include "awsmanager.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QtConcurrent>
#include <vector>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/ec2/EC2Client.h>
#include <aws/ec2/model/CreateTagsRequest.h>
#include <aws/ec2/model/ModifyVpcAttributeRequest.h>
#include <aws/ec2/model/CreateSubnetRequest.h>
#include <aws/ec2/model/ModifySubnetAttributeRequest.h>
#include <aws/ec2/model/CreateInternetGatewayRequest.h>
#include <aws/ec2/model/AttachInternetGatewayRequest.h>

AWSManager::AWSManager(QObject *parent) : QObject(parent) {}

AWSManager &AWSManager::instance(){
    static AWSManager instance;
    return instance;
}

AWSManager::~AWSManager() {}

void AWSManager::initSDK(){
    Aws::InitAPI(options);
}

void AWSManager::terminateSDK(){
    Aws::ShutdownAPI(options);
}

void AWSManager::setSelectedProfile(QString profile){
    selectedProfile = profile;
}

QString AWSManager::getSelectedProfile(){
    return selectedProfile;
}

void AWSManager::setSelectedRegion(QString region){
    selectedRegion = region;
}

QString AWSManager::getSelectedRegion(){
    return selectedRegion;
}

void AWSManager::getRegionsAsync(){

    QtConcurrent::run([this]() {

        Aws::EC2::EC2Client ec2;

        Aws::EC2::Model::DescribeRegionsRequest request;
        auto outcome = ec2.DescribeRegions(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
        }
        else {

            auto regions = outcome.GetResult().GetRegions();

            QMetaObject::invokeMethod(this, [this, regions]() {
                emit regionsReady(regions);
            });
        }
    });
}

void AWSManager::getVPCsAsync(){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    QtConcurrent::run([this, profile, region]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DescribeVpcsRequest request;
        auto outcome = ec2.DescribeVpcs(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
        }
        else {

            auto vpcs = outcome.GetResult().GetVpcs();

            QMetaObject::invokeMethod(this, [this, vpcs]() {
                emit vpcsReady(vpcs);
            });
        }
    });
}

void AWSManager::getSubnetsAsync(QString vpcID){

    QString profile = selectedProfile;
    QString region = selectedRegion;
    QString id = vpcID;

    QtConcurrent::run([this, profile, region, id]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DescribeSubnetsRequest request;
        request.AddFilters(
            Aws::EC2::Model::Filter().WithName("vpc-id")
                                     .AddValues(id.toStdString())
        );
        auto outcome = ec2.DescribeSubnets(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
        }
        else {

            auto subnets = outcome.GetResult().GetSubnets();

            QMetaObject::invokeMethod(this, [this, id, subnets]() {
                emit subnetsReady(id, subnets);
                emit notifyStatus("Found " + QString::number(subnets.size()) + " subnets");
            });
        }
    });
}

void AWSManager::getReservationsAsync(QString subnetID){

    QString profile = selectedProfile;
    QString region = selectedRegion;
    QString id = subnetID;

    QtConcurrent::run([this, profile, region, id]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DescribeInstancesRequest request;
        request.AddFilters(
            Aws::EC2::Model::Filter().WithName("subnet-id")
                                     .AddValues(id.toStdString())
        );
        auto outcome = ec2.DescribeInstances(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
        }
        else {

            auto reservations = outcome.GetResult().GetReservations();

            QMetaObject::invokeMethod(this, [this, id, reservations]() {
                emit reservationsByIdReady(id, reservations);
            });
        }
    });
}

void AWSManager::getRTAsync(QString subnetID){

    QString profile = selectedProfile;
    QString region = selectedRegion;
    QString id = subnetID;

    QtConcurrent::run([this, profile, region, id]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DescribeRouteTablesRequest request;
        request.AddFilters(
            Aws::EC2::Model::Filter().WithName("association.subnet-id")
                                     .AddValues(id.toStdString())
        );
        auto outcome = ec2.DescribeRouteTables(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
        }
        else {

            auto RT = outcome.GetResult().GetRouteTables();

            QMetaObject::invokeMethod(this, [this, id, RT]() {
                emit RTByIdReady(id, RT);
            });
        }
    });
}

void AWSManager::getACLsAsync(QString subnetID){

    QString profile = selectedProfile;
    QString region = selectedRegion;
    QString id = subnetID;

    QtConcurrent::run([this, profile, region, id]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DescribeNetworkAclsRequest request;
        request.AddFilters(
            Aws::EC2::Model::Filter().WithName("association.subnet-id")
                                     .AddValues(id.toStdString())
        );
        auto outcome = ec2.DescribeNetworkAcls(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
        }
        else {

            auto ACLs = outcome.GetResult().GetNetworkAcls();

            QMetaObject::invokeMethod(this, [this, id, ACLs]() {
                emit ACLsByIdReady(id, ACLs);
            });
        }
    });
}

void AWSManager::getRTsByVPCIdAsync(QString vpcID){

    QString profile = selectedProfile;
    QString region = selectedRegion;
    QString id = vpcID;

    QtConcurrent::run([this, profile, region, id]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DescribeRouteTablesRequest request;
        request.AddFilters(
            Aws::EC2::Model::Filter().WithName("vpc-id")
                                     .AddValues(id.toStdString())
        );
        auto outcome = ec2.DescribeRouteTables(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
        }
        else {

            auto RTs = outcome.GetResult().GetRouteTables();

            QMetaObject::invokeMethod(this, [this, id, RTs]() {
                emit RTsByVPCIdReady(id, RTs);
                emit notifyStatus("Found " + QString::number(RTs.size()) + " route tables");
            });
        }
    });
}

void AWSManager::getIGWByVPCIdAsync(QString vpcID){

    QString profile = selectedProfile;
    QString region = selectedRegion;
    QString id = vpcID;

    QtConcurrent::run([this, profile, region, id]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DescribeInternetGatewaysRequest request;
        request.AddFilters(
            Aws::EC2::Model::Filter().WithName("attachment.vpc-id")
                                     .AddValues(id.toStdString())
        );
        auto outcome = ec2.DescribeInternetGateways(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
        }
        else {

            auto IGWs = outcome.GetResult().GetInternetGateways();

            QMetaObject::invokeMethod(this, [this, id, IGWs]() {
                emit IGWByVPCIdReady(id, IGWs);
                emit notifyStatus("Found " + QString::number(IGWs.size()) + " IGW");
            });
        }
    });
}

void AWSManager::createVPCAsync(QString vpcName, QString vpcCIDR){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    QtConcurrent::run([this, profile, region, vpcName, vpcCIDR]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        // vpc creation
        Aws::EC2::Model::CreateVpcRequest request;
        request.SetCidrBlock(vpcCIDR.toStdString());

        auto outcome = ec2.CreateVpc(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
            return;
        }

        // get created vpc
        auto vpc = outcome.GetResult().GetVpc();

        QString vpcID = QString::fromStdString(vpc.GetVpcId());

        // add vpc name tag
        Aws::EC2::Model::CreateTagsRequest tagRequest;

        tagRequest.AddResources(vpc.GetVpcId());

        Aws::EC2::Model::Tag nameTag;
        nameTag.SetKey("Name");
        nameTag.SetValue(vpcName.toStdString());

        tagRequest.AddTags(nameTag);

        ec2.CreateTags(tagRequest);

        // enable dns support
        Aws::EC2::Model::ModifyVpcAttributeRequest dnsSupport;
        dnsSupport.SetVpcId(vpc.GetVpcId());

        Aws::EC2::Model::AttributeBooleanValue dnsSupportValue;
        dnsSupportValue.SetValue(true);

        dnsSupport.SetEnableDnsSupport(dnsSupportValue);

        ec2.ModifyVpcAttribute(dnsSupport);

        // enable dns hostnames
        Aws::EC2::Model::ModifyVpcAttributeRequest dnsHostnames;
        dnsHostnames.SetVpcId(vpc.GetVpcId());

        Aws::EC2::Model::AttributeBooleanValue dnsHostnamesValue;
        dnsHostnamesValue.SetValue(true);

        dnsHostnames.SetEnableDnsHostnames(dnsHostnamesValue);

        ec2.ModifyVpcAttribute(dnsHostnames);

        // emit success signal
        QMetaObject::invokeMethod(this, [this, vpcID]{
           emit vpcCreated(vpcID);
           qDebug() << "Created VPC: " + vpcID;
        });
    });
}

void AWSManager::createSubnetAsync(QString vpcID,
                        QString subnetName,
                        QString zone,
                        QString CIDR){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    QtConcurrent::run([this, profile, region, vpcID, subnetName, zone, CIDR]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        // create subnet
        Aws::EC2::Model::CreateSubnetRequest request;

        request.SetVpcId(vpcID.toStdString());
        request.SetAvailabilityZone(zone.toStdString());
        request.SetCidrBlock(CIDR.toStdString());

        auto outcome = ec2.CreateSubnet(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
            return;
        }

        auto subnet = outcome.GetResult().GetSubnet();

        QString subnetID =
            QString::fromStdString(subnet.GetSubnetId());

        // add name tag
        Aws::EC2::Model::CreateTagsRequest tagRequest;

        tagRequest.AddResources(subnet.GetSubnetId());

        Aws::EC2::Model::Tag nameTag;
        nameTag.SetKey("Name");
        nameTag.SetValue(subnetName.toStdString());

        tagRequest.AddTags(nameTag);

        ec2.CreateTags(tagRequest);

        // enable public ip auto assign
        Aws::EC2::Model::ModifySubnetAttributeRequest subnetAttr;

        subnetAttr.SetSubnetId(subnet.GetSubnetId());

        Aws::EC2::Model::AttributeBooleanValue publicIP;
        publicIP.SetValue(true);

        subnetAttr.SetMapPublicIpOnLaunch(publicIP);

        ec2.ModifySubnetAttribute(subnetAttr);

        // emit success signal
        QMetaObject::invokeMethod(this, [this, subnetID, subnetName]() {
            emit subnetCreated(subnetID, subnetName);
        }, Qt::QueuedConnection);
    });
}

void AWSManager::createIGWAsync(QString vpcID,
                    QString igwName){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    QtConcurrent::run([this, profile, region, vpcID, igwName]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        // create igw
        Aws::EC2::Model::CreateInternetGatewayRequest request;

        auto outcome = ec2.CreateInternetGateway(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
            return;
        }

        auto igw = outcome.GetResult().GetInternetGateway();

        QString igwID =
            QString::fromStdString(
                igw.GetInternetGatewayId()
            );

        // add name tag
        Aws::EC2::Model::CreateTagsRequest tagRequest;

        tagRequest.AddResources(
            igw.GetInternetGatewayId()
        );

        Aws::EC2::Model::Tag nameTag;

        nameTag.SetKey("Name");
        nameTag.SetValue(igwName.toStdString());

        tagRequest.AddTags(nameTag);

        ec2.CreateTags(tagRequest);

        // attach to vpc
        Aws::EC2::Model::AttachInternetGatewayRequest attachRequest;

        attachRequest.SetInternetGatewayId(
            igw.GetInternetGatewayId()
        );

        attachRequest.SetVpcId(
            vpcID.toStdString()
        );

        auto attachOutcome =
            ec2.AttachInternetGateway(attachRequest);

        if (!attachOutcome.IsSuccess()) {

            QString err = QString::fromStdString(attachOutcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                emit apiError(err);
            });
            return;
        }

        // emit success signal
        QMetaObject::invokeMethod(this, [this, igwID]() {
            emit igwCreated(igwID);
        }, Qt::QueuedConnection);
    });
}




















