#include "settingpage.h"
#include <iostream>



using namespace std;

static const char *rtcname;

CCURRENTSTATUSVALUE surrentstatusvalue;
CCURRENTDATETIME currentdatetime;
bool g_isSetting = false;

static const char curePeriodValue[][8] = {"1:00","1:10","1:20","1:30","1:40","1:50","2:00","2:10","2:20","2:30","2:40",
"2:50","3:00","3:10","3:20","3:30","3:40","3:50","4:00","4:10","4:20","4:30","4:40","4:50","5:00","5:10","5:20","5:30","5:40","5:50",
  "6:00","6:10","6:20","6:30","6:40","6:50","7:00","7:10","7:20","7:30","7:40","7:50","8:00","8:10","8:20","8:30","8:40","8:50",
 "9:00","9:10","9:20","9:30","9:40","9:50","10:00"};


static const char cureTempValue[][8] = {"50","51","52","53","54","55","56","57","58","59","60","61","62",
"63","64","65","66","67","68","69","70","71","72","73","74","75","76","77","78","79","80","81","82","83","84",
"85","86","87","88","89","90","91","92","93","94","95"};



static const char   curePowerValue[][8] = {"0.1","0.5","1.0","1.5","2.0","2.5","3.0","3.5","4.0","4.5","5.0","5.5","6.0",
"6.5","7.0","7.5","8.0","8.5","9.0","9.5","10.0"};

static const char   cureWaterValue[][8] = {"25.0","25.5","26.0","26.5","27.0","27.5","28.0","28.5","29.0","29.5","30.0","30.5",
"31.0","31.5","32.0","32.5","33.0","33.5","34.0","34.5","35.0","35.5","36.0","36.5","37.0","37.5","38.0","38.5","39.0","39.5",
"40.0","40.5","41.0","41.5","42.0","42.5","43.0","43.5","44.0","44.5","45.0","45.5","46.0","46.5","47.0","47.5","48.0","48.5",
"49.0","49.5","50.0","50.5","51.0","51.5","52.0","52.5","53.0","53.5","54.0","54.5","55.0","55.5","56.0","56.5","57.0","57.5",
"58.0","58.5","59.0","59.5","60.0","60.5","61.0","61.5","62.0","62.5","63.0","63.5","64.0","64.5","65.0"};



static const char aviableSelectRearValue[][8] = {"2018","2019","2020","2021","2022","2023","2024","2025","2026","2027",
 "2028","2029","2030"};

static const char aviableSelectMonthValue[][8] = {"01","02","03","04","05","06","07","08","09","10","11","12"};

static const char aviableSelectDayValue[][8] = {"01","02","03","04","05","06","07","08","09","10",
 "11","12","13","14","15","16","17","18","19","20,","21","22","23","24","25","26","27","28","29","30"};

static const char aviableSelectHourValue[][8] = {"01","02","03","04","05","06","07","08","09","10",
 "11","12","13","14","15","16","17","18","19","20","21","22","23","24"};

static const char aviableSelectMinuteValue[][8] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17",
 "18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41",
"42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59"};

static const char aviableSelectSecondValue[][8] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17",
 "18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40","41",
"42","43","44","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59"};


int  SetSysDateAndTime(const char *time_str);
void SetHWClockFromSysClock(int utc);
static int  rtc_xopen(const char **default_rtc, int flags);
static void write_rtc(time_t t, int utc);

