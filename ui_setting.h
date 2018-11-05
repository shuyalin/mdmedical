/********************************************************************************
** Form generated from reading UI file 'setting.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTING_H
#define UI_SETTING_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QLabel *label_curePeriodSetting;
    QLabel *label_coldLiquidStreamIcon;
    QLabel *label_coldLiquidStreamSetting;
    QLabel *label_curePowerIcon;
    QLabel *label_curePowerSetting;
    QLabel *label_cureTempIcon;
    QLabel *label_cureTempSetting;
    QLabel *label_curePeriodIcon;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(512, 362);
        label_curePeriodSetting = new QLabel(Form);
        label_curePeriodSetting->setObjectName(QStringLiteral("label_curePeriodSetting"));
        label_curePeriodSetting->setGeometry(QRect(0, 0, 41, 16));
        label_coldLiquidStreamIcon = new QLabel(Form);
        label_coldLiquidStreamIcon->setObjectName(QStringLiteral("label_coldLiquidStreamIcon"));
        label_coldLiquidStreamIcon->setGeometry(QRect(340, 0, 41, 16));
        label_coldLiquidStreamSetting = new QLabel(Form);
        label_coldLiquidStreamSetting->setObjectName(QStringLiteral("label_coldLiquidStreamSetting"));
        label_coldLiquidStreamSetting->setGeometry(QRect(290, 0, 41, 16));
        label_curePowerIcon = new QLabel(Form);
        label_curePowerIcon->setObjectName(QStringLiteral("label_curePowerIcon"));
        label_curePowerIcon->setGeometry(QRect(240, 0, 41, 16));
        label_curePowerSetting = new QLabel(Form);
        label_curePowerSetting->setObjectName(QStringLiteral("label_curePowerSetting"));
        label_curePowerSetting->setGeometry(QRect(190, 0, 41, 16));
        label_cureTempIcon = new QLabel(Form);
        label_cureTempIcon->setObjectName(QStringLiteral("label_cureTempIcon"));
        label_cureTempIcon->setGeometry(QRect(140, 0, 41, 16));
        label_cureTempSetting = new QLabel(Form);
        label_cureTempSetting->setObjectName(QStringLiteral("label_cureTempSetting"));
        label_cureTempSetting->setGeometry(QRect(90, 0, 41, 16));
        label_curePeriodIcon = new QLabel(Form);
        label_curePeriodIcon->setObjectName(QStringLiteral("label_curePeriodIcon"));
        label_curePeriodIcon->setGeometry(QRect(50, 0, 31, 16));

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", Q_NULLPTR));
        label_curePeriodSetting->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_coldLiquidStreamIcon->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_coldLiquidStreamSetting->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_curePowerIcon->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_curePowerSetting->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_cureTempIcon->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_cureTempSetting->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
        label_curePeriodIcon->setText(QApplication::translate("Form", "TextLabel", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTING_H
