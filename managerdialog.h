/********************************************************************************
** Form generated from reading UI file 'managerdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MANAGERDIALOG_H
#define MANAGERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QtWidgets/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_ManagerDialog
{
public:
    QPushButton *pushButton_ts;
    QPushButton *pushButton_updata;
    QPushButton *pushButton_copy;
    QPushButton *pushButton_exit;
    QLabel *label;
    QProgressBar *updateProgressBar;

    void setupUi(QDialog *ManagerDialog)
    {
        if (ManagerDialog->objectName().isEmpty())
            ManagerDialog->setObjectName(QStringLiteral("ManagerDialog"));
        ManagerDialog->resize(400, 300);
        pushButton_ts = new QPushButton(ManagerDialog);
        pushButton_ts->setObjectName(QStringLiteral("pushButton_ts"));
        pushButton_ts->setGeometry(QRect(462, 50, 100, 27));
        pushButton_ts->setText(QString::fromUtf8("触摸屏校准"));

        pushButton_updata = new QPushButton(ManagerDialog);
        pushButton_updata->setObjectName(QStringLiteral("pushButton_updata"));
        pushButton_updata->setGeometry(QRect(462, 150, 100, 27));
        pushButton_updata->setText(QString::fromUtf8("更新固件"));

        pushButton_copy = new QPushButton(ManagerDialog);
        pushButton_copy->setObjectName(QStringLiteral("pushButton_copy"));
        pushButton_copy->setGeometry(QRect(462, 450, 100, 27));
        pushButton_copy->setText(QString::fromUtf8("拷贝数据"));

        pushButton_exit = new QPushButton(ManagerDialog);
        pushButton_exit->setObjectName(QStringLiteral("pushButton_exit"));
        pushButton_exit->setGeometry(QRect(462, 550, 100, 27));
        pushButton_exit->setText(QString::fromUtf8("退出"));

        label = new QLabel(ManagerDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(462, 230, 201, 21));

        updateProgressBar = new QProgressBar(ManagerDialog);
        updateProgressBar->move(440,200);
        updateProgressBar->resize(200,20);
        updateProgressBar->setOrientation(Qt::Horizontal);
//        updateProgressBar->setStyleSheet("QProgressBar:Horizontal{"
//                                        "border-radius:5px;"
//                                        "background-color:darkgray;"
//                                        "text-align:center"
//                                        "}"
//                                      "QProgressBar::chunk:horizontal{"
//                                        "background-color:#00FF00;"
//                                        "margin:1px;"
//                                        //"height:2px;"
//                                        "}");

        retranslateUi(ManagerDialog);

        QMetaObject::connectSlotsByName(ManagerDialog);
    } // setupUi

    void retranslateUi(QDialog *ManagerDialog)
    {
        ManagerDialog->setWindowTitle(QApplication::translate("ManagerDialog", "Dialog", 0));
//        pushButton_ts->setText(QApplication::translate("ManagerDialog", "ts_modify", 0));
//        pushButton_updata->setText(QApplication::translate("ManagerDialog", "updata", 0));
//        pushButton_copy->setText(QApplication::translate("ManagerDialog", "copy data", 0));
//        pushButton_exit->setText(QApplication::translate("ManagerDialog", "exit", 0));
//        label->setText(QApplication::translate("ManagerDialog", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class ManagerDialog: public Ui_ManagerDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MANAGERDIALOG_H