CSettingPage::CSettingPage(QWidget *parent) :
    QWidget(parent)
{

#if 1
    m_groupbox_curePeriodSet = new QGroupBox(QString::fromUtf8("治疗周期设置（分)"),this);
    m_groupbox_curePeriodSet->setGeometry(10,10,200,200);
    m_label_curePeriodIcon = new QLabel(this);
    m_label_curePeriodIcon->setGeometry(20,40,30,30);
    m_label_curePeriodIcon->setPixmap(QPixmap(SETTINGPAGE"cure.png"));
    m_box_curePeriodSet = new QComboBox(this);
    m_box_curePeriodSet->setGeometry(60,40,100,30);
    m_box_curePeriodSet->setFocusPolicy(Qt::NoFocus);
    m_label_curePeriodSet = new QLabel(this);
    m_label_curePeriodSet->setWordWrap(true);
    m_label_curePeriodSet->setGeometry(20,100,150,50);
    m_label_curePeriodSet->setText(QString::fromUtf8("设置单次治疗时间"));

    m_groupbox_curePeriodSet->setStyleSheet(
                    "QGroupBox"
                              "{"
                              "border: 2px solid black;"
                              "border-radius:8px;"
                              "margin-top:6px;"
                              "}"
                              "QGroupBox:title"
                              "{"
                              "color:black;"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "}"

                      );

    m_groupbox_cureTempSet = new QGroupBox(QString::fromUtf8("治疗温度设置（℃）"),this);
    m_groupbox_cureTempSet->setGeometry(260,10,200,200);
    m_label_cureTempIcon = new QLabel(this);
    m_label_cureTempIcon->setGeometry(270,40,14,30);
    m_label_cureTempIcon->setPixmap(QPixmap(SETTINGPAGE"temp.png"));
    m_box_cureTempSet = new QComboBox(this);
    m_box_cureTempSet->setGeometry(290,40,100,30);
    m_label_cureTempSet = new QLabel(this);
    m_label_cureTempSet->setWordWrap(true);
    m_label_cureTempSet->setGeometry(270,100,150,50);
    m_label_cureTempSet->setText(QString::fromUtf8("设置治疗最大温度"));


    m_groupbox_cureTempSet->setStyleSheet(
                    "QGroupBox"
                              "{"
                              "border: 2px solid black;"
                              "border-radius:8px;"
                              "margin-top:6px;"
                              "}"
                              "QGroupBox:title"
                              "{"
                              "color:black;"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "}"

                      );


    m_groupbox_curePowerSet = new QGroupBox(QString::fromUtf8("治疗功率设置（W）"),this);
    m_groupbox_curePowerSet->setGeometry(510,10,200,200);
    m_label_curePowerIcon = new QLabel(this);
    m_label_curePowerIcon->setGeometry(520,40,30,30);
    m_label_curePowerIcon->setPixmap(QPixmap(SETTINGPAGE"power.png"));
    m_box_curePowerSet = new QComboBox(this);
    m_box_curePowerSet->setGeometry(560,40,100,30);
    m_label_curePowerSet = new QLabel(this);
    m_label_curePowerSet->setWordWrap(true);
    m_label_curePowerSet->setGeometry(520,100,150,50);
    m_label_curePowerSet->setText(QString::fromUtf8("设置最大功率"));

    m_groupbox_curePowerSet->setStyleSheet(
                    "QGroupBox"
                              "{"
                              "border: 2px solid black;"
                              "border-radius:8px;"
                              "margin-top:6px;"
                              "}"
                              "QGroupBox:title"
                              "{"
                              "color:black;"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "}"

                      );


    m_groupbox_coldWaterSet = new QGroupBox(QString::fromUtf8("冷却液流量（毫升）"),this);
    m_groupbox_coldWaterSet->setGeometry(760,10,200,200);
    m_label_coldWaterIcon = new QLabel(this);
    m_label_coldWaterIcon->setGeometry(770,40,30,30);
    //m_label_coldWaterIcon->setPixmap(QPixmap(SETTINGPAGE"water.png"));
    m_box_coldWaterSet = new QComboBox(this);
    m_box_coldWaterSet->setGeometry(810,40,100,30);
    m_label_coldWaterSet = new QLabel(this);
    m_label_coldWaterSet->setWordWrap(true);
    m_label_coldWaterSet->setGeometry(760,100,150,50);
    m_label_coldWaterSet->setText(QString::fromUtf8("设置最大水流量"));

    m_groupbox_coldWaterSet->setStyleSheet(
                    "QGroupBox"
                              "{"
                              "border: 2px solid black;"
                              "border-radius:8px;"
                              "margin-top:6px;"
                              "}"
                              "QGroupBox:title"
                              "{"
                              "color:black;"
                              "subcontrol-origin: margin;"
                              "left: 10px;"
                              "}"

                      );

#if 1
    setdatetime = new QLabel(this);
    setdatetime->setGeometry(300,300,200,50);
    setdatetime->setText(QString::fromUtf8("设置日期时间"));
//    time = new QLabel(this);
//    time->setGeometry(600,300,150,50);
//    time->setText("current time");

#endif
    m_box_year = new QComboBox(this);
    m_box_month = new QComboBox(this);
    m_box_day = new QComboBox(this);
    m_box_hour = new QComboBox(this);
    m_box_minute = new QComboBox(this);

    m_box_year->setGeometry(100,400,100,40);
    m_box_month->setGeometry(250,400,100,40);
    m_box_day->setGeometry(400,400,100,40);
    m_box_hour->setGeometry(550,400,100,40);
    m_box_minute->setGeometry(700,400,100,40);

     initQComboBox();

#if 0
    year = new QLabel(this);
    year->setGeometry(320,400,80,30);
    year->setText("year");
    year_up = new QPushButton(this);
    year_up->setGeometry(320,370,80,30);
    year_up->setText("UP");
    year_up->setIcon(QIcon(SETTINGPAGE"up.png"));
    year_up->setIconSize(QSize(80,40));
    year_down = new QPushButton(this);
    year_down->setGeometry(320,430,80,30);
    year_down->setText("DN");
    year_down->setIcon(QIcon(SETTINGPAGE"down.png"));
    year_down->setIconSize(QSize(80,40));

    month = new QLabel(this);
    month->setGeometry(420,400,80,30);
    month->setText("month");
    month_up = new QPushButton(this);
    month_up->setGeometry(420,370,80,30);
    month_up->setIcon(QIcon(SETTINGPAGE"up.png"));
    month_up->setIconSize(QSize(80,40));
    month_down = new QPushButton(this);
    month_down->setGeometry(420,430,80,30);
    month_down->setIcon(QIcon(SETTINGPAGE"down.png"));
    month_down->setIconSize(QSize(80,40));

    day = new QLabel(this);
    day->setGeometry(520,400,80,30);
    day->setText("day");
    day_up = new QPushButton(this);
    day_up->setGeometry(520,370,80,30);
    day_up->setIcon(QIcon(SETTINGPAGE"up.png"));
    day_up->setIconSize(QSize(80,40));
    day_down = new QPushButton(this);
    day_down->setGeometry(520,430,80,30);
    day_down->setIcon(QIcon(SETTINGPAGE"down.png"));
    day_down->setIconSize(QSize(80,40));


    hour = new QLabel(this);
    hour->setGeometry(620,400,80,30);
    hour->setText("hour");
    hour_up = new QPushButton(this);
    hour_up->setGeometry(620,370,80,30);
    hour_up->setIcon(QIcon(SETTINGPAGE"up.png"));
    hour_up->setIconSize(QSize(80,40));
    hour_down = new QPushButton(this);
    hour_down->setGeometry(620,430,80,30);
    hour_down->setIcon(QIcon(SETTINGPAGE"down.png"));
    hour_down->setIconSize(QSize(80,40));


    minute = new QLabel(this);
    minute->setGeometry(720,400,80,30);
    minute->setText("minute");
    minute_up = new QPushButton(this);
    minute_up->setGeometry(720,370,80,30);
    minute_up->setIcon(QIcon(SETTINGPAGE"up.png"));
    minute_up->setIconSize(QSize(80,40));
    minute_down = new QPushButton(this);
    minute_down->setGeometry(720,430,80,30);
    minute_down->setIcon(QIcon(SETTINGPAGE"down.png"));
    minute_down->setIconSize(QSize(80,40));

#endif



    ok = new QPushButton(this);
    ok->setGeometry(320,500,80,40);
    //ok->setText("OK");
    ok->setIcon(QIcon(SETTINGPAGE"ok.png"));
    ok->setIconSize(QSize(80,40));
    ok->setFocusPolicy(Qt::NoFocus);
    cancel = new QPushButton(this);
    cancel->setGeometry(700,500,80,40);
    //cancel->setText("CANCEL");
    cancel->setIcon(QIcon(SETTINGPAGE"cancel.png"));
    cancel->setIconSize(QSize(80,40));
    cancel->setFocusPolicy(Qt::NoFocus);

    connect(m_box_curePeriodSet, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetCureCycleCurrentValue()));
    connect(m_box_cureTempSet, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetTargetTmpCurrentValue()));
    connect(m_box_curePowerSet, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetMaxPowerCurrentValue()));
    connect(m_box_coldWaterSet, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetCurePosCurrentValue()));

    connect(m_box_year, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetYearValue()));
    connect(m_box_month, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetMonthValue()));
    connect(m_box_day, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetDayValue()));
    connect(m_box_hour, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetHourValue()));
    connect(m_box_minute, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetMinuteValue()));

    connect(cancel,SIGNAL(clicked()),this,SLOT(CloseSettingPage()));
    connect(ok,SIGNAL(clicked()),this,SLOT(CloseSettingOkPage()));


