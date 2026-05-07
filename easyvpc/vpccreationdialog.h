#ifndef VPCCREATIONDIALOG_H
#define VPCCREATIONDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <vector>
#include <QList>
#include <QStringList>

class VPCCreationDialog : public QDialog{
    Q_OBJECT
    public:
        explicit VPCCreationDialog(QWidget *parent = 0);

        struct subnetInfo{
            QString name;
            QString zone;
            QString ipv4;
        };

        struct RTInfo{
            QString name;
            QStringList subnets;
        };

    private:
        struct subnetBundle{
            QLineEdit *name;
            QComboBox *zone;
            QLineEdit *ipv4;
        };

        struct RTBundle{
            QLineEdit *name;
            QListWidget *subnets;
        };

        std::vector<subnetBundle> subnetBundles;
        std::vector<RTBundle> RTBundles;

        QList<subnetInfo> subnetInfos;
        QList<RTInfo> RTInfos;

        QVBoxLayout *mainLayout;
        QScrollArea *createVPCScrollArea;
        QFrame *createVPCFrame;
        QVBoxLayout *createVPCLayout;

        QLabel *vpcNameLabel;
        QLineEdit *vpcNameEdt;
        QLabel *vpcIPv4CIDRLabel;
        QLineEdit *vpcIPv4CIDREdt;

        QLabel *subnetsLabel;
        QFrame *subnetsFrame;
        QVBoxLayout *subnetsLayout;
        QLabel *subnetNameLabel;
        QLineEdit *subnetNameEdt;
        QLabel *AZsLabel;
        QComboBox *regionsCBox;
        QLabel *subnetIPv4CIDRLabel;
        QLineEdit *subnetIPv4CIDREdt;
        QPushButton *addSubnetBtn;

        QLabel *igwLabel;
        QLineEdit *igwEdt;

        QLabel *RTsLabel;
        QFrame *RTsFrame;
        QVBoxLayout *RTsLayout;
        QLabel *RTNameLabel;
        QLineEdit *RTNameEdt;
        QPushButton *refreshSubnetsBtn;
        QListWidget *subnetsLWidget;
        QPushButton *addRTBtn;

        QPushButton *createBtn;

        void createVPCRequest();

    private slots:
        void subnetsRefreshButtonClicked();

        // todo - slots for adding new subnets and rts

    signals:
        void VPCCreationRequested(
            QString &vpcName,
            QString &vpcCIDR,
            const QList<VPCCreationDialog::subnetInfo> &subnetInfos,
            QString &igwName,
            const QList<VPCCreationDialog::RTInfo> &RTInfos
        );
};

#endif // VPCCREATIONDIALOG_H
