#include "mdmedical.h"
#include "ui_mdmedical.h"


bool port[4] = {false,false,false,false};

float currTmp[8];
float showtmp[4];
//bool isTmpFinished = false;
//bool isShowFinished = false;
int indexpower;

bool g_bWaitCommand = false;

unsigned short impedance[4];

CSerial sel1;
CSerial sel2;
CSerial sel4;
CSerial sel5;

bool g_bPrerareMode = false;
bool g_bIdleMode = false;
bool g_bCureMode = false;
bool g_bPwmdMode = false;
bool g_bPwmiMode = false;
bool g_bPwmkeepMode = false;
bool g_bCureClosed = false;

bool g_bPrepareKey = false;
bool g_bFootKey = false;
bool g_bFootKeyHide = false;
bool m_bIsfootkeyPress;

volatile bool g_bIsFinishiIncreasePower = true;
volatile bool g_bIsfinishReadtempture = true;

pthread_mutex_t g_mutex;

QVector<double> temp(1200);  //zhengjian max
QVector<double> temp1(1200);  //zhengjian min
QVector<double> temp2(1200);  //nianmo Max
QVector<double> temp3(1200);  //nianmo Min
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/



static char powerindex[21][5]={"0.1","0.5","1.0","1.5","2.0","2.5","3.0","3.5","4.0","4.5","5.0","5.5","6.0","6.5","7.0","7.5","8.0","8.5","9.0","9.5","10.0"};


float ReturnMaxValue(float a,float b,float c,float d)
{
    int tmp;
    if(a>b)
        tmp = a;
    else
        tmp = b;
    if(tmp<c)
        tmp = c;
    if(tmp<d)
        tmp = d;
    return tmp;
}
float ReturnMinValue(float a,float b,float c,float d)
{
    int tmp;
    if(a>b)
        tmp = b;
    else
        tmp = a;
    if(tmp > c)
        tmp = c;
    if(tmp > d)
        tmp = d;
    return tmp;
}

/*
void *SendSerialFun(void *args)
{
    CSerial* tmp = (CSerial *)args;
    tmp->ControlBuzzer(0x23,0x34,0x2);
    tmp->RecvData();
}
*/
bool Idle_send()
{
    bool ok1 = false,ok2 = false,ok3 = false,ok4 = false;
    sel1.IdleSend(0);
    sel1.SendData();
    sel1.RecvData();
    ok1 = sel1.ParseIdleSendReturnData();

    sel2.IdleSend(0);
    sel2.SendData();
    sel2.RecvData();
    ok2 = sel2.ParsePrepareSendReturnData();

    sel4.IdleSend(0);
    sel4.SendData();
    sel4.RecvData();
    ok3 = sel4.ParsePrepareSendReturnData();

    sel5.IdleSend(0);
    sel5.SendData();
    sel5.RecvData();
    ok4 = sel5.ParsePrepareSendReturnData();

    return (ok1&&ok2&&ok3&&ok4);
}

bool cure_send()
{
    while(!g_bIsFinishiIncreasePower)
        usleep(10*1000);
    bool ok1=false,ok2=false,ok3=false,ok4=false;
    sel1.CureComand(powerindex[indexpower],3);//治疗模式，发送最大功率
    sel1.SendData();
    sel1.RecvData();
    ok1 = sel1.ParseCureSendReturnData();

    sel2.CureComand(powerindex[indexpower],3);
    sel2.SendData();
    sel2.RecvData();
    ok2 = sel2.ParseCureSendReturnData();

    sel4.CureComand(powerindex[indexpower],3);
    sel4.SendData();
    sel4.RecvData();
    ok3 = sel4.ParseCureSendReturnData();

    sel5.CureComand(powerindex[indexpower],3);
    sel5.SendData();
    sel5.RecvData();
    ok4 = sel5.ParseCureSendReturnData();

    return (ok1&&ok2&&ok3&&ok4);
}

void Prepare_send()
{
    //float f;
    //float f1;
    sel1.PrepareSend(0);
    sel1.SendData();
    sel1.RecvData();
    sel1.ParsePrepareSendReturnData();
    impedance[0] = sel1.impence;
#if 0
    if(m_bIsfootkeyPress)
    {
        f = (float)(sel1.powerrate[0]-0x30) + ((float)(sel1.powerrate[2]-0x30)/10);
        f1 = atof(powerindex[indexpower]);
        //printf("  the powerrate is    %.1f    ",f);
    }
#endif
    sel2.PrepareSend(0);
    sel2.SendData();
    sel2.RecvData();
    sel2.ParsePrepareSendReturnData();
    impedance[1] = sel2.impence;
    #if 0
    if(m_bIsfootkeyPress)
    {
        f = (float)(sel2.powerrate[0]-0x30) + ((float)(sel2.powerrate[2]-0x30)/10);
        f1 = atof(powerindex[indexpower]);
        //printf("     %.1f    ",f);
    }
    #endif
    sel4.PrepareSend(0);
    sel4.SendData();
    sel4.RecvData();
    sel4.ParsePrepareSendReturnData();
    impedance[2] = sel4.impence;
    #if 0
    if(m_bIsfootkeyPress)
    {
        f = (float)(sel4.powerrate[0]-0x30) + ((float)(sel4.powerrate[2]-0x30)/10);
        f1 = atof(powerindex[indexpower]);
        //printf("     %.1f    ",f);
    }
    #endif
    sel5.PrepareSend(0);
    sel5.SendData();
    sel5.RecvData();
    sel5.ParsePrepareSendReturnData();
    impedance[3] = sel5.impence;
     #if 0
    if(m_bIsfootkeyPress)
    {
        f = (float)(sel5.powerrate[0]-0x30) + ((float)(sel5.powerrate[2]-0x30)/10);
        f1 = atof(powerindex[indexpower]);
        //printf("     %.1f     \n",f);
    }
    #endif
}
/*发送命令给串口减小功率*/
bool pwmd_send()
{
    while(!g_bIsFinishiIncreasePower)
        usleep(10*1000);
    bool ok1=false,ok2=false,ok3=false,ok4=false;
    if(port[0])
    {
        sel1.PwmdSend(0);
        sel1.SendData();
    }
    if(port[1])
    {
        sel2.PwmdSend(0);
        sel2.SendData();
    }
    if(port[2])
    {
        sel4.PwmdSend(0);
        sel4.SendData();
    }
    if(port[3])
    {
        sel5.PwmdSend(0);
        sel5.SendData();
    }

    if(port[0])
    {
        port[0] = true;
        sel1.RecvData();
        ok1 = sel1.ParseIdleSendReturnData();
    }
    if(port[1])
    {
        port[1] = true;
        sel2.RecvData();
        ok2 = sel2.ParseIdleSendReturnData();
    }
    if(port[2])
    {
        port[2] = true;
        sel4.RecvData();
        ok3 = sel4.ParseIdleSendReturnData();
    }
    if(port[3])
    {
        port[3] = true;
        sel5.RecvData();
        ok4 = sel5.ParseIdleSendReturnData();
    }

    return (ok1&&ok2&&ok3&&ok4);
}
/*发送命令给串口增大功率*/
bool pwmi_send()
{

    bool ok1=false,ok2=false,ok3=false,ok4=false;

    if(port[0])
    {
        sel1.PwmiSend(0);
        sel1.SendData();
    }
    if(port[1])
    {
        sel2.PwmiSend(0);
        sel2.SendData();
    }
    if(port[2])
    {
        sel4.PwmiSend(0);
        sel4.SendData();
    }
    if(port[3])
    {
        sel5.PwmiSend(0);
        sel5.SendData();
    }

    if(port[0])
    {
        port[0] = false;
        sel1.RecvData();
        ok1 = sel1.ParseIdleSendReturnData();
    }

    if(port[1])
    {
        port[1] = false;
        sel2.RecvData();
        ok2 = sel2.ParseIdleSendReturnData();
    }
    if(port[2])
    {
        port[2] = false;
        sel4.RecvData();
        ok3 = sel4.ParseIdleSendReturnData();
    }
    if(port[3])
    {
        port[3] = false;
        sel5.RecvData();
        ok4 = sel5.ParseIdleSendReturnData();
    }

    return (ok1&&ok2&&ok3&&ok4);
}
/*发送命令给串口保持功率不变*/
bool pwmkeep_send()
{
    bool ok1=false,ok2=false,ok3=false,ok4=false;
    if(port[0])
    {
        sel1.PwmKeepSend(0);
        sel1.SendData();
    }
    if(port[1])
    {
        sel2.PwmKeepSend(0);
        sel2.SendData();
    }
    if(port[2])
    {
        sel4.PwmKeepSend(0);
        sel4.SendData();
    }
    if(port[3])
    {
        sel5.PwmKeepSend(0);
        sel5.SendData();
    }

    if(port[0])
    {
        port[0] = false;
        sel1.RecvData();
        ok1 = sel1.ParseIdleSendReturnData();
    }
    if(port[1])
    {
        port[1] = false;
        sel2.RecvData();
        ok2 = sel2.ParseIdleSendReturnData();
    }
    if(port[2])
    {
        port[2] = false;
        sel4.RecvData();
        ok3 = sel4.ParseIdleSendReturnData();
    }
    if(port[3])
    {
        port[3] = false;
        sel5.RecvData();
        ok4 = sel5.ParseIdleSendReturnData();
    }

    return (ok1&&ok2&&ok3&&ok4);
}

