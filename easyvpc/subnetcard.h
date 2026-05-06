#ifndef SUBNETCARD_H
#define SUBNETCARD_H

#include <QFrame>
#include <QWidget>

class SubnetCard : public QFrame{
    Q_OBJECT
    public:
        explicit SubnetCard(const QString &name, const QString &id,
                                const QString &ipv4cidr, const QString &ipAddrCount,
                                const QString &zoneID, const QString &zone,
                                const QString &state, QWidget *parent = nullptr);
    private:

};

#endif // SUBNETCARD_H
