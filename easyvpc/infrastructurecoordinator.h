#ifndef INFRASTRUCTURECOORDINATOR_H
#define INFRASTRUCTURECOORDINATOR_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "vpccreationdialog.h"

/**
 * @brief The InfrastructureCoordinator class - coordinates infrastructure related
 * operations that require a series of api calls
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

        void createNextSubnet();

    public slots:
        void coordinateVPCCreation(
            QString &vpcName,
            QString &vpcCIDR,
            const QList<VPCCreationDialog::subnetInfo> &subnetInfos,
            QString &igwName,
            const QList<VPCCreationDialog::RTInfo> &RTInfos
        );

        void coordinateSubnetsCreation(QString vpcID);

        void onSubnetCreated(const QString &subnetID,
                                const QString &subnetName);

        void onIGWCreated(const QString &igwID);

    signals:
        void coordinatorError(QString err);
};

#endif // INFRASTRUCTURECOORDINATOR_H
