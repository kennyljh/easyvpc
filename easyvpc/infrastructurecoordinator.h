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
        QString vpcID;
        QMap<QString, QString> subnetNameToID;
        QString igwID;

    public slots:
        void coordinateVPCCreation(
            QString &vpcName,
            QString &vpcCIDR,
            const QList<VPCCreationDialog::subnetInfo> &subnetInfos,
            QString &igwName,
            const QList<VPCCreationDialog::RTInfo> &RTInfos
        );

    signals:
        void vpcCreationDone();
        void subnetCreationDone();
        void igwCreationDone();
        void rtCreationDone();
        void coordinatorError(QString err);
};

#endif // INFRASTRUCTURECOORDINATOR_H
