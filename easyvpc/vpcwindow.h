#ifndef VPCWINDOW_H
#define VPCWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>

class VPCWindow : public QMainWindow{
    Q_OBJECT
    public:
        explicit VPCWindow(QWidget *parent = 0);
    private:
        QWidget *centralWindow;
        QVBoxLayout *centralLayout;

        QPushButton *backToHubBtn;
        QComboBox *regionsCBox;
        QFrame *topBarFrame;
        QHBoxLayout *topBarLayout;

        QFrame *myVPCBarFrame;
        QHBoxLayout *myVPCBarLayout;
        QLabel *welcomeName;
        QPushButton *refreshBtn;
        QPushButton *sortBtn;
        QPushButton *expandAllBtn;

        QScrollArea *myVPCScrollArea;
        QWidget *myVPCWindow;
        QVBoxLayout *myVPCLayout;

        void processVPCs();
    private slots:
        void refreshButtonClicked();
};

#endif // VPCWINDOW_H
