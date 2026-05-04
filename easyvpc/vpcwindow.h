#ifndef VPCWINDOW_H
#define VPCWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>

class VPCWindow : public QMainWindow{
    Q_OBJECT
    public:
        explicit VPCWindow(QWidget *parent = 0);
    private:
        QDockWidget *dockWidget;
        QPushButton *backToHubBtn;
        QComboBox *regionsCBox;
        QWidget *topDWidgetWindow;
        QHBoxLayout *topDWidgetLayout;

        QWidget *centralWindow;
        QVBoxLayout *centralLayout;

        QScrollArea *myVPCScrollArea;
        QWidget *myVPCWindow;
        QVBoxLayout *myVPCLayout;


    private slots:
};

#endif // VPCWINDOW_H
