/********************************************************************************
** Form generated from reading UI file 'mdmedical.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDMEDICAL_H
#define UI_MDMEDICAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mdmedical
{
public:
    QLabel *label_Max;
    QLabel *label_devidestatus;
    QLabel *label_gasvalue;
    QLabel *label_impedance1;
    QLabel *label_impedance2;
    QLabel *label_impedance4;
    QLabel *label_impedance3;
    QLabel *label_showcurtime;
    QLabel *label_tagetTmp;
    QLabel *label_tmp1;
    QLabel *label_tmp2;
    QLabel *label_tmp3;
    QLabel *label_tmp4;
    QLabel *label_tmp5;
    QLabel *label_tmp6;
    QLabel *label_tmp7;
    QLabel *label_tmp8;
    QLabel *label_zlzq;
    QPushButton *pushButton_addgas;
    QPushButton *pushButton_losegas;
    QPushButton *pushButton_td1;
    QPushButton *pushButton_td3;
    QPushButton *pushButton_td2;
    QPushButton *pushButton_td4;
    QPushButton *pushButton_waitmode;
    QPushButton *pushButton_calibrate;
    QLabel *label_showcam;
    QPushButton *pushButton_showcam;
    QPushButton *pushButton_takephoto;
    QPushButton *pushButton_closecam;
    QPushButton *pushButton_setting;
    QLabel *label_zlzqIcon;
    QLabel *label_tagetIcon;
    QLabel *label_MaxIcon;
    QLabel *label_coldwaterstream;
    QLabel *label_coldwaterstreamIcon;
    QGroupBox *groupBox_zlzq;
    QGroupBox *groupBox_tagetTmp;
    QGroupBox *groupBox_Max;
    QGroupBox *groupBox_coldwaterstream;
    QGroupBox *groupBox_channelselect;
    QPushButton *pushButton_systeminfo;

    void setupUi(QWidget *mdmedical)
    {
        if (mdmedical->objectName().isEmpty())
            mdmedical->setObjectName(QStringLiteral("mdmedical"));
        mdmedical->resize(400, 300);
        label_Max = new QLabel(mdmedical);
        label_Max->setObjectName(QStringLiteral("label_Max"));
        label_Max->setGeometry(QRect(0, 0, 67, 17));
        label_devidestatus = new QLabel(mdmedical);
        label_devidestatus->setObjectName(QStringLiteral("label_devidestatus"));
        label_devidestatus->setGeometry(QRect(70, 0, 67, 17));
        label_gasvalue = new QLabel(mdmedical);
        label_gasvalue->setObjectName(QStringLiteral("label_gasvalue"));
        label_gasvalue->setGeometry(QRect(140, 0, 67, 17));
        label_impedance1 = new QLabel(mdmedical);
        label_impedance1->setObjectName(QStringLiteral("label_impedance1"));
        label_impedance1->setGeometry(QRect(210, 0, 67, 17));
        label_impedance2 = new QLabel(mdmedical);
        label_impedance2->setObjectName(QStringLiteral("label_impedance2"));
        label_impedance2->setGeometry(QRect(280, 0, 67, 17));
        label_impedance4 = new QLabel(mdmedical);
        label_impedance4->setObjectName(QStringLiteral("label_impedance4"));
        label_impedance4->setGeometry(QRect(0, 20, 67, 17));
        label_impedance3 = new QLabel(mdmedical);
        label_impedance3->setObjectName(QStringLiteral("label_impedance3"));
        label_impedance3->setGeometry(QRect(70, 20, 67, 17));
        label_showcurtime = new QLabel(mdmedical);
        label_showcurtime->setObjectName(QStringLiteral("label_showcurtime"));
        label_showcurtime->setGeometry(QRect(140, 20, 67, 17));
        label_tagetTmp = new QLabel(mdmedical);
        label_tagetTmp->setObjectName(QStringLiteral("label_tagetTmp"));
        label_tagetTmp->setGeometry(QRect(210, 20, 67, 17));
        label_tmp1 = new QLabel(mdmedical);
        label_tmp1->setObjectName(QStringLiteral("label_tmp1"));
        label_tmp1->setGeometry(QRect(0, 40, 67, 17));
        label_tmp2 = new QLabel(mdmedical);
        label_tmp2->setObjectName(QStringLiteral("label_tmp2"));
        label_tmp2->setGeometry(QRect(70, 40, 67, 17));
        label_tmp3 = new QLabel(mdmedical);
        label_tmp3->setObjectName(QStringLiteral("label_tmp3"));
        label_tmp3->setGeometry(QRect(140, 40, 67, 17));
        label_tmp4 = new QLabel(mdmedical);
        label_tmp4->setObjectName(QStringLiteral("label_tmp4"));
        label_tmp4->setGeometry(QRect(210, 40, 67, 17));
        label_tmp5 = new QLabel(mdmedical);
        label_tmp5->setObjectName(QStringLiteral("label_tmp5"));
        label_tmp5->setGeometry(QRect(280, 40, 67, 17));
        label_tmp6 = new QLabel(mdmedical);
        label_tmp6->setObjectName(QStringLiteral("label_tmp6"));
        label_tmp6->setGeometry(QRect(0, 60, 67, 17));
        label_tmp7 = new QLabel(mdmedical);
        label_tmp7->setObjectName(QStringLiteral("label_tmp7"));
        label_tmp7->setGeometry(QRect(140, 60, 67, 17));
        label_tmp8 = new QLabel(mdmedical);
        label_tmp8->setObjectName(QStringLiteral("label_tmp8"));
        label_tmp8->setGeometry(QRect(210, 60, 67, 17));
        label_zlzq = new QLabel(mdmedical);
        label_zlzq->setObjectName(QStringLiteral("label_zlzq"));
        label_zlzq->setGeometry(QRect(70, 60, 67, 17));
        pushButton_addgas = new QPushButton(mdmedical);
        pushButton_addgas->setObjectName(QStringLiteral("pushButton_addgas"));
        pushButton_addgas->setGeometry(QRect(0, 90, 51, 27));
        pushButton_losegas = new QPushButton(mdmedical);
        pushButton_losegas->setObjectName(QStringLiteral("pushButton_losegas"));
        pushButton_losegas->setGeometry(QRect(100, 90, 61, 27));
        pushButton_td1 = new QPushButton(mdmedical);
        pushButton_td1->setObjectName(QStringLiteral("pushButton_td1"));
        pushButton_td1->setGeometry(QRect(160, 90, 51, 27));
        pushButton_td3 = new QPushButton(mdmedical);
        pushButton_td3->setObjectName(QStringLiteral("pushButton_td3"));
        pushButton_td3->setGeometry(QRect(210, 90, 51, 27));
        pushButton_td2 = new QPushButton(mdmedical);
        pushButton_td2->setObjectName(QStringLiteral("pushButton_td2"));
        pushButton_td2->setGeometry(QRect(260, 90, 51, 27));
        pushButton_td4 = new QPushButton(mdmedical);
        pushButton_td4->setObjectName(QStringLiteral("pushButton_td4"));
        pushButton_td4->setGeometry(QRect(310, 90, 61, 27));
        pushButton_waitmode = new QPushButton(mdmedical);
        pushButton_waitmode->setObjectName(QStringLiteral("pushButton_waitmode"));
        pushButton_waitmode->setGeometry(QRect(280, 130, 41, 21));
        pushButton_calibrate = new QPushButton(mdmedical);
        pushButton_calibrate->setObjectName(QStringLiteral("pushButton_calibrate"));
        pushButton_calibrate->setGeometry(QRect(0, 120, 41, 16));
        label_showcam = new QLabel(mdmedical);
        label_showcam->setObjectName(QStringLiteral("label_showcam"));
        label_showcam->setGeometry(QRect(0, 140, 41, 16));
        pushButton_showcam = new QPushButton(mdmedical);
        pushButton_showcam->setObjectName(QStringLiteral("pushButton_showcam"));
        pushButton_showcam->setGeometry(QRect(40, 140, 51, 21));
        pushButton_takephoto = new QPushButton(mdmedical);
        pushButton_takephoto->setObjectName(QStringLiteral("pushButton_takephoto"));
        pushButton_takephoto->setGeometry(QRect(100, 140, 41, 21));
        pushButton_closecam = new QPushButton(mdmedical);
        pushButton_closecam->setObjectName(QStringLiteral("pushButton_closecam"));
        pushButton_closecam->setGeometry(QRect(150, 140, 41, 21));
        pushButton_setting = new QPushButton(mdmedical);
        pushButton_setting->setObjectName(QStringLiteral("pushButton_setting"));
        pushButton_setting->setGeometry(QRect(0, 170, 51, 21));
        label_zlzqIcon = new QLabel(mdmedical);
        label_zlzqIcon->setObjectName(QStringLiteral("label_zlzqIcon"));
        label_zlzqIcon->setGeometry(QRect(0, 190, 31, 16));
        label_tagetIcon = new QLabel(mdmedical);
        label_tagetIcon->setObjectName(QStringLiteral("label_tagetIcon"));
        label_tagetIcon->setGeometry(QRect(0, 210, 31, 16));
        label_MaxIcon = new QLabel(mdmedical);
        label_MaxIcon->setObjectName(QStringLiteral("label_MaxIcon"));
        label_MaxIcon->setGeometry(QRect(0, 230, 31, 16));
        label_coldwaterstream = new QLabel(mdmedical);
        label_coldwaterstream->setObjectName(QStringLiteral("label_coldwaterstream"));
        label_coldwaterstream->setGeometry(QRect(60, 170, 41, 21));
        label_coldwaterstreamIcon = new QLabel(mdmedical);
        label_coldwaterstreamIcon->setObjectName(QStringLiteral("label_coldwaterstreamIcon"));
        label_coldwaterstreamIcon->setGeometry(QRect(110, 170, 41, 16));
        groupBox_zlzq = new QGroupBox(mdmedical);
        groupBox_zlzq->setObjectName(QStringLiteral("groupBox_zlzq"));
        groupBox_zlzq->setGeometry(QRect(60, 190, 41, 16));
        groupBox_tagetTmp = new QGroupBox(mdmedical);
        groupBox_tagetTmp->setObjectName(QStringLiteral("groupBox_tagetTmp"));
        groupBox_tagetTmp->setGeometry(QRect(60, 210, 41, 16));
        groupBox_Max = new QGroupBox(mdmedical);
        groupBox_Max->setObjectName(QStringLiteral("groupBox_Max"));
        groupBox_Max->setGeometry(QRect(60, 230, 41, 16));
        groupBox_coldwaterstream = new QGroupBox(mdmedical);
        groupBox_coldwaterstream->setObjectName(QStringLiteral("groupBox_coldwaterstream"));
        groupBox_coldwaterstream->setGeometry(QRect(110, 190, 41, 16));
        groupBox_channelselect = new QGroupBox(mdmedical);
        groupBox_channelselect->setObjectName(QStringLiteral("groupBox_channelselect"));
        groupBox_channelselect->setGeometry(QRect(110, 210, 41, 21));
        pushButton_systeminfo = new QPushButton(mdmedical);
        pushButton_systeminfo->setObjectName(QStringLiteral("pushButton_systeminfo"));
        pushButton_systeminfo->setGeometry(QRect(160, 170, 31, 21));

        retranslateUi(mdmedical);

        QMetaObject::connectSlotsByName(mdmedical);
    } // setupUi

    void retranslateUi(QWidget *mdmedical)
    {
        mdmedical->setWindowTitle(QApplication::translate("mdmedical", "mdmedical", Q_NULLPTR));
        label_Max->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_devidestatus->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_gasvalue->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_impedance1->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_impedance2->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_impedance4->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_impedance3->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_showcurtime->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tagetTmp->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tmp1->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tmp2->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tmp3->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tmp4->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tmp5->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tmp6->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tmp7->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tmp8->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_zlzq->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        pushButton_addgas->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_losegas->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_td1->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_td3->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_td2->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_td4->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_waitmode->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_calibrate->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        label_showcam->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        pushButton_showcam->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_takephoto->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_closecam->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        pushButton_setting->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
        label_zlzqIcon->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_tagetIcon->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_MaxIcon->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_coldwaterstream->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        label_coldwaterstreamIcon->setText(QApplication::translate("mdmedical", "TextLabel", Q_NULLPTR));
        groupBox_zlzq->setTitle(QApplication::translate("mdmedical", "GroupBox", Q_NULLPTR));
        groupBox_tagetTmp->setTitle(QApplication::translate("mdmedical", "GroupBox", Q_NULLPTR));
        groupBox_Max->setTitle(QApplication::translate("mdmedical", "GroupBox", Q_NULLPTR));
        groupBox_coldwaterstream->setTitle(QApplication::translate("mdmedical", "GroupBox", Q_NULLPTR));
        groupBox_channelselect->setTitle(QApplication::translate("mdmedical", "GroupBox", Q_NULLPTR));
        pushButton_systeminfo->setText(QApplication::translate("mdmedical", "PushButton", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class mdmedical: public Ui_mdmedical {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDMEDICAL_H
