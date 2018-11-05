#include "settingpage.h"
#include <iostream>



using namespace std;


CCURRENTSTATUSVALUE surrentstatusvalue;
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


CSettingPage::CSettingPage(QWidget *parent) :
    QWidget(parent)
{

#if 1
    m_groupbox_curePeriodSet = new QGroupBox(tr("period"),this);
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

    m_groupbox_cureTempSet = new QGroupBox(tr("temprate"),this);
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


    m_groupbox_curePowerSet = new QGroupBox(tr("power"),this);
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


    m_groupbox_coldWaterSet = new QGroupBox(tr("water"),this);
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



    initQComboBox();



    date = new QLabel(this);
    date->setGeometry(300,300,150,50);
    date->setText("current date");
    time = new QLabel(this);
    time->setGeometry(600,300,150,50);
    time->setText("current time");

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
void CSettingPage::CloseSettingPage()
{
    g_isSetting = true;
    close();
}
void CSettingPage::CloseSettingOkPage()
{
    g_isSetting = true;
    close();
}
