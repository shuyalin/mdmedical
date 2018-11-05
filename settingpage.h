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


#define SETTINGPAGE "/mnt/mdmedical1/res/SETTINGPAGE/"
#define SETTINBACKGROUND "/mnt/mdmedical1/res/Background/"
#if 1
typedef struct CCurrentstatusvalue
{
     int cureCycle = 0;
     int targetTmp  = 50;
     int maxPower  = 0;
     int curePos  = 0;
}CCURRENTSTATUSVALUE;
#endif



class CSettingPage : public QWidget
{
    Q_OBJECT
public:
    explicit CSettingPage(QWidget *parent = 0);
    ~CSettingPage();

    void initQComboBox();


private slots:
    void GetCureCycleCurrentValue();
    void GetTargetTmpCurrentValue();
    void GetMaxPowerCurrentValue();
    void GetCurePosCurrentValue();

    void CloseSettingPage();
    void CloseSettingOkPage();

private:
#if 1
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

    QLabel *date;
    QLabel *time;
    QLabel *year;
    QLabel *month;
    QLabel *day;
    QLabel *hour;
    QLabel *minute;

    QPushButton  *year_up;
    QPushButton   *year_down;
    QPushButton  *month_up;
    QPushButton   *month_down;
    QPushButton  *day_up;
    QPushButton   *day_down;
    QPushButton  *hour_up;
    QPushButton   *hour_down;
    QPushButton  *minute_up;
    QPushButton   *minute_down;
    QPushButton   *ok;
    QPushButton   *cancel;



#endif
};



#endif // SETTINGPAGE_H
