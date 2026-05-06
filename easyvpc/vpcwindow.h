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
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>

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

        /**
         * @brief processVPCs - inserts vpcs into appropriate widgets
         * @param vpcs
         */
        void processVPCs(const std::vector<Aws::EC2::Model::Vpc> &vpcs);

        /**
         * @brief setStatusBar - sets status bar message
         * @param msg
         */
        void setStatusBar(QString msg);

    private slots:
        void refreshButtonClicked();
};

#endif // VPCWINDOW_H
