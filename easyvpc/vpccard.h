#ifndef VPCCARD_H
#define VPCCARD_H

#include <QFrame>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

class VPCCard : public QFrame{
    Q_OBJECT
    public:
        explicit VPCCard(const QString &name, const QString &id,
                            const QString &ipv4cidr, QWidget *parent = nullptr);
    private:
        QString *name;
        QString *id;
        QString *ipv4cidr;
        QStringList *subnets;
        QStringList *routeTables;
        QStringList *internetGateways;

        QWidget *vpcFrameWindow;
        QVBoxLayout *vpcFrameLayout;

        QWidget *vpcTitleWindow;
        QHBoxLayout *vpcTitleLayout;

        QPushButton *expandBtn;
        QPushButton *deleteBtn;
        QLabel *vpcName;
        QLabel *vpcID;
        QLabel *vpcIPv4CIDR;

        void expandCard();
    private slots:
        void expandTriggered();
};

#endif // VPCCARD_H
