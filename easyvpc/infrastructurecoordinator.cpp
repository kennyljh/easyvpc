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
    connect(&AWSManager::instance(), &AWSManager::igwCreated,
                this, &InfrastructureCoordinator::onIGWCreated);
    connect(&AWSManager::instance(), &AWSManager::routeTableCreated,
                this, &InfrastructureCoordinator::onRTCreated);
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
    emit coordinatorStageChanged("Starting VPC creation... %p%", 0);
    AWSManager::instance().createVPCAsync(vpc, CIDR);
}

void InfrastructureCoordinator::coordinateSubnetsCreation(QString vpcId){

    vpcID = vpcId;
    qDebug() << "VPC created: " + vpcID;
    emit coordinatorStageChanged("VPC created... %p%", 30);

    subnetIndex = 0;
    emit coordinatorStageChanged("Starting Subnet creation... %p%", 30);
    createNextSubnet();
}

void InfrastructureCoordinator::createNextSubnet(){

    if (subnetIndex >= subnetInfos.size()){
        qDebug() << "All subnets created";

        emit coordinatorStageChanged("All Subnets created... %p%", 70);
        emit coordinatorStageChanged("Creating Internet Gateway... %p%", 70);

        AWSManager::instance().createIGWAsync(vpcID, igwName);
        return;
    }

    auto subnet = subnetInfos[subnetIndex];

    emit coordinatorStageChanged("Creating Subnet " + subnet.name + "... %p%", 30);

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
    emit coordinatorStageChanged("Interet Gateway created... %p%", 85);

    coordinateRTCreation();
}

void InfrastructureCoordinator::coordinateRTCreation(){

    rtIndex = 0;
    emit coordinatorStageChanged("Creating Route Tables... %p%", 85);
    createNextRT();
}

void InfrastructureCoordinator::createNextRT(){

    if (rtIndex >= RTInfos.size()){

        qDebug() << "VPC infrastructure completed";
        emit vpcInfrastructureFinished("Created VPC " + vpcName + " " + vpcID);
        emit coordinatorStageChanged("VPC ready %p%", 100);
        return;
    }

    auto rt = RTInfos[rtIndex];

    QStringList subnetIDs;

    for (const auto &subnetName : rt.subnets){
        subnetIDs.append(subnetNameToID[subnetName]);
    }

    emit coordinatorStageChanged("Creating Route Table " + rt.name + "... %p%", 85);

    AWSManager::instance().createRTAsync(
        vpcID,
        rt.name,
        igwID,
        subnetIDs
    );
}

void InfrastructureCoordinator::onRTCreated(const QString &rtID){

    qDebug() << "Created RT: " + rtID;
    rtIndex++;
    createNextRT();
}