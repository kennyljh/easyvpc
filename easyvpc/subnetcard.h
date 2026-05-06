#ifndef SUBNETCARD_H
#define SUBNETCARD_H

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class SubnetCard : public QFrame{
    Q_OBJECT
    public:
        explicit SubnetCard(const QString &name, const QString &id,
                                const QString &ipv4cidr, const QString &ipAddrCount,
                                const QString &zoneID, const QString &zone,
                                const QString &state, QWidget *parent = nullptr);
    private:
        QVBoxLayout *mainLayout;

        QFrame *subnetTopFrame;
        QHBoxLayout *subnetTopLayout;
        QLabel *subnetNameLabel;
        QPushButton *deleteSubnetBtn;

        QFrame *subnetMiscFrame;
        QHBoxLayout *subnetMiscLayout;
        QFrame *subnetDetailsFrame;
        QVBoxLayout *subnetDetailsLayout;
        QLabel *subnetDetailsLabel;
        QLabel *subnetIDLabel;
        QLabel *subnetIPv4CIDRLabel;
        QLabel *subnetIPAddrCountLabel;
        QLabel *subnetZoneLabel;
        QLabel *subnetStateLabel;

        QFrame *subnetEC2sFrame;
        QFrame *subnetRTsFrame;

};

#endif // SUBNETCARD_H
