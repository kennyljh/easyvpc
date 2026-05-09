#include "awsmanager.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QtConcurrent>
#include <vector>
#include <QStringList>
#include <QString>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/ec2/EC2Client.h>
#include <aws/ec2/model/CreateTagsRequest.h>
#include <aws/ec2/model/ModifyVpcAttributeRequest.h>
#include <aws/ec2/model/CreateSubnetRequest.h>
#include <aws/ec2/model/ModifySubnetAttributeRequest.h>
#include <aws/ec2/model/CreateInternetGatewayRequest.h>
#include <aws/ec2/model/AttachInternetGatewayRequest.h>
#include <aws/ec2/model/CreateRouteTableRequest.h>
#include <aws/ec2/model/CreateRouteRequest.h>
#include <aws/ec2/model/AssociateRouteTableRequest.h>
#include <aws/ec2/model/DisassociateRouteTableRequest.h>
#include <aws/ec2/model/DeleteRouteTableRequest.h>
#include <aws/ec2/model/DetachInternetGatewayRequest.h>
#include <aws/ec2/model/DeleteInternetGatewayRequest.h>
#include <aws/ec2/model/DeleteSubnetRequest.h>
#include <aws/ec2/model/DeleteVpcRequest.h>

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
                qDebug() << err;
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

    vpcIDCache.clear();
    qDebug() << "Clearing VPC cache";

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
                qDebug() << err;
                emit apiError(err);
            });
        }
        else {

            auto vpcs = outcome.GetResult().GetVpcs();
            qDebug() << "Found VPCs for region: " + region;

            for (const auto &vpc : vpcs){
                vpcDetails detail;
                vpcIDCache[QString::fromStdString(vpc.GetVpcId())] = detail;
            }

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
                qDebug() << err;
                emit apiError(err);
            });
        }
        else {

            auto subnets = outcome.GetResult().GetSubnets();

            vpcIDCache[id].subnets = subnets;

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
                qDegub() << err;
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
                qDebug() << err;
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
                qDebug() << err;
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
                qDebug() << err;
                emit apiError(err);
            });
        }
        else {

            auto RTs = outcome.GetResult().GetRouteTables();

            vpcIDCache[id].routeTables = RTs;

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
                qDebug() << err;
                emit apiError(err);
            });
        }
        else {

            auto IGWs = outcome.GetResult().GetInternetGateways();

            vpcIDCache[id].igw = IGWs;

            QMetaObject::invokeMethod(this, [this, id, IGWs]() {
                emit IGWByVPCIdReady(id, IGWs);
                emit notifyStatus("Found " + QString::number(IGWs.size()) + " IGW");
            });
        }
    });
}

