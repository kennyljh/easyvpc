#ifndef VPCWINDOW_H
#define VPCWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QStatusBar>
#include <QMap>
#include <QProgressBar>
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>
#include "vpccreationdialog.h"

class VPCWindow : public QMainWindow{
    Q_OBJECT
    public:
        explicit VPCWindow(QWidget *parent = 0);

    private:
        QWidget *centralWindow;
        QVBoxLayout *centralLayout;

        QLabel *easyVPCLabel;
        QProgressBar *taskProgressBar;

        QComboBox *regionsCBox;
        QFrame *topBarFrame;
        QHBoxLayout *topBarLayout;

        QFrame *myVPCBarFrame;
        QHBoxLayout *myVPCBarLayout;
        QLabel *welcomeName;
        QPushButton *refreshBtn;
        QPushButton *createBtn;

        QScrollArea *myVPCScrollArea;
        QWidget *myVPCWindow;
        QVBoxLayout *myVPCLayout;

    private slots:
        void processRegions(const std::vector<Aws::EC2::Model::Region> &regions);

        /**
         * @brief processVPCs - inserts vpcs into appropriate widgets, cache
         * vpcs based on id
         * @param vpcs
         */
        void processVPCs(const std::vector<Aws::EC2::Model::Vpc> &vpcs);

        /**
         * @brief setStatusBar - sets status bar message
         * @param msg
         */
        void setStatusBar(QString msg);

        void refreshButtonClicked();

        /**
         * @brief createVPCButtonClicked - initiates vpc creation dialog
         * popup and infrastructure coordinator setup
         */
        void createVPCButtonClicked();

        /**
         * @brief regionChangeTriggered - reconfigure current region and
         * refresh vpcs
         * @param region
         */
        void regionChangeTriggered(const QString &region);

        /**
         * @brief coordinatorUpdated - update progress bar based
         * on coordinator stage change
         * @param msg
         * @param val
         */
        void coordinatorUpdated(const QString &msg, const int &val);

        /**
         * @brief VPCCreationDataDebug - to debug vpc creation request
         * information retrieved from dialog
         * @param vpcName
         * @param vpcCIDR
         * @param subnetInfos
         * @param igwName
         * @param RTInfos
         */
        void VPCCreationDataDebug(
            QString &vpcName,
            QString &vpcCIDR,
            const QList<VPCCreationDialog::subnetInfo> &subnetInfos,
            QString &igwName,
            const QList<VPCCreationDialog::RTInfo> &RTInfos
        );
};

#endif // VPCWINDOW_H
