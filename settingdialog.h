/********************************************************************************
** Form generated from reading UI file 'settingdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *label_curePeriodSetting;
    QLabel *label_curePeriodIcon;
    QLabel *label_cureTempSetting;
    QLabel *label_cureTempIcon;
    QLabel *label_curePowerSetting;
    QLabel *label_cureTempIcon_2;
    QLabel *label_coldWatersetting;
    QLabel *label_coldWaterIcon;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(400, 300);
        label_curePeriodSetting = new QLabel(Dialog);
        label_curePeriodSetting->setObjectName(QStringLiteral("label_curePeriodSetting"));
        label_curePeriodSetting->setGeometry(QRect(0, 0, 67, 17));
        label_curePeriodIcon = new QLabel(Dialog);
        label_curePeriodIcon->setObjectName(QStringLiteral("label_curePeriodIcon"));
        label_curePeriodIcon->setGeometry(QRect(70, 0, 67, 17));
        label_cureTempSetting = new QLabel(Dialog);
        label_cureTempSetting->setObjectName(QStringLiteral("label_cureTempSetting"));
        label_cureTempSetting->setGeometry(QRect(140, 0, 67, 17));
        label_cureTempIcon = new QLabel(Dialog);
        label_cureTempIcon->setObjectName(QStringLiteral("label_cureTempIcon"));
        label_cureTempIcon->setGeometry(QRect(210, 0, 67, 17));
        label_curePowerSetting = new QLabel(Dialog);
        label_curePowerSetting->setObjectName(QStringLiteral("label_curePowerSetting"));
        label_curePowerSetting->setGeometry(QRect(0, 20, 67, 17));
        label_cureTempIcon_2 = new QLabel(Dialog);
        label_cureTempIcon_2->setObjectName(QStringLiteral("label_cureTempIcon_2"));
        label_cureTempIcon_2->setGeometry(QRect(70, 20, 67, 17));
        label_coldWatersetting = new QLabel(Dialog);
        label_coldWatersetting->setObjectName(QStringLiteral("label_coldWatersetting"));
        label_coldWatersetting->setGeometry(QRect(140, 20, 67, 17));
        label_coldWaterIcon = new QLabel(Dialog);
        label_coldWaterIcon->setObjectName(QStringLiteral("label_coldWaterIcon"));
        label_coldWaterIcon->setGeometry(QRect(210, 20, 67, 17));

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        label_curePeriodSetting->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_curePeriodIcon->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_cureTempSetting->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_cureTempIcon->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_curePowerSetting->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_cureTempIcon_2->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_coldWatersetting->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
        label_coldWaterIcon->setText(QApplication::translate("Dialog", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SETTINGDIALOG_H
