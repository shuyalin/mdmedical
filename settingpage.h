#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QTextStream>
#include <QDebug>
#include <QString>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QDebug>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QProcess>

#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <linux/rtc.h>
#include <linux/capability.h>

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

typedef struct CCurrentdatetime
{
     int year = 0;
     int   month = 0;
     int day  = 0;
     int hour  = 0;
     int minute = 0;
     int second = 0;
}CCURRENTDATETIME;


class CSettingPage : public QDialog
{
    Q_OBJECT
public:
    explicit CSettingPage(QDialog *parent = 0);
    ~CSettingPage();


    char *datetime;

    void initQComboBox();

    char * transdatetimetostr();

    /*这一段是仿busybox设置linux时钟的api*/



private slots:
    void GetCureCycleCurrentValue();
    void GetTargetTmpCurrentValue();
    void GetMaxPowerCurrentValue();
    void GetCurePosCurrentValue();

    void GetYearValue();
    void GetMonthValue();
    void GetDayValue();
    void GetHourValue();
    void GetMinuteValue();


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

    QComboBox *m_box_year;
    QComboBox *m_box_month;
    QComboBox *m_box_day;
    QComboBox *m_box_hour;
    QComboBox *m_box_minute;

    QLabel *setdatetime;
    #if 0
    QLabel *time;
    QLabel *year;
    QLabel *month;
    QLabel *day;
    QLabel *hour;
    QLabel *minute;
#endif
#if 0
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

#endif
    QPushButton   *ok;
    QPushButton   *cancel;





#endif
};



#endif // SETTINGPAGE_H
