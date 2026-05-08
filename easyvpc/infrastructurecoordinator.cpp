#include "infrastructurecoordinator.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QDebug>
#include "awsmanager.h"

InfrastructureCoordinator::InfrastructureCoordinator(QObject *parent)
                          : QObject(parent) {

    connect(&AWSManager::instance(), &AWSManager::vpcCreated,
                this, &InfrastructureCoordinator::coordinateSubnetsCreation);
    connect(&AWSManager::instance(), &AWSManager::subnetCreated,
                this, &InfrastructureCoordinator::onSubnetCreated);
}

void InfrastructureCoordinator::coordinateVPCCreation(
    QString &vpc,
    QString &CIDR,
    const QList<VPCCreationDialog::subnetInfo> &subnets,
    QString &igw,
    const QList<VPCCreationDialog::RTInfo> &RTs){

    vpcName = vpc;
    vpcCIDR = CIDR;
    for (const auto &subnet : subnets){
        subnetInfos.append(subnet);
    }
    igwName = igw;
    for (const auto &rt : RTs){
        RTInfos.append(rt);
    }

    AWSManager::instance().createVPCAsync(vpc, CIDR);
}

void InfrastructureCoordinator::coordinateSubnetsCreation(QString vpcId){

    vpcID = vpcId;
    qDebug() << "VPC created: " + vpcID;
    subnetIndex = 0;
    createNextSubnet();
}

void InfrastructureCoordinator::createNextSubnet(){

    if (subnetIndex >= subnetInfos.size()){
        qDebug() << "All subnets created";

        AWSManager::instance().createIGWAsync(vpcID, igwName);
        return;
    }

    auto subnet = subnetInfos[subnetIndex];

    AWSManager::instance().createSubnetAsync(
        vpcID,
        subnet.name,
        subnet.zone,
        subnet.ipv4
    );
}

void InfrastructureCoordinator::onSubnetCreated(const QString &subnetID,
                                const QString &subnetName){

    qDebug() << "Subnet created: " + subnetID + " " + subnetName;
    subnetNameToID[subnetName] = subnetID;

    subnetIndex++;
    createNextSubnet();
}

void InfrastructureCoordinator::onIGWCreated(const QString &igwId){

    igwID = igwId;
    qDebug() << "IGW created: " + igwID;

    // coordinate next

}