void AWSManager::getZonesAsync(){

    QString region = selectedRegion;

    QtConcurrent::run([this, region]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DescribeAvailabilityZonesRequest request;

        auto outcome = ec2.DescribeAvailabilityZones(request);

        if (!outcome.IsSuccess()) {

            QString err = QString::fromStdString(outcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                qDebug() << err;
                emit apiError(err);
            });
        }
        else {

            auto zones = outcome.GetResult().GetAvailabilityZones();

            QMetaObject::invokeMethod(this, [this, zones]() {
                emit zonesReady(zones);
                qDebug() << "Found " + QString::number(zones.size()) + " zones";
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
                qDebug() << err;
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
                qDebug() << err;
                emit apiError(err);
            });
            return;
        }

        auto subnet = outcome.GetResult().GetSubnet();

        QString subnetID = QString::fromStdString(subnet.GetSubnetId());

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
                qDebug() << err;
                emit apiError(err);
            });
            return;
        }

        auto igw = outcome.GetResult().GetInternetGateway();

        QString igwID = QString::fromStdString(igw.GetInternetGatewayId());

        // add name tag
        Aws::EC2::Model::CreateTagsRequest tagRequest;

        tagRequest.AddResources(igw.GetInternetGatewayId());

        Aws::EC2::Model::Tag nameTag;

        nameTag.SetKey("Name");
        nameTag.SetValue(igwName.toStdString());

        tagRequest.AddTags(nameTag);

        ec2.CreateTags(tagRequest);

        // attach to vpc
        Aws::EC2::Model::AttachInternetGatewayRequest attachRequest;

        attachRequest.SetInternetGatewayId(igw.GetInternetGatewayId());

        attachRequest.SetVpcId(vpcID.toStdString());

        auto attachOutcome = ec2.AttachInternetGateway(attachRequest);

        if (!attachOutcome.IsSuccess()) {

            QString err = QString::fromStdString(attachOutcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                qDebug() << err;
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

void AWSManager::createRTAsync(QString vpcID,
                                QString RTName,
                                QString igwID,
                                QStringList subnetIDs){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    QtConcurrent::run([this, profile, region,
                        vpcID, RTName, igwID, subnetIDs]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        // create rt
        Aws::EC2::Model::CreateRouteTableRequest rtRequest;

        rtRequest.SetVpcId(vpcID.toStdString());

        auto rtOutcome = ec2.CreateRouteTable(rtRequest);

        if (!rtOutcome.IsSuccess()) {

            QString err = QString::fromStdString(rtOutcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                qDebug() << err;
                emit apiError(err);
            });
            return;
        }

        auto rt = rtOutcome.GetResult().GetRouteTable();

        QString rtID = QString::fromStdString(rt.GetRouteTableId());

        // add name tag
        Aws::EC2::Model::CreateTagsRequest tagRequest;

        tagRequest.AddResources(rt.GetRouteTableId());

        Aws::EC2::Model::Tag nameTag;

        nameTag.SetKey("Name");
        nameTag.SetValue(RTName.toStdString());

        tagRequest.AddTags(nameTag);

        ec2.CreateTags(tagRequest);

        // add default route
        Aws::EC2::Model::CreateRouteRequest routeRequest;

        routeRequest.SetRouteTableId(rt.GetRouteTableId());
        routeRequest.SetDestinationCidrBlock("0.0.0.0/0");
        routeRequest.SetGatewayId(igwID.toStdString());

        auto routeOutcome = ec2.CreateRoute(routeRequest);

        if (!routeOutcome.IsSuccess()) {

            QString err = QString::fromStdString(routeOutcome.GetError().GetMessage());

            QMetaObject::invokeMethod(this, [this, err]() {
                qDebug() << err;
                emit apiError(err);
            });
            return;
        }

        // associate subnets
        for (const QString &subnetID : subnetIDs) {

            Aws::EC2::Model::AssociateRouteTableRequest assocRequest;

            assocRequest.SetRouteTableId(rt.GetRouteTableId());
            assocRequest.SetSubnetId(subnetID.toStdString());

            auto assocOutcome = ec2.AssociateRouteTable(assocRequest);

            if (!assocOutcome.IsSuccess()) {

                QString err = QString::fromStdString(assocOutcome.GetError().GetMessage());

                QMetaObject::invokeMethod(this, [this, err]() {
                    qDebug() << err;
                    emit apiError(err);
                });
                return;
            }
        }

        // emit success signal
        QMetaObject::invokeMethod(this, [this, rtID]() {
            emit routeTableCreated(rtID);
        });
    });
}

void AWSManager::deleteRTsByVPCIdAsync(QString vpcID){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    auto temp = vpcIDCache[vpcID].routeTables;

    QtConcurrent::run([this, profile, region, vpcID]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        for (const auto &rt : vpcIDCache[vpcID].routeTables){

            auto rtId = rt.GetRouteTableId();

            // disassociate associations
            for (const auto &assoc : rt.GetAssociations()){

                Aws::EC2::Model::DisassociateRouteTableRequest disReq;
                disReq.SetAssociationId(assoc.GetRouteTableAssociationId());
                auto outcome = ec2.DisassociateRouteTable(disReq);

                if (!outcome.IsSuccess()){

                    QString err = QString::fromStdString(
                        qDebug() << err;
                        outcome.GetError().GetMessage()
                    );

                    QMetaObject::invokeMethod(this, [this, err]() {
                        emit apiError(err);
                        qDebug() << err;
                        return;
                    });
                }
            }

            // can't detele RT if main
            bool isMain = false;
            for (const auto &assoc : rt.GetAssociations()){

                if (assoc.GetMain()){

                    isMain = true;
                    break;
                }
            }
            if (isMain) continue;

            // delete RT
            Aws::EC2::Model::DeleteRouteTableRequest delReq;
            delReq.SetRouteTableId(rtId);
            auto outcome = ec2.DeleteRouteTable(delReq);

            if (!outcome.IsSuccess()){

                QString err = QString::fromStdString(
                    outcome.GetError().GetMessage()
                );

                QMetaObject::invokeMethod(this, [this, err]() {
                    qDebug() << err;
                    emit apiError(err);
                    return;
                });
            }

            qDebug() << "Deleted RT: " + rtId;
        }

        QMetaObject::invokeMethod(this, [this, vpcID]() {
            qDebug() << "RT deletion success";
            emit RTsDeletionCompleted(vpcID);
        });
    });
}

