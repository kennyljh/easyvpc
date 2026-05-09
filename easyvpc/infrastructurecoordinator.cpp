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

    connect(&AWSManager::instance(), &AWSManager::RTsDeletionCompleted,
                this, &InfrastructureCoordinator::coordinateIGWDeletion);
    connect(&AWSManager::instance(), &AWSManager::IGWDeletionCompleted,
                this, &InfrastructureCoordinator::coordinateSubnetsDeletion);
    connect(&AWSManager::instance(), &AWSManager::SubnetsDeletionCompleted,
                this, &InfrastructureCoordinator::coordinateVPCDeletion);
    connect(&AWSManager::instance(), &AWSManager::VPCDeletionCompleted,
                this, &InfrastructureCoordinator::coordinateVPCDeletionCompleted);
}

void InfrastructureCoordinator::coordinateVPCCreation(QString &vpc,
                                                        QString &CIDR,
                                                        const QList<VPCCreationDialog::subnetInfo> &subnets,
                                                        QString &igw,
                                                        const QList<VPCCreationDialog::RTInfo> &RTs){

    vpcName = vpc;
    vpcCIDR = CIDR;
    for (const auto &subnet : subnets) subnetInfos.append(subnet);

    igwName = igw;
    for (const auto &rt : RTs) RTInfos.append(rt);

    QMetaObject::invokeMethod(this, [this]() {
        emit coordinatorStageChanged("Starting VPC creation... %p%", 0);
    });

    AWSManager::instance().createVPCAsync(vpc, CIDR);
}

void InfrastructureCoordinator::coordinateSubnetsCreation(QString vpcId){

    vpcID = vpcId;
    qDebug() << "VPC created: " + vpcID;
    QMetaObject::invokeMethod(this, [this]() {
        emit coordinatorStageChanged("VPC created... %p%", 30);
    });

    subnetIndex = 0;
    QMetaObject::invokeMethod(this, [this]() {
        emit coordinatorStageChanged("Starting Subnet creation... %p%", 30);
    });
    createNextSubnet();
}

void InfrastructureCoordinator::createNextSubnet(){

    if (subnetIndex >= subnetInfos.size()){
        qDebug() << "All subnets created";

        QMetaObject::invokeMethod(this, [this]() {
            emit coordinatorStageChanged("All Subnets created... %p%", 70);
            emit coordinatorStageChanged("Creating Internet Gateway... %p%", 70);
        });

        AWSManager::instance().createIGWAsync(vpcID, igwName);
        return;
    }

    auto subnet = subnetInfos[subnetIndex];
    int index = subnetIndex;
    int size = subnetInfos.size();

    QMetaObject::invokeMethod(this, [this, subnet, index, size]() {
        emit coordinatorStageChanged("Creating Subnet " + subnet.name + "... %p%", (30 + (index*(40/size))));
    });

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

    int index = subnetIndex;
    int size = subnetInfos.size();
    QMetaObject::invokeMethod(this, [this, subnetName, index, size]() {
        emit coordinatorStageChanged("Created Subnet " + subnetName + " %p%", (30 + (index*(40/size))));
    });

    createNextSubnet();
}

void InfrastructureCoordinator::onIGWCreated(const QString &igwId){

    igwID = igwId;
    qDebug() << "IGW created: " + igwID;
    QMetaObject::invokeMethod(this, [this]() {
        emit coordinatorStageChanged("Interet Gateway created... %p%", 85);
    });

    coordinateRTCreation();
}

void InfrastructureCoordinator::coordinateRTCreation(){

    rtIndex = 0;
    QMetaObject::invokeMethod(this, [this]() {
        emit coordinatorStageChanged("Creating Route Tables... %p%", 85);
    });
    createNextRT();
}

void InfrastructureCoordinator::createNextRT(){

    if (rtIndex >= RTInfos.size()){

        qDebug() << "VPC infrastructure completed";
        QMetaObject::invokeMethod(this, [this]() {
            emit vpcInfrastructureFinished("Created VPC " + vpcName + " " + vpcID);
            emit coordinatorStageChanged("VPC " + vpcName + " is ready %p%", 100);
        });
        return;
    }

    auto rt = RTInfos[rtIndex];

    QStringList subnetIDs;

    for (const auto &subnetName : rt.subnets) subnetIDs.append(subnetNameToID[subnetName]);

    int index = rtIndex;
    int size = RTInfos.size();
    QMetaObject::invokeMethod(this, [this, rt, index, size]() {
        emit coordinatorStageChanged("Creating Route Table " + rt.name + "... %p%", (85 + (index*(15/size))));
    });

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

    int index = rtIndex;
    int size = RTInfos.size();
    QMetaObject::invokeMethod(this, [this, rtID, index, size]() {
        emit coordinatorStageChanged("Created Route Table " + rtID + " %p%", (85 + (index*(15/size))));
    });

    createNextRT();
}

void InfrastructureCoordinator::coordinateVPCInfrastructureDeletion(const QString &vpcID){

    if (!AWSManager::instance().vpcIDCache.contains(vpcID)){
        qDebug() << "Cannot deleting non-existing vpc: " + vpcID;
        return;
    }

    QMetaObject::invokeMethod(this, [this, vpcID]() {
        qDebug() << "Coordinating VPC deletion for: " + vpcID;
        emit coordinatorStageChanged("Starting VPC " + vpcID + " deletion... %p%", 0);
    });

    AWSManager::instance().deleteRTsByVPCIdAsync(vpcID);
}

void InfrastructureCoordinator::coordinateIGWDeletion(const QString &vpcID){

    QMetaObject::invokeMethod(this, [this]() {
        emit coordinatorStageChanged("Route tables deleted. Starting Internet Gateway deletion... %p%", 30);
    });
    AWSManager::instance().deleteIGWByVPCIdAsync(vpcID);
}

void InfrastructureCoordinator::coordinateSubnetsDeletion(const QString &vpcID){

    QMetaObject::invokeMethod(this, [this]() {
        emit coordinatorStageChanged("Internet Gateway deleted. Starting Subnets deletion... %p%", 60);
    });

    AWSManager::instance().deleteSubnetsByVPCIdAsync(vpcID);
}

void InfrastructureCoordinator::coordinateVPCDeletion(const QString &vpcID){

    QMetaObject::invokeMethod(this, [this]() {
        emit coordinatorStageChanged("Subnets deleted. Starting VPC deletion... %p%", 90);
    });

    AWSManager::instance().deleteVPCByVPCIdAsync(vpcID);
}

void InfrastructureCoordinator::coordinateVPCDeletionCompleted(const QString &vpcID){

    QMetaObject::invokeMethod(this, [this, vpcID]() {
        emit coordinatorStageChanged("VPC " + vpcID + " successfully deleted %p%", 100);
        emit vpcInfrastructureDeleted("VPC " + vpcID + " successfully deleted");
    });
}
