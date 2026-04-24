#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QStringList>
#include <QLabel>
#include <QHBoxLayout>

class LoginWindow : public QMainWindow{
    Q_OBJECT
    public:
        explicit LoginWindow(QWidget *parent  = 0);

    private:
        QStringList *foundProfiles;

        QWidget *centralWindow;
        QVBoxLayout *centralLayout;
        QPushButton *connectBtn;

        QWidget *selectProfileWindow;
        QVBoxLayout *selectProfileLayout;
        QLabel *profilesLineEdt;
        QWidget *CBoxBtnWindow;
        QHBoxLayout *CBoxBtnLayout;
        QComboBox *profilesCBox;
        QPushButton *goBtn;

        void getAWSProfiles(QStringList &profiles);

    private slots:
        void loginButtonClicked();
};

#endif // LOGINWINDOW_H
