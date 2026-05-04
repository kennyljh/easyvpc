#include "vpcwindow.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>

VPCWindow::VPCWindow(QWidget *parent) : QMainWindow(parent){

    resize(1280, 720);

    dockWidget = new QDockWidget(this);
    dockWidget->setAllowedAreas(Qt::TopDockWidgetArea);
    dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);

        topDWidgetWindow = new QWidget(dockWidget);
        topDWidgetLayout = new QHBoxLayout(topDWidgetWindow);
            backToHubBtn = new QPushButton("EasyVPC", topDWidgetWindow);
            regionsCBox = new QComboBox(topDWidgetWindow);
        topDWidgetLayout->addWidget(backToHubBtn, 0, Qt::AlignLeft);
        topDWidgetLayout->addWidget(regionsCBox, 0, Qt::AlignRight);

    dockWidget->setWidget(topDWidgetWindow);
    addDockWidget(Qt::TopDockWidgetArea, dockWidget);
}