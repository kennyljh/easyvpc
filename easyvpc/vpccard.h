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
                            const QString &ipv4cidr, const QString &state,
                            QWidget *parent = nullptr);
    private:
        QVBoxLayout *vpcFrameLayout;

        QFrame *vpcTitleFrame;
        QHBoxLayout *vpcTitleLayout;

        QPushButton *expandBtn;
        QPushButton *deleteBtn;
        QLabel *vpcName;

        QFrame *vpcDetailsFrame;
        QHBoxLayout *vpcDetailsLayout;
        QLabel *vpcIDLabel;
        QLabel *vpcIPv4CIDRLabel;
        QLabel *vpcStateLabel;

        QFrame *subnetsMainFrame;
        QPushButton *addSubnetBtn;

        QFrame *routeTableMainFrame;
        QFrame *igwMainFrame;
        QFrame *natGatewayMainFrame;
        QFrame *secGroupMainFrame;
        QFrame *aclsMainFrame;

        // void expandCard();
    private slots:
        // void expandTriggered();
        // void deleteButtonClicked();
};

#endif // VPCCARD_H