bool cure_close()
{
    bool ok1=false,ok2=false,ok3=false,ok4=false;
    sel1.CureClosed(0);
    sel1.SendData();
    sel1.RecvData();
    ok1 = sel1.ParseIdleSendReturnData();

    sel2.CureClosed(0);
    sel2.SendData();
    sel2.RecvData();
    ok2 = sel2.ParseIdleSendReturnData();

    sel4.CureClosed(0);
    sel4.SendData();
    sel4.RecvData();
    ok3 = sel4.ParseIdleSendReturnData();

    sel5.CureClosed(0);
    sel5.SendData();
    sel5.RecvData();
    ok4 = sel5.ParseIdleSendReturnData();

    return (ok1&&ok2&&ok3&&ok4);
}


void *GetCurrTmp(void *args)
{
        unsigned short tmp,count = 0;
        unsigned int sum_tmp = 0,i;
        float currTmp1[8];
        float currTmp2[8];

        sel1.openSerial(SERIAL1,115200);
        sel2.openSerial(SERIAL2,115200);
        sel4.openSerial(SERIAL4,115200);
        sel5.openSerial(SERIAL5,115200);

        int fd = open("/dev/mdmedical_tmp", O_RDWR);
        if(fd < 0)
        {
            printf("open device failed!\n");
        }
        usleep(2*1000);
        while(1)
        {
                    for(i=3;i<11;i++)
                    {
                        ioctl(fd,i,12);
                        usleep(2*1000);
                        while(count < 16)
                        {
                            read(fd,&tmp,2);
//                            if(tmp > 3000)
//                                printf("SS %hu",tmp);
                            count++;
                            sum_tmp += tmp;
                        }

                        currTmp1[i-3] = (float)((sum_tmp)*120.0/16/4096);//(currTmptmp[i-3] + currTmptmplast[i-3])/2;
                        count = 0;
                        sum_tmp = 0;
                    }

                    usleep(10*1000);
#if 1
                    for(i=3;i<11;i++)
                    {
                        ioctl(fd,i,12);
                        usleep(2*1000);
                        while(count < 16)
                        {
                            read(fd,&tmp,2);
                            count++;
                            sum_tmp += tmp;
                        }

                        currTmp2[i-3] = (float)((sum_tmp)*120.0/16/4096);//(currTmptmp[i-3] + currTmptmplast[i-3])/2;
                        count = 0;
                        sum_tmp = 0;
                    }
#endif

                    g_bIsFinishiIncreasePower = true;
                    for(int j=0;j<8;j++)
                    {
                        currTmp[j] = (currTmp1[j] + currTmp2[j])/2;
                        if(currTmp[j]>80)
                            printf(" %.1f \n ",currTmp[j]);
                    }
                    //usleep(300*1000);


                #if 1
                        if(g_bPrerareMode && g_bWaitCommand)
                        {
                            Prepare_send();
                            g_bPrerareMode = false;
                        }
                        else
                        {
                            usleep(10*1000);
                        }
                #if 1
                        if(g_bIdleMode)
                        {
                            g_bWaitCommand = Idle_send();
                            g_bIdleMode = false;
                        }
                #endif
                #endif
                #if 1
                        if(g_bCureMode)
                        {
                            g_bIsfinishReadtempture = false;
                            cure_send();
                            g_bIsfinishReadtempture = true;
                            g_bCureMode = false;
                        }
                #endif
                #if 1
                        if(g_bPwmdMode)
                        {
                            pwmd_send();
                            usleep(20*1000);
                            g_bPwmdMode = false;
                            usleep(100*1000);
                        }
                #endif
                #if 1
                        if(g_bPwmiMode)
                        {
                            pwmi_send();
                            usleep(20*1000);
                            g_bPwmiMode = false;
                            usleep(100*1000);
                        }
                #endif
                #if 1
                        if(g_bPwmkeepMode)
                        {
                            printf("PWM----KKKKKKKKKKKKKKKKK  %f\n",currTmp[3]);
                            pwmkeep_send();
                            g_bPwmkeepMode = false;
                        }
                        if(g_bCureClosed)
                        {
                            cure_close();
                            g_bCureClosed = false;
                        }
                #endif
                        //usleep(400*1000);
        }
}
/*检测按键按下，/dev/mdmedical_buttons为按键的驱动程序的设备节点，驱动采用中断方式检测按键是否按下*/
void *detect_key_func(void *args)
{
    unsigned char key_val;
    int fd = open("/dev/mdmedical_buttons", O_RDWR);
    if(fd < 0)
    {
        printf("open device failed!\n");
    }
    while(1)
    {
        read(fd, &key_val, 1);
        if(key_val == 0x1)
        {
            g_bPrepareKey = true;
        }
        else if(key_val == 0x2)
        {
             g_bFootKey = true;
        }
        //printf("key_val = 0x%x\n", key_val);
    }
}