//    QPixmap pixmap(SETTINBACKGROUND"setting.png");
//    QPalette palette;
//    palette.setBrush(backgroundRole(), QBrush(pixmap));
//    setPalette(palette);


//    QPalette palette(this->palette());
//    palette.setColor(QPalette::Background, Qt::blue);
//    this->setPalette(palette);

//    setAutoFillBackground(true);//必须有这条语句
//    setPalette(QPalette(QColor(123,34,123)));


#endif
}


CSettingPage::~CSettingPage()
{

}

void CSettingPage::initQComboBox()
{
    int i = 0;
    for(i = 0;i < 55;i++)
    {
        m_box_curePeriodSet->addItem(QWidget::tr(curePeriodValue[i]));
    }

    for(i = 0;i < 46;i++)
    {
        m_box_cureTempSet->addItem(QWidget::tr(cureTempValue[i]));
    }

    for(i = 0;i < 46;i++)
    {
        m_box_curePowerSet->addItem(QWidget::tr(curePowerValue[i]));
    }

    for(i = 0;i < 46;i++)
    {
        m_box_coldWaterSet->addItem(QWidget::tr(cureWaterValue[i]));
    }
    for(i = 0;i < 13;i++)
    {
        m_box_year->addItem(QWidget::tr(aviableSelectRearValue[i]));
    }
    for(i = 0;i < 12;i++)
    {
        m_box_month->addItem(QWidget::tr(aviableSelectMonthValue[i]));
    }
    for(i = 0;i < 30;i++)
    {
        m_box_day->addItem(QWidget::tr(aviableSelectDayValue[i]));
    }
    for(i = 0;i < 24;i++)
    {
        m_box_hour->addItem(QWidget::tr(aviableSelectHourValue[i]));
    }
    for(i = 0;i < 60;i++)
    {
        m_box_minute->addItem(QWidget::tr(aviableSelectMinuteValue[i]));
    }

}



