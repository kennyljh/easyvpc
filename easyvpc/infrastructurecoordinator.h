#ifndef INFRASTRUCTURECOORDINATOR_H
#define INFRASTRUCTURECOORDINATOR_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "vpccreationdialog.h"

/**
 * @brief The InfrastructureCoordinator class - coordinates infrastructure
 * provisioning and deletion related operations that require a series of
 * api calls
 */
class InfrastructureCoordinator : public QObject{
    Q_OBJECT
    public:
        explicit InfrastructureCoordinator(QObject *parent = nullptr);

    private:
        QString vpcName;
        QString vpcCIDR;
        QList<VPCCreationDialog::subnetInfo> subnetInfos;
        QString igwName;
        QList<VPCCreationDialog::RTInfo> RTInfos;
        QString vpcID;
        QMap<QString, QString> subnetNameToID;
        QString igwID;

        int subnetIndex;

        int rtIndex;

        /**
         * @brief createNextSubnet - coordinates next subnet
         * creation
         */
        void createNextSubnet();

        /**
         * @brief coordinateRTCreation - coordinates rt creation
         * and association
         */
        void coordinateRTCreation();

        /**
         * @brief createNextRT - coordinates next rt creation
         */
        void createNextRT();

    public slots:
        /**
         * @brief coordinateVPCCreation - coordinates provisioning of
         * VPC and associated subnets, IGW, and RTs
         * @param vpcName
         * @param vpcCIDR
         * @param subnetInfos
         * @param igwName
         * @param RTInfos
         */
        void coordinateVPCCreation(
            QString &vpcName,
            QString &vpcCIDR,
            const QList<VPCCreationDialog::subnetInfo> &subnetInfos,
            QString &igwName,
            const QList<VPCCreationDialog::RTInfo> &RTInfos
        );

        void coordinateSubnetsCreation(QString vpcID);

        /**
         * @brief onSubnetCreated - informs subnet creation
         * completion and begins next subnet creation
         * @param subnetID
         * @param subnetName
         */
        void onSubnetCreated(const QString &subnetID,
                                const QString &subnetName);

        /**
         * @brief onIGWCreated - informs igw creation
         * completion and begins rt coordination
         * @param igwID
         */
        void onIGWCreated(const QString &igwID);

        /**
         * @brief onRTCreated - informs rt creation completion
         * and begins next rt creation
         * @param rtID
         */
        void onRTCreated(const QString &rtID);

        /**
         * @brief coordinateVPCInfrastructureDeletion coordinates
         * deletion of VPC and associated subnets, IGW, and RTs
         * @param vpcID
         */
        void coordinateVPCInfrastructureDeletion(const QString &vpcID);

        /**
         * @brief coordinateIGWDeletion - coordinates igw disassociation
         * and deletion
         * @param vpcID
         */
        void coordinateIGWDeletion(const QString &vpcID);

        /**
         * @brief coordinateSubnetsDeletion - coordinates subnets
         * disassociation and deletion
         * @param vpcID
         */
        void coordinateSubnetsDeletion(const QString &vpcID);

        /**
         * @brief coordinateVPCDeletion - coodinates vpc
         * disassociation and deletion
         * @param vpcID
         */
        void coordinateVPCDeletion(const QString &vpcID);

        /**
         * @brief coordinateVPCDeletionCompleted - informs
         * vpc deletion completion
         * @param vpcID
         */
        void coordinateVPCDeletionCompleted(const QString &vpcID);

    signals:
        void vpcInfrastructureFinished(const QString &msg);

        void coordinatorError(const QString &err);

        void coordinatorStageChanged(const QString &msg, const int &val);

        void vpcInfrastructureDeleted(const QString &msg);
};

#endif // INFRASTRUCTURECOORDINATOR_H
