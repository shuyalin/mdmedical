#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QTextStream>
#include <QDebug>
#include <QString>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QRadioButton>
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>




class CSettingPage : public QWidget
{
    Q_OBJECT
public:
    explicit CSettingPage(QWidget *parent = 0);
    ~CSettingPage();
    //void SetVisible(bool visible);
    void initQComboBox();
signals:

public slots:
    //void OnDeviceInfoBtnClicked();
    //void OnNetInfoBtnClicked();

private:
#if 0
    QGroupBox *m_groupbox_curePeriodSet;
    QLabel *m_label_curePeriodSet;
    QLabel *m_label_curePeriodIcon;

    QGroupBox *m_groupbox_cureTempSet;
    QLabel *m_label_cureTempSet;
    QLabel *m_label_cureTempIcon;

    QGroupBox *m_groupbox_curePowerSet;
    QLabel *m_label_curePowerSet;
    QLabel *m_label_curePowerIcon;

    QGroupBox *m_groupbox_coldWaterSet;
    QLabel *m_label_coldWaterSet;
    QLabel *m_label_coldWaterIcon;

    QComboBox *m_box_curePeriodSet;
    QComboBox *m_box_cureTempSet;
    QComboBox *m_box_curePowerSet;
    QComboBox *m_box_coldWaterSet;

    QVBoxLayout *vbox_period;
    QVBoxLayout *vbox_temp;
    QVBoxLayout *vbox_power;
    QVBoxLayout *vbox_water;
#endif
};


#endif // SETTINGPAGE_H
