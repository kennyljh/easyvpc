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
#include <QFrame>

class LoginWindow : public QMainWindow{
    Q_OBJECT
    public:
        explicit LoginWindow(QWidget *parent = 0);

    private:
        QStringList *foundProfiles;

        QWidget *centralWindow;
        QVBoxLayout *centralLayout;

        QFrame *topBarFrame;
        QHBoxLayout *topBarLayout;
        QPushButton *helpBtn;

        QFrame *connectFrame;
        QHBoxLayout *connectLayout;
        QPushButton *connectBtn;

        QWidget *selectProfileWindow;
        QVBoxLayout *selectProfileLayout;
        QLabel *profilesLabel;
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
        void helpButtonClicked();

    signals:
        void loginIsSuccessful();
};

#endif // LOGINWINDOW_H