mdmedical::mdmedical(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdmedical)
{
    ui->setupUi(this);

        widget = new QCustomPlot(this);
        widget->setGeometry(10,380,600,300);
        widget->setVisible(0);
        widget->addGraph();
        widget->addGraph();
        widget->addGraph();
        widget->addGraph();
        widget->xAxis->setRange(0,600);
        widget->yAxis->setRange(20,100);


        widget->graph(0)->setName("fist line");

        widget->graph(1)->setName("second line");
        widget->graph(1)->setPen(QPen(QColor(255,0,0)));

        widget->graph(2)->setName("third line");
        widget->graph(2)->setPen(QPen(QColor(255,0,0)));

        widget->graph(3)->setName("four line");
        widget->graph(3)->setPen(QPen(QColor(0,255,0)));

        widget->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20))); //设置曲线下方颜色

        widget->xAxis->setSubTickCount(9);

        widget->xAxis->setLabel(QString::fromUtf8("时间"));
        widget->yAxis->setLabel(QString::fromUtf8("温度"));

        //显示铭文
        //widget->legend->setVisible(true);

        //设置customPlot可进行拽托或伸缩
        widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        widget->axisRect()->setRangeDrag(Qt::Horizontal);  //设置允许在某一方向上拽托
        widget->axisRect()->setRangeZoom(Qt::Horizontal);  //设置允许在某一方向上伸缩

        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(80, 80, 80));
        axisRectGradient.setColorAt(1, QColor(30, 30, 30));
        widget->axisRect()->setBackground(axisRectGradient);

        limit_count = 0;
        closegraph = 0;
        m_calibate_count = 0;
        lasttmp1 = 0.0;
        lasttmp2 = 0.0;
        lasttmp3 = 0.0;
        lasttmp4 = 0.0;

        m_minute = 1;
        m_second = 0;
        m_bIsfootkeyPress = false;

        isclosecam = false;
        istakephoto = false;

        pthread_mutex_init(&g_mutex, NULL);

        m_QProcess_calibration = new QProcess();


        m_pQTimer_showtime = new QTimer(this);
        m_pQTimer_showgraph = new QTimer(this);
        m_pQTimer_showtmp = new QTimer(this);
        m_pQTimer_showimpedance = new QTimer(this);
        m_pQTimer_preparestatus = new QTimer(this);
        m_pQTimer_detectkey = new QTimer(this);
        m_pQTimer_detectcalibrate = new QTimer(this);

        m_pQTimer_showpowerrate1 = new QTimer(this);
        m_pQTimer_showpowerrate2 = new QTimer(this);
        m_pQTimer_showpowerrate3 = new QTimer(this);
        m_pQTimer_showpowerrate4 = new QTimer(this);

        m_pQTimer_showcam = new QTimer(this);

        m_pQTimer_writedata = new QTimer(this);

         m_QComboBox_cureperiod = new QComboBox(this);
         InitCureReriodSubWidgets();
         m_QComboBox_targettemp = new QComboBox(this);
         InitTargetTempSubWidgets();
         m_QComboBox_maxpower = new QComboBox(this);
         InitMaxPowerSubWidgets();
         m_QComboBox_curepos = new QComboBox(this);
         InitCurePosSubWidgets();

         m_QLabel_pic1 = new QLabel(this);
         m_QLeftcureTime = new QLabel(this);

         progressBar1 = new QProgressBar(this);
         progressBar2 = new QProgressBar(this);
         progressBar3 = new QProgressBar(this);
         progressBar4 = new QProgressBar(this);



        connect(m_pQTimer_showpowerrate1, SIGNAL(timeout()), this, SLOT(ShowPowerRate()));
