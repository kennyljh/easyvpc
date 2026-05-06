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
        explicit LoginWindow(QWidget *parent = 0);

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

        /**
         * @brief getAWSProfiles - retrieve list of found local
         * profiles
         * @param profiles
         */
        void getAWSProfiles(QStringList &profiles);

        /**
         * @brief loginAWSProfile - login AWS with selected profile
         * @param selectedProfile
         */
        void loginAWSProfile(QString selectedProfile);

    private slots:
        void loginButtonClicked();
        void goButtonClicked();
};

#endif // LOGINWINDOW_H