void AWSManager::deleteIGWByVPCIdAsync(QString vpcID){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    QtConcurrent::run([this, profile, region, vpcID]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        for (const auto &igw : vpcIDCache[vpcID].igw){

            auto igwId = igw.GetInternetGatewayId();

            // detach igw from vpc
            Aws::EC2::Model::DetachInternetGatewayRequest detachReq;
            detachReq.SetInternetGatewayId(igwId);
            detachReq.SetVpcId(vpcID.toStdString());

            auto detachOutcome = ec2.DetachInternetGateway(detachReq);

            if (!detachOutcome.IsSuccess()){

                QString err = QString::fromStdString(
                    detachOutcome.GetError().GetMessage()
                );

                QMetaObject::invokeMethod(this, [this, err]() {
                    qDebug() << err;
                    emit apiError(err);
                    return;
                });
            }

            // delete igw
            Aws::EC2::Model::DeleteInternetGatewayRequest delReq;
            delReq.SetInternetGatewayId(igwId);

            auto deleteOutcome = ec2.DeleteInternetGateway(delReq);

            if (!deleteOutcome.IsSuccess()){

                QString err = QString::fromStdString(
                    deleteOutcome.GetError().GetMessage()
                );

                QMetaObject::invokeMethod(this, [this, err]() {
                    qDebug() << err;
                    emit apiError(err);
                    return;
                });
            }
            qDebug() << "Deleted IGW: " + igwId;
        }

        QMetaObject::invokeMethod(this, [this, vpcID]() {
            emit IGWDeletionCompleted(vpcID);
        });
    });
}

void AWSManager::deleteSubnetsByVPCIdAsync(QString vpcID){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    QtConcurrent::run([this, profile, region, vpcID]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        for (const auto &subnet : vpcIDCache[vpcID].subnets){

            Aws::EC2::Model::DeleteSubnetRequest req;
            req.SetSubnetId(subnet.GetSubnetId());

            auto outcome = ec2.DeleteSubnet(req);

            if (!outcome.IsSuccess()){

                QString err = QString::fromStdString(
                    outcome.GetError().GetMessage()
                );

                QMetaObject::invokeMethod(this, [this, err]() {
                    qDebug() << err;
                    emit apiError(err);
                });
            }
            qDebug() << "Deleted subnet: " + subnet.GetSubnetId();
        }

        QMetaObject::invokeMethod(this, [this, vpcID]() {
            emit SubnetsDeletionCompleted(vpcID);
        });
    });
}

void AWSManager::deleteVPCByVPCIdAsync(QString vpcID){

    QString profile = selectedProfile;
    QString region = selectedRegion;

    QtConcurrent::run([this, profile, region, vpcID]() {

        Aws::Client::ClientConfiguration config;
        config.region = region.toStdString();
        config.profileName = profile.toStdString();

        Aws::EC2::EC2Client ec2(config);

        Aws::EC2::Model::DeleteVpcRequest req;
        req.SetVpcId(vpcID.toStdString());

        auto outcome = ec2.DeleteVpc(req);

        if (!outcome.IsSuccess()){

            QString err = QString::fromStdString(
                outcome.GetError().GetMessage()
            );

            QMetaObject::invokeMethod(this, [this, err]() {
                qDebug() << err;
                emit apiError(err);
            });
            return;
        }
        qDebug() << "Deleted VPC: " + vpcID;

        QMetaObject::invokeMethod(this, [this, vpcID]() {
            emit VPCDeletionCompleted(vpcID);
        });
    });
}



