//        connect(m_pQTimer_showpowerrate2, SIGNAL(timeout()), this, SLOT(on_time_out()));
//        connect(m_pQTimer_showpowerrate3, SIGNAL(timeout()), this, SLOT(on_time_out()));
//        connect(m_pQTimer_showpowerrate4, SIGNAL(timeout()), this, SLOT(on_time_out()));

        connect(m_pQTimer_showtime, SIGNAL(timeout()), this, SLOT(ShowTimeCurrent()));
        connect(m_pQTimer_showgraph, SIGNAL(timeout()), this, SLOT(makeGraph()));
        connect(m_pQTimer_showtmp, SIGNAL(timeout()), this, SLOT(ShowCurrentTmp()));
        connect(m_pQTimer_showimpedance, SIGNAL(timeout()), this, SLOT(showCurrentImpedance()));
        connect(m_pQTimer_preparestatus, SIGNAL(timeout()), this, SLOT(ChangePrepareStatus()));
        connect(m_pQTimer_detectkey, SIGNAL(timeout()), this, SLOT(DetectKey()));
        connect(m_pQTimer_detectcalibrate, SIGNAL(timeout()), this, SLOT(DetectCalibrate()));

        connect(m_pQTimer_showcam, SIGNAL(timeout()), this, SLOT(ShowCam()));

        connect(m_pQTimer_writedata, SIGNAL(timeout()), this, SLOT(WriteData()));
        connect(ui->pushButton_td1,SIGNAL(clicked()),this,SLOT(GetChanel1Value()));
        connect(ui->pushButton_td2,SIGNAL(clicked()),this,SLOT(GetChanel2Value()));
        connect(ui->pushButton_td3,SIGNAL(clicked()),this,SLOT(GetChanel3Value()));
        connect(ui->pushButton_td4,SIGNAL(clicked()),this,SLOT(GetChanel4Value()));

        connect(m_QComboBox_cureperiod, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetCureCycleCurrentValue()));
        connect(m_QComboBox_targettemp, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetTargetTmpCurrentValue()));
        connect(m_QComboBox_maxpower, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetMaxPowerCurrentValue()));
        connect(m_QComboBox_curepos, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetCurePosCurrentValue()));




        ui->label_showcurtime->setGeometry(1,1,400,50);
        ui->label_showcurtime->setText("");

        ui->label_tmp1->setGeometry(280,100,65,20);  //chanel   1
        ui->label_tmp2->setGeometry(454,278,65,20);  //chanel   1  nianmo
        ui->label_tmp3->setGeometry(280,460,65,20);
        ui->label_tmp4->setGeometry(100,267,65,20);

        ui->label_tmp5->setGeometry(280,163,65,20);
        ui->label_tmp6->setGeometry(400,267,65,20);
        ui->label_tmp7->setGeometry(280,400,65,20);
        ui->label_tmp8->setGeometry(155,278,65,20);

        ui->label_impedance1->setGeometry(280,130,45,20);
        ui->label_impedance2->setGeometry(400,292,45,20);
        ui->label_impedance3->setGeometry(280,430,45,20);
        ui->label_impedance4->setGeometry(100,292,45,20);

        ui->label_tmp1->setText(QString::fromUtf8("tmp1"));
        ui->label_tmp2->setText(QString::fromUtf8("tmp2"));
        ui->label_tmp3->setText(QString::fromUtf8("tmp3"));
        ui->label_tmp4->setText(QString::fromUtf8("tmp4"));

        ui->label_tmp5->setText(QString::fromUtf8("15.6"));
        ui->label_tmp6->setText(QString::fromUtf8("15.6"));
        ui->label_tmp7->setText(QString::fromUtf8("15.6"));
        ui->label_tmp8->setText(QString::fromUtf8("15.6"));

        ui->label_tmp1->setAlignment(Qt::AlignHCenter);
        ui->label_tmp2->setAlignment(Qt::AlignHCenter);
        ui->label_tmp3->setAlignment(Qt::AlignHCenter);
        ui->label_tmp4->setAlignment(Qt::AlignHCenter);
        ui->label_tmp5->setAlignment(Qt::AlignHCenter);
        ui->label_tmp6->setAlignment(Qt::AlignHCenter);
        ui->label_tmp7->setAlignment(Qt::AlignHCenter);
        ui->label_tmp8->setAlignment(Qt::AlignHCenter);

        ui->label_gasvalue->setAlignment(Qt::AlignHCenter);


        ui->label_impedance1->setText("");
        ui->label_impedance2->setText("");
        ui->label_impedance3->setText("");
        ui->label_impedance4->setText("");




    #if 0
        //ui->label_tmp1->setStyleSheet ("border:2px groove gray;border-radius:10px;padding:2px 4px;");
        ui->label_tmp1->setStyleSheet ("background-color:green;color:black;border:0px groove gray;border-radius:10px;padding:2px 4px;");
        ui->label_tmp2->setStyleSheet ("background-color:green;color:black;border:0px groove gray;border-radius:10px;padding:2px 4px;");
        ui->label_tmp3->setStyleSheet ("background-color:green;color:black;border:0px groove gray;border-radius:10px;padding:2px 4px;");
        ui->label_tmp4->setStyleSheet ("background-color:green;color:black;border:0px groove gray;border-radius:10px;padding:2px 4px;");
        ui->label_tmp5->setStyleSheet ("background-color:green;color:black;border:0px groove gray;border-radius:10px;padding:2px 4px;");
        ui->label_tmp6->setStyleSheet ("background-color:green;color:black;border:0px groove gray;border-radius:10px;padding:2px 4px;");
        ui->label_tmp7->setStyleSheet ("background-color:green;color:black;border:0px groove gray;border-radius:10px;padding:2px 4px;");
        ui->label_tmp8->setStyleSheet ("background-color:green;color:black;border:0px groove gray;border-radius:10px;padding:2px 4px;");

    #endif

        ui->label_tmp1->setStyleSheet ("background-color:green;color:black;");
        ui->label_tmp2->setStyleSheet ("background-color:green;color:black;");
        ui->label_tmp3->setStyleSheet ("background-color:green;color:black;");
        ui->label_tmp4->setStyleSheet ("background-color:green;color:black;");

        ui->label_tmp5->setStyleSheet ("background-color:green;color:black;");
        ui->label_tmp6->setStyleSheet ("background-color:green;color:black;");
        ui->label_tmp7->setStyleSheet ("background-color:green;color:black;");
        ui->label_tmp8->setStyleSheet ("background-color:green;color:black;");

        ui->label_impedance1->setStyleSheet ("background-color:gray;color:black;");
        ui->label_impedance2->setStyleSheet ("background-color:gray;color:black;");
        ui->label_impedance3->setStyleSheet ("background-color:gray;color:black;");
        ui->label_impedance4->setStyleSheet ("background-color:gray;color:black;");

        ui->label_gasvalue->setStyleSheet ("background-color:white;color:black;");

        ui->pushButton_td1->setText("1");
        ui->pushButton_td2->setText("2");
        ui->pushButton_td3->setText("3");
        ui->pushButton_td4->setText("4");

        ui->pushButton_td1->setGeometry(710,230,40,20);
        ui->pushButton_td2->setGeometry(740,255,40,20);
        ui->pushButton_td3->setGeometry(710,280,40,20);
        ui->pushButton_td4->setGeometry(680,255,40,20);


        ui->pushButton_waitmode->setGeometry(800,650,100,30);
        ui->pushButton_waitmode->setText(QString::fromUtf8("待机模式"));

        ui->pushButton_calibrate->setGeometry(800,600,100,30);
        ui->pushButton_calibrate->setText(QString::fromUtf8("触摸屏校准"));

        ui->label_tagetTmp->setGeometry(680,75,100,20);
        ui->label_tagetTmp->setText(QString::fromUtf8("目标温度"));

        ui->label_Max->setGeometry(680,140,100,20);
        ui->label_Max->setText(QString::fromUtf8("最大功率"));



        m_QComboBox_cureperiod->setGeometry(695,34,100,30);

        m_QComboBox_targettemp->setGeometry(695,99,100,30);

        m_QComboBox_maxpower->setGeometry(695,164,100,30);

        m_QComboBox_curepos->setGeometry(695,344,100,30);


        ui->label_tdxz->setGeometry(680,200,80,20);
        ui->label_tdxz->setText(QString::fromUtf8("通道选择"));

        ui->label_zlwz->setGeometry(680,320,100,20);
        ui->label_zlwz->setText(QString::fromUtf8("治疗位置(cm)"));

        ui->label_zlzq->setGeometry(680,10,100,20);
        ui->label_zlzq->setText(QString::fromUtf8("治疗周期"));



        ui->label_devidestatus->setGeometry(0,740,200,20);
        ui->label_devidestatus->setText(QString::fromUtf8("设备正常"));

        ui->pushButton_addgas->setGeometry(40,120,60,30);
        ui->pushButton_losegas->setGeometry(110,120,60,30);
        ui->label_gasvalue->setGeometry(80,80,40,20);

        ui->label_gasvalue->setText("150");
        ui->pushButton_addgas->setText(QString::fromUtf8("充气"));
        ui->pushButton_losegas->setText(QString::fromUtf8("放气"));



        m_QLabel_pic1->setGeometry(205,200,180,180);
        m_QLabel_pic1->setPixmap(QPixmap(RES_POS"step3.jpg"));
        m_QLabel_pic1->setScaledContents(true) ;
        //m_QLabel_pic1->setVisible(0);

        m_QLeftcureTime->setGeometry(700,500,180,30);



        progressBar1->move(20,200);
        progressBar1->setOrientation(Qt::Vertical);
        progressBar1->setStyleSheet("QProgressBar:vertical{"
                                        "border-radius:5px;"
                                        "background-color:darkgray;"
                                        "text-align:center"
                                        "}"
                                      "QProgressBar::chunk:vertical{"
                                        "background-color:#00FF00;"
                                        "margin:1px;"
                                        //"height:2px;"
                                        "}");
        progressBar1->setValue(0);
        progressBar1->setRange(0,100);
        progressBar1->setMinimumSize(0,100);

        progressBar2->move(70,200);
        progressBar2->setOrientation(Qt::Vertical);
        progressBar2->setStyleSheet("QProgressBar:vertical{"
                                        "border-radius:5px;"
                                        "background-color:darkgray;"
                                        "text-align:center"
                                        "}"
                                      "QProgressBar::chunk:vertical{"
                                        "background-color:#00FF00;"
                                        "margin:1px;"
                                        //"height:2px;"
                                        "}");
        progressBar2->setValue(0);
        progressBar2->setRange(0,100);
        progressBar2->setMinimumSize(0,100);

        progressBar3->move(120,200);
        progressBar3->setOrientation(Qt::Vertical);
        progressBar3->setStyleSheet("QProgressBar:vertical{"
                                        "border-radius:5px;"
                                        "background-color:darkgray;"
                                        "text-align:center"
                                        "}"
                                      "QProgressBar::chunk:vertical{"
                                        "background-color:#00FF00;"
                                        "margin:1px;"
                                        //"height:2px;"
                                        "}");
        progressBar3->setValue(0);
        progressBar3->setRange(0,100);
        progressBar3->setMinimumSize(0,100);

        progressBar4->move(170,200);
        progressBar4->setOrientation(Qt::Vertical);
        progressBar4->setStyleSheet("QProgressBar:vertical{"
                                        "border-radius:5px;"
                                        "background-color:darkgray;"
                                        "text-align:center"
                                        "}"
                                      "QProgressBar::chunk:vertical{"
                                        "background-color:#00FF00;"
                                        "margin:1px;"
                                        //"height:2px;"
                                        "}");
        progressBar4->setValue(0);
        progressBar4->setRange(0,100);
        progressBar4->setMinimumSize(0,100);

       progressBar1->setVisible(0);
       progressBar2->setVisible(0);
       progressBar3->setVisible(0);
       progressBar4->setVisible(0);



       ui->label_showcam->setGeometry(400,400,320,240);
       ui->label_showcam->setScaledContents(true);
       ui->label_showcam->setVisible(0);
       ui->label_showcam->setText("");
       ui->pushButton_showcam->setGeometry(500,340,80,35);
       ui->pushButton_showcam->setText(QString::fromUtf8("摄像头"));

       ui->pushButton_takephoto->setGeometry(450,660,80,35);
       ui->pushButton_takephoto->setText(QString::fromUtf8("拍照"));
       ui->pushButton_closecam->setGeometry(550,660,80,35);
       ui->pushButton_closecam->setText(QString::fromUtf8("关闭"));


#if 0
       QPixmap pixmap(QString::fromUtf8(RES_POS"step2.jpg"));//当前文件夹下面的图片
       QPalette palette = this->palette();
       palette.setBrush(backgroundRole(),QBrush(pixmap.scaled(// 缩放背景图.
                                                              QSize(800,600),
                                                                     Qt::IgnoreAspectRatio,
                                                                     Qt::SmoothTransformation)));
       setPalette(palette);
#endif


       m_pQTimer_detectkey->start(10);

        m_pQTimer_showtime->start(1000);

        m_pQTimer_showtmp->start(1000);
#if 1
        pthread_t tid;
        int ret;
        ret = pthread_create(&tid,NULL,GetCurrTmp,NULL);
        if(ret != 0)
        {
            printf("create GetCurrTmp pthread failed!\n");
        }
