/********************************************************************************
** Form generated from reading UI file 'systeminfo.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SYSTEMINFO_H
#define UI_SYSTEMINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label_systeminfo;
    QLabel *label_version;
    QLabel *label_versionvalue;
    QLabel *label__mcuVersion;
    QLabel *label_mcuVersionvalue;
    QPushButton *pushButton_sure;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(400, 300);
        label_systeminfo = new QLabel(Dialog);
        label_systeminfo->setObjectName(QStringLiteral("label_systeminfo"));
        label_systeminfo->setGeometry(QRect(120, 20, 91, 31));
        label_version = new QLabel(Dialog);
        label_version->setObjectName(QStringLiteral("label_version"));
        label_version->setGeometry(QRect(100, 80, 67, 17));
        label_versionvalue = new QLabel(Dialog);
        label_versionvalue->setObjectName(QStringLiteral("label_versionvalue"));
        label_versionvalue->setGeometry(QRect(170, 80, 67, 17));
        label__mcuVersion = new QLabel(Dialog);
        label__mcuVersion->setObjectName(QStringLiteral("label__mcuVersion"));
        label__mcuVersion->setGeometry(QRect(100, 140, 67, 17));
        label_mcuVersionvalue = new QLabel(Dialog);
        label_mcuVersionvalue->setObjectName(QStringLiteral("label_mcuVersionvalue"));
        label_mcuVersionvalue->setGeometry(QRect(170, 140, 67, 17));
        pushButton_sure = new QPushButton(Dialog);
        pushButton_sure->setObjectName(QStringLiteral("pushButton_sure"));
        pushButton_sure->setGeometry(QRect(250, 230, 99, 27));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        label_systeminfo->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_version->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_versionvalue->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label__mcuVersion->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_mcuVersionvalue->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        pushButton_sure->setText(QApplication::translate("Dialog", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SYSTEMINFO_H
