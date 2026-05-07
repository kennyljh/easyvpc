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

class VPCCreationDialog : public QDialog{
    Q_OBJECT
    public:
        explicit VPCCreationDialog(QWidget *parent = 0);

    private:
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
        QListWidget *subnetsLWidget;
        QPushButton *addRTBtn;

        QPushButton *createBtn;
        QPushButton *cancelBtn;
    signals:
        // todo - signal for sending all related info

};

#endif // VPCCREATIONDIALOG_H