#endif
#if 0
        pthread_t sel_send_tid;
        int ret1;
        ret1 = pthread_create(&sel_send_tid,NULL,sel_send_func,NULL);
        if(ret1 != 0)
        {
            printf("create sel_send_func pthread failed!\n");
        }
#endif
#if 1
        pthread_t detect_key_tid;
        int ret2;
        ret2 = pthread_create(&detect_key_tid,NULL,detect_key_func,NULL);
        if(ret2 != 0)
        {
            printf("create detect_key_func pthread failed!\n");
        }
#endif
}

mdmedical::~mdmedical()
{
    pthread_mutex_destroy(&g_mutex); //销毁互斥锁
    delete ui;
}

/*初始化治疗周期下拉框的时间显示条目*/
void mdmedical::InitCureReriodSubWidgets()
{
    m_QComboBox_cureperiod->addItem(QWidget::tr("1:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("1:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("1:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("1:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("1:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("1:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("2:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("2:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("2:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("2:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("2:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("2:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("3:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("3:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("3:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("3:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("3:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("3:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("4:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("4:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("4:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("4:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("4:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("4:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("5:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("5:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("5:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("5:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("5:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("5:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("6:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("6:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("6:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("6:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("6:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("6:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("7:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("7:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("7:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("7:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("7:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("7:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("8:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("8:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("8:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("8:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("8:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("8:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("9:00"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("9:10"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("9:20"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("9:30"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("9:40"));
    m_QComboBox_cureperiod->addItem(QWidget::tr("9:50"));

    m_QComboBox_cureperiod->addItem(QWidget::tr("10:00"));

    //m_QComboBox_cureperiod->setEditable(true);
//    m_QComboBox_cureperiod->setMaxVisibleItems(10);
//     m_QComboBox_cureperiod->insertItem(55, tr("Insert item"));
//     m_QComboBox_cureperiod->insertSeparator(55);

}
/*初始化目标温度下拉框的温度显示条目*/
void mdmedical::InitTargetTempSubWidgets()
{
    m_QComboBox_targettemp->addItem(QWidget::tr("50"));
    m_QComboBox_targettemp->addItem(QWidget::tr("51"));
    m_QComboBox_targettemp->addItem(QWidget::tr("52"));
    m_QComboBox_targettemp->addItem(QWidget::tr("53"));
    m_QComboBox_targettemp->addItem(QWidget::tr("54"));
    m_QComboBox_targettemp->addItem(QWidget::tr("55"));
    m_QComboBox_targettemp->addItem(QWidget::tr("56"));
    m_QComboBox_targettemp->addItem(QWidget::tr("57"));
    m_QComboBox_targettemp->addItem(QWidget::tr("58"));
    m_QComboBox_targettemp->addItem(QWidget::tr("59"));

    m_QComboBox_targettemp->addItem(QWidget::tr("60"));
    m_QComboBox_targettemp->addItem(QWidget::tr("61"));
    m_QComboBox_targettemp->addItem(QWidget::tr("62"));
    m_QComboBox_targettemp->addItem(QWidget::tr("63"));
    m_QComboBox_targettemp->addItem(QWidget::tr("64"));
    m_QComboBox_targettemp->addItem(QWidget::tr("65"));
    m_QComboBox_targettemp->addItem(QWidget::tr("66"));
    m_QComboBox_targettemp->addItem(QWidget::tr("67"));
    m_QComboBox_targettemp->addItem(QWidget::tr("68"));
    m_QComboBox_targettemp->addItem(QWidget::tr("69"));

    m_QComboBox_targettemp->addItem(QWidget::tr("70"));
    m_QComboBox_targettemp->addItem(QWidget::tr("71"));
    m_QComboBox_targettemp->addItem(QWidget::tr("72"));
    m_QComboBox_targettemp->addItem(QWidget::tr("73"));
    m_QComboBox_targettemp->addItem(QWidget::tr("74"));
    m_QComboBox_targettemp->addItem(QWidget::tr("75"));
    m_QComboBox_targettemp->addItem(QWidget::tr("76"));
    m_QComboBox_targettemp->addItem(QWidget::tr("77"));
    m_QComboBox_targettemp->addItem(QWidget::tr("78"));
    m_QComboBox_targettemp->addItem(QWidget::tr("79"));

    m_QComboBox_targettemp->addItem(QWidget::tr("80"));
    m_QComboBox_targettemp->addItem(QWidget::tr("81"));
    m_QComboBox_targettemp->addItem(QWidget::tr("82"));
    m_QComboBox_targettemp->addItem(QWidget::tr("83"));
    m_QComboBox_targettemp->addItem(QWidget::tr("84"));
    m_QComboBox_targettemp->addItem(QWidget::tr("85"));
    m_QComboBox_targettemp->addItem(QWidget::tr("86"));
    m_QComboBox_targettemp->addItem(QWidget::tr("87"));
    m_QComboBox_targettemp->addItem(QWidget::tr("88"));
    m_QComboBox_targettemp->addItem(QWidget::tr("89"));

    m_QComboBox_targettemp->addItem(QWidget::tr("90"));
    m_QComboBox_targettemp->addItem(QWidget::tr("91"));
    m_QComboBox_targettemp->addItem(QWidget::tr("92"));
    m_QComboBox_targettemp->addItem(QWidget::tr("93"));
    m_QComboBox_targettemp->addItem(QWidget::tr("94"));
    m_QComboBox_targettemp->addItem(QWidget::tr("95"));

    //m_QComboBox_targettemp->setEditable(true);
//    m_QComboBox_targettemp->setMaxVisibleItems(10);
//     m_QComboBox_targettemp->insertItem(46, tr("Insert item"));
//     m_QComboBox_targettemp->insertSeparator(46);
}
/*初始化最大功率下拉框的功率显示条目*/
void mdmedical::InitMaxPowerSubWidgets()
{
    m_QComboBox_maxpower->addItem(QWidget::tr("0.1"));
    m_QComboBox_maxpower->addItem(QWidget::tr("0.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("1.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("1.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("2.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("2.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("3.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("3.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("4.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("4.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("5.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("5.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("6.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("6.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("7.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("7.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("8.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("8.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("9.0"));
    m_QComboBox_maxpower->addItem(QWidget::tr("9.5"));
    m_QComboBox_maxpower->addItem(QWidget::tr("10.0"));

    //m_QComboBox_maxpower->setEditable(true);
//    m_QComboBox_maxpower->setMaxVisibleItems(10);
//     m_QComboBox_maxpower->insertItem(21, tr("Insert item"));
//     m_QComboBox_maxpower->insertSeparator(21);

}

/*初始化治疗位置下拉框的位置显示条目*/
void mdmedical::InitCurePosSubWidgets()
{
    m_QComboBox_curepos->addItem(QWidget::tr("25.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("25.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("26.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("26.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("27.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("27.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("28.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("28.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("29.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("29.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("30.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("30.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("31.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("31.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("32.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("32.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("33.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("33.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("34.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("34.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("35.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("35.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("36.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("36.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("37.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("37.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("38.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("38.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("39.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("39.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("40.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("40.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("41.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("41.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("42.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("42.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("43.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("43.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("44.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("44.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("45.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("45.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("46.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("46.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("47.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("47.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("48.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("48.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("49.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("49.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("50.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("50.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("51.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("51.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("52.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("52.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("53.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("53.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("54.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("54.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("55.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("55.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("56.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("56.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("57.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("57.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("58.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("58.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("59.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("59.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("60.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("60.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("61.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("61.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("62.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("62.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("63.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("63.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("64.0"));
    m_QComboBox_curepos->addItem(QWidget::tr("64.5"));
    m_QComboBox_curepos->addItem(QWidget::tr("65.0"));

    //m_QComboBox_curepos->setEditable(true);
//    m_QComboBox_curepos->setMaxVisibleItems(10);
//     m_QComboBox_curepos->insertItem(81, tr("Insert item"));
//     m_QComboBox_curepos->insertSeparator(81);
}


void mdmedical::ShowTimeCurrent(void)
{

    QTime qtimeObj = QTime::currentTime();
    QDate qdateObj = QDate::currentDate();
    QString strtime = qtimeObj.toString("hh:mm:ss");
    QString strDate = qdateObj.toString("yyyy-MM-dd"); //星期、月份、天、年
    strDate.append(" ");
    strDate.append(strtime);
    ui->label_showcurtime->setText(strDate);
}

/*显示温度*/
void mdmedical::ShowCurrentTmp(void)
{
    {
        QString data1 = QString("%1").arg(currTmp[0]);
        QString data2 = QString("%1").arg(currTmp[1]);
        QString data3 = QString("%1").arg(currTmp[2]);
        QString data4 = QString("%1").arg(currTmp[3]);
        QString data5 = QString("%1").arg(currTmp[4]);
        QString data6 = QString("%1").arg(currTmp[5]);
        QString data7 = QString("%1").arg(currTmp[6]);
        QString data8 = QString("%1").arg(currTmp[7]);
        ui->label_tmp1->setText(data1.mid(0,4));
        ui->label_tmp2->setText(data3.mid(0,4));
        ui->label_tmp3->setText(data5.mid(0,4));
        ui->label_tmp4->setText(data7.mid(0,4));
        ui->label_tmp5->setText(data2.mid(0,4));
        ui->label_tmp6->setText(data4.mid(0,4));
        ui->label_tmp7->setText(data6.mid(0,4));
        ui->label_tmp8->setText(data8.mid(0,4));
    }

}

/*显示阻抗*/
void mdmedical::showCurrentImpedance(void)
{
    ui->label_impedance1->setText(QString::number(impedance[0],10));
    ui->label_impedance2->setText(QString::number(impedance[1],10));
    ui->label_impedance3->setText(QString::number(impedance[2],10));
    ui->label_impedance4->setText(QString::number(impedance[3],10));

}

void mdmedical::ChangePrepareStatus()
{
    g_bPrerareMode = true;
}

/*治疗模式下绘制温度曲线图与控制功率的操作*/
void mdmedical::makeGraph()
{
    static int count = 0;
    int i;

    if(closegraph %2 == 0)
        ShowLeftCureTime();



    if((closegraph++) ==  (originalInfo.cureCycle*10+60)*2 || g_bFootKeyHide)
    {
        if(m_pQTimer_showgraph->isActive())
            m_pQTimer_showgraph->stop();
//        if(m_pQTimer_showpowerrate1->isActive())
//            m_pQTimer_showpowerrate1->stop();
        if(m_pQTimer_writedata->isActive())
            m_pQTimer_writedata->stop();
        oFile.close();
        progressBar1->setVisible(0);
        progressBar2->setVisible(0);
        progressBar3->setVisible(0);
        progressBar4->setVisible(0);

        count = 0;
        closegraph = 0;
        g_bFootKeyHide = false;
        g_bCureClosed = true;
        OriginalPosition();
        m_QLeftcureTime->setVisible(0);
        widget->setVisible(0);

        m_QLabel_pic1->setGeometry(205,200,180,180);
        m_QLabel_pic1->setPixmap(QPixmap(RES_POS"step3.jpg"));
    }



    temp[closegraph] = ReturnMaxValue(currTmp[0],currTmp[2],currTmp[4],currTmp[6]);
    temp1[closegraph] = ReturnMinValue(currTmp[0],currTmp[2],currTmp[4],currTmp[6]);
    temp2[closegraph] = ReturnMaxValue(currTmp[1],currTmp[3],currTmp[5],currTmp[7]);
    temp3[closegraph] = ReturnMinValue(currTmp[1],currTmp[3],currTmp[5],currTmp[7]);

    if(closegraph > 1)
    {
#if 0
        if(temp[closegraph] < originalInfo.targetTmp &&( (currTmp[0]-lasttmp1)<5.0 || (currTmp[2]-lasttmp2)<5.0 || (currTmp[4]-lasttmp3)<5.0 || (currTmp[6]-lasttmp4)<5.0 &&(limit_count<4)))
        {
            g_bPwmiMode = true;
            limit_count++;
            printf("PWM increase %d  %f\n",originalInfo.targetTmp,temp[closegraph]);
        }
        else if(temp[closegraph] < originalInfo.targetTmp && ((currTmp[0]-lasttmp1)>5.0 || (currTmp[2]-lasttmp2)>5.0 || (currTmp[4]-lasttmp3)>5.0 || (currTmp[6]-lasttmp4)>5.0))
        {
            g_bPwmdMode = true;
            printf("%d  %f\n",originalInfo.targetTmp,temp[closegraph]);
        }
        else if(temp[closegraph] > originalInfo.targetTmp)
        {
            g_bPwmdMode = true;
            printf("PWM keep  %d  %f\n",originalInfo.targetTmp,temp[closegraph]);
        }
#endif

        for(i=0;i<4;i++)
        {
            if(currTmp[i] < originalInfo.targetTmp/* &&( (currTmp[0]-lasttmp1)<5.0 || (currTmp[2]-lasttmp2)<5.0 || (currTmp[4]-lasttmp3)<5.0 || (currTmp[6]-lasttmp4)<5.0 &&(limit_count<4))*/)
            {
                port[i] = true;
                g_bPwmiMode = true;
                limit_count++;
                //printf("PWM increase %d  %f\n",originalInfo.targetTmp,temp[closegraph]);
            }
            else if(temp[closegraph] < originalInfo.targetTmp /*&& ((currTmp[0]-lasttmp1)>5.0 || (currTmp[2]-lasttmp2)>5.0 || (currTmp[4]-lasttmp3)>5.0 || (currTmp[6]-lasttmp4)>5.0)*/)
            {
                port[i] = true;
                g_bPwmdMode = true;
                //printf("%d  %f\n",originalInfo.targetTmp,temp[closegraph]);
            }
            else if(temp[closegraph] > originalInfo.targetTmp)
            {
                port[i] = true;
                g_bPwmdMode = true;
                //printf("PWM keep  %d  %f\n",originalInfo.targetTmp,temp[closegraph]);
            }
        }


    }




    lasttmp1 = currTmp[0];
    lasttmp2 = currTmp[2];
    lasttmp3 = currTmp[4];
    lasttmp4 = currTmp[6];

    if(count%2 == 0)
    {
        QVector<double> x(1200),y0(1200),y1(1200),y2(1200),y3(1200);
        for(int i=0;i<closegraph+1;i++)
        {

                x[i]=i/2+1;
                y0[i]=temp[i];
                y1[i]=temp1[i];
                y2[i]=temp2[i];
                y3[i]=temp3[i];
        }
        widget->graph(0)->setData(x,y0);
//        widget->graph(1)->setData(x,y1);
//        widget->graph(2)->setData(x,y2);
//        widget->graph(3)->setData(x,y3);

        widget->replot();
    }
    count++;
}


void mdmedical::HideAllWidget()
{
    //ui->label_showcurtime->setVisible(0);
    ui->label_tmp1->setVisible(0);
    ui->label_tmp2->setVisible(0);
    ui->label_tmp3->setVisible(0);
    ui->label_tmp4->setVisible(0);
    ui->label_tmp5->setVisible(0);
    ui->label_tmp6->setVisible(0);
    ui->label_tmp7->setVisible(0);
    ui->label_tmp8->setVisible(0);
    //ui->label_tagetTmp->setVisible(0);
    ui->label_Max->setVisible(0);
    ui->label_tdxz->setVisible(0);
    ui->label_zlwz->setVisible(0);
    ui->pushButton_td1->setVisible(0);
    ui->pushButton_td2->setVisible(0);
    ui->pushButton_td3->setVisible(0);
    ui->pushButton_td4->setVisible(0);
    ui->label_zlzq->setVisible(0);
    ui->label_devidestatus->setVisible(0);
    ui->label_impedance1->setVisible(0);
    ui->label_impedance2->setVisible(0);
    ui->label_impedance3->setVisible(0);
    ui->label_impedance4->setVisible(0);
    ui->pushButton_addgas->setVisible(0);
    ui->pushButton_losegas->setVisible(0);
    ui->label_gasvalue->setVisible(0);
    //ui->menuBar->setVisible(0);
    //ui->mainToolBar->setVisible(0);
}

void mdmedical::HShowAllWidget()
{
    //ui->label_showcurtime->setVisible(0);
    ui->label_tmp1->setVisible(1);
    ui->label_tmp2->setVisible(1);
    ui->label_tmp3->setVisible(1);
    ui->label_tmp4->setVisible(1);
    ui->label_tmp5->setVisible(1);
    ui->label_tmp6->setVisible(1);
    ui->label_tmp7->setVisible(1);
    ui->label_tmp8->setVisible(1);
    ui->label_tagetTmp->setVisible(1);
    ui->label_Max->setVisible(01);
    ui->label_tdxz->setVisible(1);
    ui->label_zlwz->setVisible(1);
    ui->pushButton_td1->setVisible(1);
    ui->pushButton_td2->setVisible(1);
    ui->pushButton_td3->setVisible(1);
    ui->pushButton_td4->setVisible(1);
    ui->label_zlzq->setVisible(1);
    ui->label_devidestatus->setVisible(1);
    ui->label_impedance1->setVisible(1);
    ui->label_impedance2->setVisible(1);
    ui->label_impedance3->setVisible(1);
    ui->label_impedance4->setVisible(1);
    ui->pushButton_addgas->setVisible(1);
    ui->pushButton_losegas->setVisible(1);
    ui->label_gasvalue->setVisible(1);
    //ui->menuBar->setVisible(1);
    //ui->mainToolBar->setVisible(1);
}



void mdmedical::ItemNewPosition()
{
    ui->label_tmp1->setGeometry(470,10,45,20);
    ui->label_tmp2->setGeometry(620,152,45,20);
    ui->label_tmp3->setGeometry(470,250,45,20);
    ui->label_tmp4->setGeometry(370,152,45,20);

    ui->label_tmp5->setGeometry(470,73,45,20);
    ui->label_tmp6->setGeometry(560,140,45,20);
    ui->label_tmp7->setGeometry(470,310,45,20);
    ui->label_tmp8->setGeometry(320,140,45,20);

    ui->label_impedance1->setGeometry(470,40,45,20);
    ui->label_impedance2->setGeometry(560,165,45,20);
    ui->label_impedance3->setGeometry(470,280,45,20);
    ui->label_impedance4->setGeometry(320,165,45,20);

    progressBar1->setVisible(1);
    progressBar2->setVisible(1);
    progressBar3->setVisible(1);
    progressBar4->setVisible(1);

    m_QLeftcureTime->setVisible(1);
}

void mdmedical::OriginalPosition()
{
    ui->label_tmp1->setGeometry(280,100,45,20);
    ui->label_tmp2->setGeometry(454,278,45,20);
    ui->label_tmp3->setGeometry(280,400,45,20);
    ui->label_tmp4->setGeometry(155,278,45,20);

    ui->label_tmp5->setGeometry(280,163,45,20);
    ui->label_tmp6->setGeometry(400,267,45,20);
    ui->label_tmp7->setGeometry(280,460,45,20);
    ui->label_tmp8->setGeometry(100,267,45,20);

    ui->label_impedance1->setGeometry(280,130,45,20);
    ui->label_impedance2->setGeometry(400,292,45,20);
    ui->label_impedance3->setGeometry(280,430,45,20);
    ui->label_impedance4->setGeometry(100,292,45,20);

}

/*获得当前治疗周期时间*/
void mdmedical::GetCureCycleCurrentValue()
{
    originalInfo.cureCycle = m_QComboBox_cureperiod->currentIndex();
    m_minute = originalInfo.cureCycle / 6 + 1;
    printf("sssssssssss  %d\n",m_minute);
    m_second = originalInfo.cureCycle % 6 *10;

}
/*获得当前目标温度*/
void mdmedical::GetTargetTmpCurrentValue()
{
    originalInfo.targetTmp = m_QComboBox_targettemp->currentIndex() + 50;
}

/*获得当前功率值*/
void mdmedical::GetMaxPowerCurrentValue()
{
    originalInfo.maxPower = m_QComboBox_maxpower->currentIndex();
}

/*获得当前治疗位置值*/
void mdmedical::GetCurePosCurrentValue()
{
    originalInfo.curePos = m_QComboBox_curepos->currentIndex();
}

void mdmedical::GetChanel1Value()
{
    originalInfo.chanel = 1;
}
void mdmedical::GetChanel2Value()
{
    originalInfo.chanel = 2;
}
void mdmedical::GetChanel3Value()
{
    originalInfo.chanel = 3;
}
void mdmedical::GetChanel4Value()
{
    originalInfo.chanel = 4;
}


/*等待模式按钮槽函数*/
void mdmedical::on_pushButton_waitmode_clicked()
{
    g_bPrerareMode = false;
    if(m_pQTimer_showimpedance->isActive())
    {
        m_pQTimer_showimpedance->stop();
    }
    if(m_pQTimer_preparestatus->isActive())
    {
        m_pQTimer_preparestatus->stop();
    }

    ui->label_impedance1->setText("");
    ui->label_impedance2->setText("");
    ui->label_impedance3->setText("");
    ui->label_impedance4->setText("");

}

/*不断检测是否有按键按下*/
void mdmedical::DetectKey()
{
    if(g_bPrepareKey)
    {
        g_bIdleMode = true;
        g_bPrerareMode = true;
        m_pQTimer_showimpedance->start(1000);
        m_pQTimer_preparestatus->start(1000);
        g_bPrepareKey = false;
    }
    if(g_bFootKey && (!m_pQTimer_showgraph->isActive()))
    {
        m_pQTimer_showgraph->start(500);
        m_pQTimer_showpowerrate1->start(1000);
        m_pQTimer_writedata->start(1000);
        ItemNewPosition();
        indexpower = originalInfo.maxPower;

        originalInfo.cureCycle = m_QComboBox_cureperiod->currentIndex();
        m_minute = originalInfo.cureCycle / 6 + 1;
        printf("sssssssssss  %d\n",m_minute);
        m_second = originalInfo.cureCycle % 6 *10;

        m_bIsfootkeyPress = true;
        m_QLabel_pic1->setGeometry(430,110,130,130);
        m_QLabel_pic1->setPixmap(QPixmap(RES_POS"step4.jpg"));

        g_bCureMode = true;

        widget->setVisible(1);
        g_bFootKey = false;

        //打开要输出的文件
        oFile.open("scoresheet.csv", ios::out | ios::trunc);    // 这样就很容易的输出一个需要的excel 文件
        oFile << "温度" << "," << "阻抗" << "," << "功率" << endl;

    }
    else if(g_bFootKey && m_pQTimer_showgraph->isActive())
    {
        g_bFootKeyHide = true;
        g_bFootKey = false;

        m_pQTimer_writedata->stop();
        oFile.close();
    }
}

/*触摸屏校验按钮按下超过五次，就触发触摸屏校验程序*/
void mdmedical::on_pushButton_calibrate_clicked()
{
    if((m_calibate_count++) > 5)
    {
        this->hide();  //隐藏主界面
        m_pQTimer_detectcalibrate->start(1000);
        m_calibate_count = 0;
        if(m_QProcess_calibration->isOpen())
            m_QProcess_calibration->close();
        m_QProcess_calibration->start("ts_calibrate");
    }
}
/*检测触摸屏校验程序是否执行完成，完成的情况下显示软件的整体界面*/
void mdmedical::DetectCalibrate()
{
    if(m_QProcess_calibration->state() == 0)
    {
        this->show();  //显示软件的整体界面
        if(m_pQTimer_detectcalibrate->isActive())
            m_pQTimer_detectcalibrate->stop();
    }
}

/*直方条显示功率的百分比*/
void mdmedical::ShowPowerRate()
{
    float f = (float)(sel1.powerrate[0]-0x30) + ((float)(sel1.powerrate[2]-0x30)/10);
    progressBar1->setValue((int)(f*10));
    f = (float)(sel2.powerrate[0]-0x30) + ((float)(sel2.powerrate[2]-0x30)/10);
    progressBar2->setValue((int)(f*10));
    f = (float)(sel4.powerrate[0]-0x30) + ((float)(sel4.powerrate[2]-0x30)/10);
    progressBar3->setValue((int)(f*10));
    f = (float)(sel5.powerrate[0]-0x30) + ((float)(sel5.powerrate[2]-0x30)/10);
    progressBar4->setValue((int)(f*10));
}
/*治疗模式启动后，不断显示剩下的治疗时间*/
void mdmedical::ShowLeftCureTime(void)
{
        if(m_second == 0)
        {
            if(m_minute != 0)
            {
                m_minute--;
                m_second = 60;
            }
        }
        m_second--;
        time = "剩余治疗时间："  + QString::number(m_minute, 10) + ":" + QString::number(m_second, 10);
        m_QLeftcureTime->setText(time);

}

void mdmedical::on_pushButton_showcam_clicked()
{
    ope.Open("/dev/video0");
    usleep(100*1000);
    m_pQTimer_showcam->start(20);
    ui->label_showcam->setVisible(1);
}

/*显示摄像头画面*/
void mdmedical::ShowCam()
{

    ope.Read();
    jpeg_buf = (unsigned char *)malloc(ope.df.real_len);
    memcpy(jpeg_buf, ope.df.data, ope.df.real_len);
    image.loadFromData(jpeg_buf,ope.df.real_len,"jpg");
    if(istakephoto)  //是否拍照标志
    {
        istakephoto = false;
        image.save("picture.jpg");  //拍照
    }
    ui->label_showcam->setPixmap(QPixmap::fromImage(image));
    ui->label_showcam->show();
    free(jpeg_buf);

    if(isclosecam)
    {
        isclosecam = false;
        m_pQTimer_showcam->stop();
        //ui->label_showcam->setPixmap(NULL);
        ui->label_showcam->setVisible(0);
        ope.Free();
    }

}


void mdmedical::WriteData()
{
    printf("222222222222222\n");

    float f = (float)(sel1.powerrate[0]-0x30) + ((float)(sel1.powerrate[2]-0x30)/10);
    oFile << currTmp[0] << "," << impedance[0] << ","<<f<<endl;
    f = (float)(sel2.powerrate[0]-0x30) + ((float)(sel2.powerrate[2]-0x30)/10);
    oFile << currTmp[2] << "," << impedance[1] << ","<<f<< endl;
    f = (float)(sel4.powerrate[0]-0x30) + ((float)(sel4.powerrate[2]-0x30)/10);
    oFile << currTmp[4] << "," << impedance[2] << ","<<f<< endl;
    f = (float)(sel5.powerrate[0]-0x30) + ((float)(sel5.powerrate[2]-0x30)/10);
    oFile << currTmp[6] << "," << impedance[3] << ","<<f<< endl;
}



































/*
void *sel_recv_func(void *args)
{
    struct timeval tv;
    int i=0,j=0;
    fd_set rdfds;
    int maxfd = -1;
    int ret;
    char buff[512];
    int len;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    fflush(stdout);

    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond,&mutex);
    pthread_mutex_unlock(&mutex);
    while(1)
    {
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        FD_ZERO(&rdfds);
        FD_SET(0,&rdfds);
        maxfd = maxfd>0?maxfd:0;
        FD_SET(sel1.m_fd, &rdfds);
        FD_SET(sel2.m_fd, &rdfds);
        FD_SET(sel4.m_fd, &rdfds);
        FD_SET(sel5.m_fd, &rdfds);
        maxfd = maxfd > sel1.m_fd ? maxfd:sel1.m_fd;
        maxfd = maxfd > sel2.m_fd ? maxfd:sel2.m_fd;
        maxfd = maxfd > sel4.m_fd ? maxfd:sel4.m_fd;
        maxfd = maxfd > sel5.m_fd ? maxfd:sel5.m_fd;
        ret = select(maxfd + 1, &rdfds, NULL, NULL, &tv);
        if(ret < 0)
        {
                perror("Select fail\n");
        }
        else if(ret == 0)
        {
            printf("timeout!\n");
            continue;
        }
        else
        {
            if(FD_ISSET(sel1.m_fd, &rdfds))
            {
                memset(buff, 0, 512);
                i = 0;
                while((read(sel1.m_fd,&buff[i++],1) > 0)&&(i<511));
                buff[i] = '\0';

                memcpy(sel1.m_recvdata,buff,i);
                printf("sel1 receve data is :");

                for(j=0;j<i-1;j++)
                    printf("  %x  ",buff[j]);

                printf("\n");
                fflush(stdout);
                sel1.ParsePrepareSendReturnData();

                impedance[0] = sel1.impence;
                printf("the zukang is : %hu\n",sel1.impence);

                printf("powerrate is  %d  %d  %d\n",sel1.powerrate[0],sel1.powerrate[1],sel1.powerrate[2]);

            }
            else if(FD_ISSET(sel2.m_fd, &rdfds))
            {
                memset(buff, 0, 512);
                i = 0;
                while((read(sel2.m_fd,&buff[i++],1) > 0)&&(i<511));
                buff[i] = '\0';
                memcpy(sel2.m_recvdata,buff,i);
                printf("sel2 receve data is :");

               for(j=0;j<i-1;j++)
                   printf("  %x  ",buff[j]);
               printf("\n");
                fflush(stdout);
                sel2.ParsePrepareSendReturnData();

                impedance[1] = sel2.impence;
                printf("the zukang is : %hu\n",sel2.impence);

                printf("powerrate is  %d  %d  %d\n",sel2.powerrate[0],sel2.powerrate[1],sel2.powerrate[2]);

            }
            else if(FD_ISSET(sel4.m_fd, &rdfds))
            {
                memset(buff, 0, 512);
                i = 0;
                while((read(sel4.m_fd,&buff[i++],1) > 0)&&(i<511));
                buff[i] = '\0';
                memcpy(sel4.m_recvdata,buff,i);
                printf("sel4 receve data is :");

               for(j=0;j<i-1;j++)
                   printf(" %x ",buff[j]);
               printf("\n");
                fflush(stdout);
                sel4.ParsePrepareSendReturnData();

                impedance[2] = sel4.impence;
                printf("the zukang is : %hu\n",sel4.impence);

                printf("powerrate is  %d  %d  %d\n",sel4.powerrate[0],sel4.powerrate[1],sel4.powerrate[2]);

            }
            else if(FD_ISSET(sel5.m_fd, &rdfds))
            {
                memset(buff, 0, 512);
                i = 0;
                while((read(sel5.m_fd,&buff[i++],1) > 0)&&(i<511));
                buff[i] = '\0';
                memcpy(sel5.m_recvdata,buff,i);
                printf("sel5 receve data is :");
                for(j=0;j<i-1;j++)
                    printf("  %x  ",buff[j]);
                printf("\n");
                fflush(stdout);

                sel5.ParsePrepareSendReturnData();

                impedance[3] = sel5.impence;
                printf("the zukang is : %hu\n",sel5.impence);

                printf("powerrate is  %d  %d  %d\n",sel5.powerrate[0],sel5.powerrate[1],sel5.powerrate[2]);
            }
        }

    }
}
*/


void mdmedical::on_pushButton_takephoto_clicked()
{
    istakephoto = true;
}

void mdmedical::on_pushButton_closecam_clicked()
{
    isclosecam = true;
}
