#ifndef SETUPINSTRUCTIONDIALOG_H
#define SETUPINSTRUCTIONDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

class SetupInstructionDialog : public QDialog{
    Q_OBJECT
    public:
        explicit SetupInstructionDialog(QWidget *parent = nullptr);

    private:
        QVBoxLayout *mainLayout;
        QScrollArea *scrollArea;

        QWidget *container;
        QVBoxLayout *containerLayout;

        QLabel *instructions;

        QPushButton *closeBtn;

};

#endif // SETUPINSTRUCTIONDIALOG_H