/*获得当前治疗周期时间*/
void CSettingPage::GetCureCycleCurrentValue()
{
    surrentstatusvalue.cureCycle = m_box_curePeriodSet->currentIndex();
}
/*获得当前目标温度*/
void CSettingPage::GetTargetTmpCurrentValue()
{
    surrentstatusvalue.targetTmp = m_box_cureTempSet->currentIndex();
}

/*获得当前功率值*/
void CSettingPage::GetMaxPowerCurrentValue()
{
    surrentstatusvalue.maxPower = m_box_curePowerSet->currentIndex();
}

/*获得当前治疗位置值*/
void CSettingPage::GetCurePosCurrentValue()
{
    surrentstatusvalue.curePos = m_box_coldWaterSet->currentIndex();
}

/*获得当前治疗位置值*/
void CSettingPage::GetYearValue()
{
    currentdatetime.year = m_box_year->currentIndex();
}
/*获得当前治疗位置值*/
void CSettingPage::GetMonthValue()
{
    currentdatetime.month = m_box_month->currentIndex();
    printf("%d\n",currentdatetime.month);
}
/*获得当前治疗位置值*/
void CSettingPage::GetDayValue()
{
    currentdatetime.day = m_box_day->currentIndex();
    printf("%d\n",currentdatetime.day);
}
/*获得当前治疗位置值*/
void CSettingPage::GetHourValue()
{
    currentdatetime.hour = m_box_hour->currentIndex();
    printf("%d\n",currentdatetime.hour);
}
/*获得当前治疗位置值*/
void CSettingPage::GetMinuteValue()
{
    currentdatetime.minute = m_box_minute->currentIndex();
    printf("%d\n",currentdatetime.minute);
}


