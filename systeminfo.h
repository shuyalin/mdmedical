/********************************************************************************
** Form generated from reading UI file 'systeminfo.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QFont>

QT_BEGIN_NAMESPACE

class Ui_SystemInfo
{
public:
    QLabel *label_systeminfo;
    QLabel *label_version;
    QLabel *label_versionvalue;
    QLabel *label__mcuVersion;
    QLabel *label_mcuVersionvalue;
    QPushButton *pushButton_sure;
    QFont ft1,ft2;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(400, 300);
        label_systeminfo = new QLabel(Dialog);
        label_systeminfo->setObjectName(QStringLiteral("label_systeminfo"));
        label_systeminfo->setGeometry(QRect(400, 30, 200, 60));
        ft1.setPointSize(30);
        label_systeminfo->setFont(ft1);
        label_version = new QLabel(Dialog);
        label_version->setObjectName(QStringLiteral("label_version"));
        label_version->setGeometry(QRect(200, 180, 300, 40));

        label_versionvalue = new QLabel(Dialog);
        label_versionvalue->setObjectName(QStringLiteral("label_versionvalue"));
        label_versionvalue->setGeometry(QRect(510, 180, 100, 40));
        label__mcuVersion = new QLabel(Dialog);
        label__mcuVersion->setObjectName(QStringLiteral("label__mcuVersion"));
        label__mcuVersion->setGeometry(QRect(200, 300, 300, 40));
        label_mcuVersionvalue = new QLabel(Dialog);
        label_mcuVersionvalue->setObjectName(QStringLiteral("label_mcuVersionvalue"));
        label_mcuVersionvalue->setGeometry(QRect(510, 300, 100, 40));
        pushButton_sure = new QPushButton(Dialog);
        pushButton_sure->setObjectName(QStringLiteral("pushButton_sure"));
        pushButton_sure->setGeometry(QRect(600, 500, 80, 40));

        ft2.setPointSize(25);
        label_version->setFont(ft2);
        label_versionvalue->setFont(ft2);
        label__mcuVersion->setFont(ft2);
        label_mcuVersionvalue->setFont(ft2);
        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        label_systeminfo->setText(QString::fromUtf8("系统信息"));
        label_version->setText(QString::fromUtf8("人机系统软件版本号："));

        label_versionvalue->setText("1.0");

        label__mcuVersion->setText(QString::fromUtf8("M C U 软 件 版 本 号："));

        label_mcuVersionvalue->setText("1.0");

        pushButton_sure->setText(QString::fromUtf8("确认"));
    } // retranslateUi

};

namespace Ui {
    class SystemInfo: public Ui_SystemInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SYSTEMINFO_H
