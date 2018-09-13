/********************************************************************************
** Form generated from reading UI file 'managerdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGERDIALOG_H
#define UI_MANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ManagerDialog
{
public:
    QPushButton *pushButton_ts;
    QPushButton *pushButton_updata;
    QPushButton *pushButton_copy;
    QPushButton *pushButton_exit;
    QLabel *label;

    void setupUi(QDialog *ManagerDialog)
    {
        if (ManagerDialog->objectName().isEmpty())
            ManagerDialog->setObjectName(QStringLiteral("ManagerDialog"));
        ManagerDialog->resize(400, 300);
        pushButton_ts = new QPushButton(ManagerDialog);
        pushButton_ts->setObjectName(QStringLiteral("pushButton_ts"));
        pushButton_ts->setGeometry(QRect(40, 50, 99, 27));
        pushButton_updata = new QPushButton(ManagerDialog);
        pushButton_updata->setObjectName(QStringLiteral("pushButton_updata"));
        pushButton_updata->setGeometry(QRect(40, 120, 99, 27));
        pushButton_copy = new QPushButton(ManagerDialog);
        pushButton_copy->setObjectName(QStringLiteral("pushButton_copy"));
        pushButton_copy->setGeometry(QRect(40, 200, 99, 27));
        pushButton_exit = new QPushButton(ManagerDialog);
        pushButton_exit->setObjectName(QStringLiteral("pushButton_exit"));
        pushButton_exit->setGeometry(QRect(240, 250, 99, 27));
        label = new QLabel(ManagerDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(180, 120, 201, 21));

        retranslateUi(ManagerDialog);

        QMetaObject::connectSlotsByName(ManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *ManagerDialog)
    {
        ManagerDialog->setWindowTitle(QApplication::translate("ManagerDialog", "Dialog", Q_NULLPTR));
        pushButton_ts->setText(QApplication::translate("ManagerDialog", "ts_modify", Q_NULLPTR));
        pushButton_updata->setText(QApplication::translate("ManagerDialog", "updata", Q_NULLPTR));
        pushButton_copy->setText(QApplication::translate("ManagerDialog", "copy data", Q_NULLPTR));
        pushButton_exit->setText(QApplication::translate("ManagerDialog", "exit", Q_NULLPTR));
        label->setText(QApplication::translate("ManagerDialog", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ManagerDialog: public Ui_ManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGERDIALOG_H