void CSettingPage::CloseSettingPage()
{
    g_isSetting = true;
    close();
}

//"2018-11-19 14:11:33";
void CSettingPage::CloseSettingOkPage()
{
    char time_str[64]={0};
    strcpy(time_str,aviableSelectRearValue[currentdatetime.year]);
    strcat(time_str,"-");
    strcat(time_str,aviableSelectMonthValue[currentdatetime.month]);
    strcat(time_str,"-");
    strcat(time_str,aviableSelectDayValue[currentdatetime.day]);
    strcat(time_str,"  ");
    strcat(time_str,aviableSelectHourValue[currentdatetime.hour]);
    strcat(time_str,":");
    strcat(time_str,aviableSelectMinuteValue[currentdatetime.minute]);
    strcat(time_str,":");
    strcat(time_str,aviableSelectSecondValue[currentdatetime.second]);


    printf("%s\n",time_str);

    SetSysDateAndTime(time_str);
     SetHWClockFromSysClock(0);


    g_isSetting = true;
    close();
}

/*
char * CSettingPage::transdatetimetostr()
{
     QString str = QString(aviableSelectRearValue[currentdatetime.year])+ QString("  ")+QString(aviableSelectMonthValue[currentdatetime.month])+\
            + QString("  ")+QString(aviableSelectDayValue[currentdatetime.day])+ QString("  ")+QString(aviableSelectHourValue[currentdatetime.hour])+\
             QString("  ")+QString(aviableSelectMinuteValue[currentdatetime.minute])+ QString("  ")+QString(aviableSelectSecondValue[currentdatetime.second]) ;
    const datetime = str.toStdString().c_str();
    return datetime;
}
*/
int SetSysDateAndTime(const char *time_str)
{
    struct tm 			time_tm;
    struct timeval 	time_tv;
    time_t 					timep;
        int 						ret;

        if(time_str == NULL)
        {
                fprintf(stderr, "time string args invalid!\n");
                return -1;
        }

    sscanf(time_str, "%d-%d-%d %d:%d:%d", &time_tm.tm_year, &time_tm.tm_mon, &time_tm.tm_mday, &time_tm.tm_hour, &time_tm.tm_min, &time_tm.tm_sec);
    time_tm.tm_year 	-= 1900;
    time_tm.tm_mon 		-= 1;
    time_tm.tm_wday 	= 0;
    time_tm.tm_yday 	= 0;
    time_tm.tm_isdst	= 0;

    timep = mktime(&time_tm);
    time_tv.tv_sec  = timep;
    time_tv.tv_usec = 0;

    ret = settimeofday(&time_tv, NULL);
    if(ret != 0)
    {
        fprintf(stderr, "settimeofday failed\n");
        return -2;
    }

    return 0;
}


void SetHWClockFromSysClock(int utc)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    //if (gettimeofday(&tv, NULL))
    //	bb_perror_msg_and_die("gettimeofday() failed");
    write_rtc(tv.tv_sec, utc);
}



static int  rtc_xopen(const char **default_rtc, int flags)
{
    int rtc;

    if (!*default_rtc) {
        *default_rtc = "/dev/rtc";
        rtc = open(*default_rtc, flags);
        if (rtc >= 0)
            return rtc;
        *default_rtc = "/dev/rtc0";
        rtc = open(*default_rtc, flags);
        if (rtc >= 0)
            return rtc;
        *default_rtc = "/dev/misc/rtc";
    }

    return open(*default_rtc, flags);
}

static void write_rtc(time_t t, int utc)
{
#define RTC_SET_TIME	_IOW('p', 0x0a, struct rtc_time) /* Set RTC time    */

    struct tm tm;
    int rtc = rtc_xopen(&rtcname, O_WRONLY);

    tm = *(utc ? gmtime(&t) : localtime(&t));
    tm.tm_isdst = 0;

    ioctl(rtc, RTC_SET_TIME, &tm);
    close(rtc);
}
