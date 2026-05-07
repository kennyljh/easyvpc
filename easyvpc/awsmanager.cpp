#include "awsmanager.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QtConcurrent>
#include <vector>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/ec2/EC2Client.h>

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