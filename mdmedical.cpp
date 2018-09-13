#include "mdmedical.h"
#include "ui_mdmedical.h"



bool port[4] = {false,false,false,false};

float g_fCurrTmp[8];
float showtmp[4];
//bool isTmpFinished = false;
//bool isShowFinished = false;
int g_iIndexpower;

bool g_bWaitCommand = false;

unsigned short impedance[4];

CSerial sel1;
CSerial sel2;
CSerial sel4;
CSerial sel5;

QFile outFile("log.txt");

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
bool g_bisupdatestatus = true;
bool g_bisfinishedupdate = false;
bool g_bupdatestatus = false;
bool g_bisUpdatesuccess = false;
volatile bool g_bIsFinishiIncreasePower = true;
volatile bool g_bIsfinishReadtempture = true;



volatile bool channel_mask[4]={false,false,false,false};

pthread_mutex_t g_mutex;

QVector<double> temp(1200);  //zhengjian max
QVector<double> temp1(1200);  //zhengjian min
QVector<double> temp2(1200);  //nianmo Max
QVector<double> temp3(1200);  //nianmo Min
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/



const char powerindex[21][5]={"0.1","0.5","1.0","1.5","2.0","2.5","3.0","3.5","4.0","4.5","5.0","5.5","6.0","6.5","7.0","7.5","8.0","8.5","9.0","9.5","10.0"};


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


float fliter(unsigned short *array)
{
    int i,j,sum=0;
    unsigned short  tmpval;
    unsigned short ret_array[32]={0};
    int arrlay_en = 16;//sizeof(array)/sizeof(int);
    for(i=0;i<arrlay_en-1;i++)
    {
        for(j=0;j<arrlay_en-1-i;j++)
        {
            if(array[j] >= array[j+1])
            {
                tmpval = array[j];
                array[j] = array[j+1];
                array[j+1] = tmpval;
            }
        }
    }

    for(i=0;i<arrlay_en;i++)
    {
        ret_array[i] = array[i];
    }

    for (i=5;i<13;i++)
    {
        sum += ret_array[i];
    }
    //printf("dddd %d\n",sum/16);
    return ((float)(sum)*124.7/8/4096);
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
    if(!channel_mask[0])
    {
        sel1.IdleSend(0);
        sel1.SendData();
        sel1.RecvData();
        ok1 = sel1.ParseIdleSendReturnData();
    }

    if(!channel_mask[1])
    {
        sel2.IdleSend(0);
        sel2.SendData();
        sel2.RecvData();
        ok2 = sel2.ParsePrepareSendReturnData();
    }

    if(!channel_mask[2])
    {
        sel4.IdleSend(0);
        sel4.SendData();
        sel4.RecvData();
        ok3 = sel4.ParsePrepareSendReturnData();
    }

    if(!channel_mask[3])
    {
        sel5.IdleSend(0);
        sel5.SendData();
        sel5.RecvData();
        ok4 = sel5.ParsePrepareSendReturnData();
    }

    return (ok1||ok2||ok3||ok4);
}

bool cure_send()
{

    bool ok1=false,ok2=false,ok3=false,ok4=false;
    sel1.CureComand(powerindex[g_iIndexpower],3);//治疗模式，发送最大功率
    sel1.SendData();
    sel1.RecvData();
    ok1 = sel1.ParseCureSendReturnData();

    sel2.CureComand(powerindex[g_iIndexpower],3);
    sel2.SendData();
    sel2.RecvData();
    ok2 = sel2.ParseCureSendReturnData();

    sel4.CureComand(powerindex[g_iIndexpower],3);
    sel4.SendData();
    sel4.RecvData();
    ok3 = sel4.ParseCureSendReturnData();

    sel5.CureComand(powerindex[g_iIndexpower],3);
    sel5.SendData();
    sel5.RecvData();
    ok4 = sel5.ParseCureSendReturnData();

    return (ok1&&ok2&&ok3&&ok4);
}

void Prepare_send()
{
    //float f;
    //float f1;
    if(!channel_mask[0])
    {
        sel1.PrepareSend(0);
        sel1.SendData();
        sel1.RecvData();
        sel1.ParsePrepareSendReturnData();
        impedance[0] = sel1.impence;
        sel1.PowerSend(0);
        sel1.SendData();
        sel1.RecvData();
        sel1.ParsePowerSendReturnData();
    }
#if 0
    if(m_bIsfootkeyPress)
    {
        f = (float)(sel1.powerrate[0]-0x30) + ((float)(sel1.powerrate[2]-0x30)/10);
        f1 = atof(powerindex[g_iIndexpower]);
        //printf("  the powerrate is    %.1f    ",f);
    }
#endif

    if(!channel_mask[1])
    {
        sel2.PrepareSend(0);
        sel2.SendData();
        sel2.RecvData();
        sel2.ParsePrepareSendReturnData();
        impedance[1] = sel2.impence;
        sel2.PowerSend(0);
        sel2.SendData();
        sel2.RecvData();
        sel2.ParsePowerSendReturnData();
    }
    #if 0
    if(m_bIsfootkeyPress)
    {
        f = (float)(sel2.powerrate[0]-0x30) + ((float)(sel2.powerrate[2]-0x30)/10);
        f1 = atof(powerindex[g_iIndexpower]);
        //printf("     %.1f    ",f);
    }
    #endif

    if(!channel_mask[2])
    {
        sel4.PrepareSend(0);
        sel4.SendData();
        sel4.RecvData();
        sel4.ParsePrepareSendReturnData();
        impedance[2] = sel4.impence;

        sel4.PowerSend(0);
        sel4.SendData();
        sel4.RecvData();
        sel4.ParsePowerSendReturnData();
    }
    #if 0
    if(m_bIsfootkeyPress)
    {
        f = (float)(sel4.powerrate[0]-0x30) + ((float)(sel4.powerrate[2]-0x30)/10);
        f1 = atof(powerindex[g_iIndexpower]);
        //printf("     %.1f    ",f);
    }
    #endif

    if(!channel_mask[3])
    {
        sel5.PrepareSend(0);
        sel5.SendData();
        sel5.RecvData();
        sel5.ParsePrepareSendReturnData();
        impedance[3] = sel5.impence;

        sel5.PowerSend(0);
        sel5.SendData();
        sel5.RecvData();
        sel5.ParsePowerSendReturnData();
    }
     #if 0
    if(m_bIsfootkeyPress)
    {
        f = (float)(sel5.powerrate[0]-0x30) + ((float)(sel5.powerrate[2]-0x30)/10);
        f1 = atof(powerindex[g_iIndexpower]);
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
    if(!channel_mask[0])
    {
        if(port[0])
        {
            sel1.PwmdSend(0);
            sel1.SendData();
        }
    }

    if(!channel_mask[1])
    {
        if(port[1])
        {
            sel2.PwmdSend(0);
            sel2.SendData();
        }
    }

    if(!channel_mask[2])
    {
        if(port[2])
        {
            sel4.PwmdSend(0);
            sel4.SendData();
        }
    }

    if(!channel_mask[3])
    {
        if(port[3])
        {
            sel5.PwmdSend(0);
            sel5.SendData();
        }
    }

    if(!channel_mask[0])
    {
        if(port[0])
        {
            port[0] = true;
            sel1.RecvData();
            ok1 = sel1.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[1])
    {
        if(port[1])
        {
            port[1] = true;
            sel2.RecvData();
            ok2 = sel2.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[2])
    {
        if(port[2])
        {
            port[2] = true;
            sel4.RecvData();
            ok3 = sel4.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[3])
    {
        if(port[3])
        {
            port[3] = true;
            sel5.RecvData();
            ok4 = sel5.ParseIdleSendReturnData();
        }
    }

    return (ok1&&ok2&&ok3&&ok4);
}
/*发送命令给串口增大功率*/
bool pwmi_send()
{

    bool ok1=false,ok2=false,ok3=false,ok4=false;
    if(!channel_mask[0])
    {
        if(port[0])
        {
            sel1.PwmiSend(0);
            sel1.SendData();
        }
    }

    if(!channel_mask[1])
    {
        if(port[1])
        {
            sel2.PwmiSend(0);
            sel2.SendData();
        }
    }

    if(!channel_mask[2])
    {
        if(port[2])
        {
            sel4.PwmiSend(0);
            sel4.SendData();
        }
    }

    if(!channel_mask[3])
    {
        if(port[3])
        {
            sel5.PwmiSend(0);
            sel5.SendData();
        }
    }

    if(!channel_mask[0])
    {
        if(port[0])
        {
            port[0] = false;
            sel1.RecvData();
            ok1 = sel1.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[1])
    {
        if(port[1])
        {
            port[1] = false;
            sel2.RecvData();
            ok2 = sel2.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[2])
    {
        if(port[2])
        {
            port[2] = false;
            sel4.RecvData();
            ok3 = sel4.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[3])
    {
        if(port[3])
        {
            port[3] = false;
            sel5.RecvData();
            ok4 = sel5.ParseIdleSendReturnData();
        }
    }

    return (ok1&&ok2&&ok3&&ok4);
}
/*发送命令给串口保持功率不变*/
bool pwmkeep_send()
{
    bool ok1=false,ok2=false,ok3=false,ok4=false;

    if(!channel_mask[0])
    {
        if(port[0])
        {
            sel1.PwmKeepSend(0);
            sel1.SendData();
        }
    }

    if(!channel_mask[1])
    {
        if(port[1])
        {
            sel2.PwmKeepSend(0);
            sel2.SendData();
        }
    }

    if(!channel_mask[2])
    {
        if(port[2])
        {
            sel4.PwmKeepSend(0);
            sel4.SendData();
        }
    }

    if(!channel_mask[3])
    {
        if(port[3])
        {
            sel5.PwmKeepSend(0);
            sel5.SendData();
        }
    }

    if(!channel_mask[0])
    {
        if(port[0])
        {
            port[0] = false;
            sel1.RecvData();
            ok1 = sel1.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[1])
    {
        if(port[1])
        {
            port[1] = false;
            sel2.RecvData();
            ok2 = sel2.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[2])
    {
        if(port[2])
        {
            port[2] = false;
            sel4.RecvData();
            ok3 = sel4.ParseIdleSendReturnData();
        }
    }

    if(!channel_mask[3])
    {
        if(port[3])
        {
            port[3] = false;
            sel5.RecvData();
            ok4 = sel5.ParseIdleSendReturnData();
        }
    }

    return (ok1&&ok2&&ok3&&ok4);
}

bool cure_close()
{
    bool ok1=false,ok2=false,ok3=false,ok4=false;

    if(!channel_mask[0])
    {
        sel1.CureClosed(0);
        sel1.SendData();
        sel1.RecvData();
        ok1 = sel1.ParseIdleSendReturnData();
    }

    if(!channel_mask[1])
    {
        sel2.CureClosed(0);
        sel2.SendData();
        sel2.RecvData();
        ok2 = sel2.ParseIdleSendReturnData();
    }


    if(!channel_mask[2])
    {
        sel4.CureClosed(0);
        sel4.SendData();
        sel4.RecvData();
        ok3 = sel4.ParseIdleSendReturnData();
    }

    if(!channel_mask[3])
    {
        sel5.CureClosed(0);
        sel5.SendData();
        sel5.RecvData();
        ok4 = sel5.ParseIdleSendReturnData();
    }

    return (ok1&&ok2&&ok3&&ok4);
}


void *GetCurrTmp(void *args)
{
        unsigned int i;
        unsigned short currTmp1[32]={0};
        unsigned short currTmp2[32]={0};
        float tmp1[8]={0},tmp2[8]={0};
        struct timeval start,end;

        int fd = open("/dev/mdmedical_tmp", O_RDWR);
        if(fd < 0)
        {
            printf("open device failed!\n");
        }

        usleep(2*1000);
        while(1)
        {
                    gettimeofday(&start,NULL);
                    for(i=3;i<11;i++)
                    {

                        if(channel_mask[(i-3)/2])
                            continue;

                        ioctl(fd,i,12);

                        read(fd,currTmp1,32);
                        tmp1[i-3] = fliter(currTmp1);
//                        for(k=0;k<16;k++)
//                            printf(" %hu ",currTmp1[k]);
//                            printf("\n");

                    }
                     //printf("\n"); printf("\n"); printf("\n");
                    gettimeofday(&end,NULL);
                    usleep(10*1000-(end.tv_sec*1000000+end.tv_usec-start.tv_usec-start.tv_sec*1000000));
                   //printf("chazhi is %d\n",end.tv_usec-start.tv_usec);

#if 1
                    for(i=3;i<11;i++)
                    {
                        if(channel_mask[(i-3)/2])
                            continue;

                        ioctl(fd,i,12);

                        read(fd,&currTmp2,32);
                        tmp2[i-3] = fliter(currTmp2);

//                        for(k=0;k<16;k++)
//                            printf(" %hu ",currTmp1[k]);
//                            printf("\n");

                    }
                    //printf("\n");printf("\n");printf("*********************************************************************88\n");
#endif

                    g_bIsFinishiIncreasePower = true;
                    for(int j=0;j<8;j++)
                    {
                        g_fCurrTmp[j] = (tmp1[j] + tmp2[j])/2;
                        //g_fCurrTmp[j] = tmp1[j];
                        //if(g_fCurrTmp[j]>80)
                          //printf(" channel  %d   %.1f   %lf  %lf\n ",j,tmp1[j],tmp2[j],g_fCurrTmp[j]);
                    }

 /************************************以上读温度************************************************/
                    usleep(300*1000);
 /************************************以下发送串口命令************************************************/
#if 1
                    if(!g_bupdatestatus)
                    {
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
                            printf("PWM----KKKKKKKKKKKKKKKKK  %f\n",g_fCurrTmp[3]);
                            pwmkeep_send();
                            g_bPwmkeepMode = false;
                        }
                        if(g_bCureClosed)
                        {
                            cure_close();
                            g_bCureClosed = false;
                        }
                #endif
                    }
  #endif                      //usleep(400*1000);
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
char g_version[14];
int g_updateprocess;
bool g_bStartProcessBar = false;
void *update_firmware(void *args)
{

       bool ok1=false,ok2=false,ok3=false,ok4=false;
        const char *filename = "../USMART.bin";
        int filelen;

        filelen = sel1.Get_File_Size(filename);
        if(filelen == -1)
            printf("get file size failed!\n");
            //return ;
        printf("file size is = %d\n",filelen);

        g_updateprocess = filelen / 1024 * 4 + 12;
        g_bStartProcessBar = true;
        unsigned char *buff;
        buff = (unsigned char*)malloc(sizeof(unsigned char)*filelen);
        if(buff == NULL)
        {
            printf("malloc memory failed!\n");
            //return ;
        }

        if(sel1.Read_File(filename,buff,filelen) < 0)
        {
            printf("there is something wrong with reading data from the file!\n");
            //return ;
        }
        sel1.UpdateSend(0);
        sel1.SendData();
        sel1.RecvData();
        if(sel1.ParseUpdateSendReturnData())
        {
            printf("eeeeeeeeeeeeeeeeee\n");
            usleep(30*1000);
            sel1.Send_Byte(0x31);
            sel1.BreakForTransFile();
            if(sel1.Ymodem_Transmit(buff, (const unsigned char *)filename, filelen) == 0)
            {
                sel1.Send_Byte(0x33);
                printf("update success!\n");
                ok1 = true;
            }
            //sel1.GetVersionNumber();
          }

        sel2.UpdateSend(0);
        sel2.SendData();
        sel2.RecvData();
        if(sel2.ParseUpdateSendReturnData())
        {
            usleep(30*1000);
            sel2.Send_Byte(0x31);
            sel2.BreakForTransFile();
            if(sel2.Ymodem_Transmit(buff, (const unsigned char *)filename, filelen) == 0)
            {
                sel2.Send_Byte(0x33);
                printf("update success!\n");
                ok2 = true;
            }
           // sel2.GetVersionNumber();
         }

        sel4.UpdateSend(0);
        sel4.SendData();
        sel4.RecvData();
        if(sel4.ParseUpdateSendReturnData())
        {
             usleep(30*1000);
            sel4.Send_Byte(0x31);
            sel4.BreakForTransFile();
            if(sel4.Ymodem_Transmit(buff, (const unsigned char *)filename, filelen) == 0)
            {
                sel4.Send_Byte(0x33);
                printf("update success!\n");
                ok3 = true;
            }
            //sel4.GetVersionNumber();
        }


        sel5.UpdateSend(0);
        sel5.SendData();
        sel5.RecvData();
        if(sel5.ParseUpdateSendReturnData())
        {
            usleep(30*1000);
            sel5.Send_Byte(0x31);
            sel5.BreakForTransFile();
            if(sel5.Ymodem_Transmit(buff, (const unsigned char *)filename, filelen) == 0)
            {
                sel5.Send_Byte(0x33);
                printf("update success!\n");
                ok4 = true;
            }
            sel5.GetVersionNumber(g_version);
        }
        printf("g_version = %s\n",g_version);
        if(ok1 && ok2 && ok3 && ok4)
            g_bisUpdatesuccess = true;
        free(buff);
        g_bisupdatestatus = true;
}

mdmedical::mdmedical(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdmedical)
{
        ui->setupUi(this);

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

        col = 0;
        row = 0;
        isclosecam = false;
        istakephoto = false;
        m_bIsExistFirmware = false;

        if(outFile.size()>LOG_MAXSIZE)
                outFile.remove();



        outFile.open(QIODevice::WriteOnly | QIODevice::Append);

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


        mymanagerdialog = new Ui_ManagerDialog();
        managerdialog = new QDialog(this);// superFactoryMode;
        mymanagerdialog->setupUi(managerdialog);


        pthread_mutex_init(&g_mutex, NULL);



      sel1.openSerial(SERIAL1,115200);
      sel2.openSerial(SERIAL2,115200);
      sel4.openSerial(SERIAL4,115200);
      sel5.openSerial(SERIAL5,115200);


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

        m_pShowUpdateFinished = new QTimer(this);

        m_pRecordTempture  = new QTimer(this);

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


         connect(mymanagerdialog->pushButton_ts,SIGNAL(clicked()),this,SLOT(TsModify()));
         connect(mymanagerdialog->pushButton_updata,SIGNAL(clicked()),this,SLOT(Updata()));
         connect(mymanagerdialog->pushButton_copy,SIGNAL(clicked()),this,SLOT(CopyUserData()));
         connect(mymanagerdialog->pushButton_exit,SIGNAL(clicked()),this,SLOT(ExitManagerPage()));



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
        connect(m_pShowUpdateFinished, SIGNAL(timeout()), this, SLOT(ShowUpdateStr()));

        connect(m_pRecordTempture, SIGNAL(timeout()), this, SLOT(RecordTempture()));

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
        ui->label_tmp2->setGeometry(280,163,65,20);  //chanel   1  nianmo (454,278,65,20);
        ui->label_tmp3->setGeometry(454,278,65,20);
        ui->label_tmp4->setGeometry(400,267,65,20);

        ui->label_tmp5->setGeometry(280,400,65,20);
        ui->label_tmp6->setGeometry(280,467,65,20);//400,267,65,20
        ui->label_tmp7->setGeometry(100,265,65,20);//280,400,65,20
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


        ui->pushButton_waitmode->setGeometry(900,150,100,30);
        ui->pushButton_waitmode->setText(QString::fromUtf8("待机模式"));

        ui->pushButton_calibrate->setGeometry(900,100,100,30);
        ui->pushButton_calibrate->setText(QString::fromUtf8("管理员模式"));

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





       ui->widget->setGeometry(700,500,324,268);
       m_QPushButton_on_off = new QPushButton(ui->widget);
       m_switch_up = new QPushButton(ui->widget);
       m_switch_down = new QPushButton(ui->widget);
       m_QLabel_streamspeed = new QLabel(ui->widget);

       m_QPushButton_on_off->setText(QString::fromUtf8("开关"));
       m_switch_up->setText(QString::fromUtf8("上调"));
       m_switch_down->setText(QString::fromUtf8("下调"));
       m_QLabel_streamspeed->setText(QString::fromUtf8("流速"));
       m_QLabel_streamspeed->setGeometry(50,10,60,30);
       m_QPushButton_on_off->setGeometry(10,100,60,30);
       m_switch_up->setGeometry(80,100,60,30);
       m_switch_down->setGeometry(150,100,60,30);

        qDebug("This is a debug message");
       //m_pRecordTempture->start(1000);

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

#if 0
        pthread_t tid_update;
        int ret3 = pthread_create(&tid_update,NULL,update_firmware,NULL);
        if(ret3 < 0)
        {
            printf("create pthread failed!\n");
            return ;
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
        if(!channel_mask[0])
        {
            QString data1 = QString("%1").arg(g_fCurrTmp[0]);
            QString data2 = QString("%1").arg(g_fCurrTmp[1]);
            ui->label_tmp1->setText(data1.mid(0,4));
            ui->label_tmp2->setText(data2.mid(0,4));
        }
        else
        {
            ui->label_tmp1->setText("");
            ui->label_tmp2->setText("");
        }
        if(!channel_mask[1])
        {
            QString data3 = QString("%1").arg(g_fCurrTmp[2]);
            QString data4 = QString("%1").arg(g_fCurrTmp[3]);
            ui->label_tmp3->setText(data3.mid(0,4));
            ui->label_tmp4->setText(data4.mid(0,4));
        }
        else
        {
            ui->label_tmp3->setText("");
            ui->label_tmp4->setText("");
        }

        if(!channel_mask[2])
        {
            QString data5 = QString("%1").arg(g_fCurrTmp[4]);
            QString data6 = QString("%1").arg(g_fCurrTmp[5]);
            ui->label_tmp5->setText(data5.mid(0,4));
            ui->label_tmp6->setText(data6.mid(0,4));
        }
        else
        {
            ui->label_tmp5->setText("");
            ui->label_tmp6->setText("");
        }
        if(!channel_mask[3])
        {
            QString data7 = QString("%1").arg(g_fCurrTmp[6]);
            QString data8 = QString("%1").arg(g_fCurrTmp[7]);
            ui->label_tmp7->setText(data7.mid(0,4));
            ui->label_tmp8->setText(data8.mid(0,4));
        }
        else
        {
            printf("gggggggggggggggggg\n");
            ui->label_tmp7->setText("");
            ui->label_tmp8->setText("");
        }
}

/*显示阻抗*/
void mdmedical::showCurrentImpedance(void)
{
    if(!channel_mask[0])
        ui->label_impedance1->setText(QString::number(impedance[0],10));
    else
        ui->label_impedance1->setText("");

    if(!channel_mask[1])
        ui->label_impedance2->setText(QString::number(impedance[1],10));
    else
        ui->label_impedance2->setText("");

    if(!channel_mask[2])
        ui->label_impedance3->setText(QString::number(impedance[2],10));
    else
        ui->label_impedance3->setText("");

    if(!channel_mask[3])
        ui->label_impedance4->setText(QString::number(impedance[3],10));
    else
        ui->label_impedance4->setText("");

}

void mdmedical::ChangePrepareStatus()
{
    g_bPrerareMode = true;
}


void mdmedical::TsModify()
{
    this->managerdialog->hide();
    this->hide();  //隐藏主界面
    m_pQTimer_detectcalibrate->start(1000);
    m_calibate_count = 0;
    if(m_QProcess_calibration->isOpen())
        m_QProcess_calibration->close();
    m_QProcess_calibration->start("ts_calibrate");

}
void mdmedical::Updata()
{
    printf("update firmware start!\n");
    sel1.m_updateprocess = 0;
    sel2.m_updateprocess = 0;
    sel4.m_updateprocess = 0;
    sel5.m_updateprocess = 0;




    if(g_bisupdatestatus && m_bIsExistFirmware)
    {
        g_bisupdatestatus = false;
        pthread_t tid_update;
        int ret = pthread_create(&tid_update,NULL,update_firmware,NULL);
        if(ret < 0)
        {
            printf("create pthread failed!\n");
            return ;
        }
        m_pShowUpdateFinished->start(100);
    }
    else
    {
        QMessageBox::information(this,"error window",QString::fromUtf8("不可更新"));
    }

}
void mdmedical::CopyData()
{

}
void mdmedical::ExitManagerPage()
{
    g_bupdatestatus = false;
    managerdialog->close();
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
        if(m_pQTimer_showpowerrate1->isActive())
            m_pQTimer_showpowerrate1->stop();
        if(m_pQTimer_writedata->isActive())
            m_pQTimer_writedata->stop();
        wb.Dump(datafilename);
        printf("xxxxxxxxxxxx%s\n",datafilename.c_str());
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



    temp[closegraph] = ReturnMaxValue(g_fCurrTmp[0],g_fCurrTmp[2],g_fCurrTmp[4],g_fCurrTmp[6]);
    temp1[closegraph] = ReturnMinValue(g_fCurrTmp[0],g_fCurrTmp[2],g_fCurrTmp[4],g_fCurrTmp[6]);
    temp2[closegraph] = ReturnMaxValue(g_fCurrTmp[1],g_fCurrTmp[3],g_fCurrTmp[5],g_fCurrTmp[7]);
    temp3[closegraph] = ReturnMinValue(g_fCurrTmp[1],g_fCurrTmp[3],g_fCurrTmp[5],g_fCurrTmp[7]);

    if(closegraph > 1)
    {
#if 0
        if(temp[closegraph] < originalInfo.targetTmp &&( (g_fCurrTmp[0]-lasttmp1)<5.0 || (g_fCurrTmp[2]-lasttmp2)<5.0 || (g_fCurrTmp[4]-lasttmp3)<5.0 || (g_fCurrTmp[6]-lasttmp4)<5.0 &&(limit_count<4)))
        {
            g_bPwmiMode = true;
            limit_count++;
            printf("PWM increase %d  %f\n",originalInfo.targetTmp,temp[closegraph]);
        }
        else if(temp[closegraph] < originalInfo.targetTmp && ((g_fCurrTmp[0]-lasttmp1)>5.0 || (g_fCurrTmp[2]-lasttmp2)>5.0 || (g_fCurrTmp[4]-lasttmp3)>5.0 || (g_fCurrTmp[6]-lasttmp4)>5.0))
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
            if(g_fCurrTmp[i] < originalInfo.targetTmp/* &&( (currTmp[0]-lasttmp1)<5.0 || (currTmp[2]-lasttmp2)<5.0 || (currTmp[4]-lasttmp3)<5.0 || (currTmp[6]-lasttmp4)<5.0 &&(limit_count<4))*/)
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




    lasttmp1 = g_fCurrTmp[0];
    lasttmp2 = g_fCurrTmp[2];
    lasttmp3 = g_fCurrTmp[4];
    lasttmp4 = g_fCurrTmp[6];

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
    ui->label_tmp2->setGeometry(470,73,45,20);  //(620,152,45,20);
    ui->label_tmp7->setGeometry(320,140,45,20);
    ui->label_tmp8->setGeometry(370,152,45,20);

    ui->label_tmp5->setGeometry(470,250,45,20);
    ui->label_tmp6->setGeometry(470,310,45,20); //560,140,45,20
    ui->label_tmp4->setGeometry(560,140,45,20);
    ui->label_tmp3->setGeometry(620,152,45,20);  //320,140,45,20

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
    ui->label_tmp1->setGeometry(280,100,65,20);  //chanel   1
    ui->label_tmp2->setGeometry(280,163,65,20);  //chanel   1  nianmo (454,278,65,20);
    ui->label_tmp3->setGeometry(454,278,65,20);
    ui->label_tmp4->setGeometry(400,267,65,20);

    ui->label_tmp5->setGeometry(280,400,65,20);
    ui->label_tmp6->setGeometry(280,467,65,20);//400,267,65,20
    ui->label_tmp7->setGeometry(100,265,65,20);//280,400,65,20
    ui->label_tmp8->setGeometry(155,278,65,20);

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
    if(!channel_mask[0])
        channel_mask[0] = true;
    else
        channel_mask[0] = false;
}
void mdmedical::GetChanel2Value()
{
    originalInfo.chanel = 2;
    if(!channel_mask[1])
        channel_mask[1] = true;
    else
        channel_mask[1] = false;
}
void mdmedical::GetChanel3Value()
{
    originalInfo.chanel = 3;
    if(!channel_mask[2])
        channel_mask[2] = true;
    else
        channel_mask[2] = false;
}
void mdmedical::GetChanel4Value()
{
    originalInfo.chanel = 4;
    if(!channel_mask[3])
        channel_mask[3] = true;
    else
        channel_mask[3] = false;
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
        g_iIndexpower = originalInfo.maxPower;

        originalInfo.cureCycle = m_QComboBox_cureperiod->currentIndex();
        m_minute = originalInfo.cureCycle / 6 + 1;
        m_second = originalInfo.cureCycle % 6 *10;

        m_bIsfootkeyPress = true;
        m_QLabel_pic1->setGeometry(430,110,130,130);
        m_QLabel_pic1->setPixmap(QPixmap(RES_POS"step4.jpg"));

        g_bCureMode = true;

        widget->setVisible(1);
        g_bFootKey = false;

        struct tm *t;
        time_t tt;
        char data[48] = {0};
        time(&tt);
        t = localtime(&tt);
        sprintf(data,"%s%d%c%d%c%d%c%d%c%d%c%d%s","/home/scandata/",t->tm_year + 1900,'-', t->tm_mon + 1,'-', t->tm_mday,'-', t->tm_hour,'-', t->tm_min,'-', t->tm_sec,".xls");
        printf("%s\n",data);
        datafilename = data;

        col = 0;
        row = 2;
        second = 1;
        xf = wb.xformat();
        ws = wb.sheet("sheet1");

        string time = "时间 秒";
            string label = "1st单位：阻抗欧姆";
            string label1 = "1st单位：℃ 针尖";
            string label2 = "1st单位：℃ 粘膜";

            string channel1 = "通道1";
            string channel2 = "通道2";
            string channel3 = "通道3";
            string channel4 = "通道4";


            cell  = ws->label(0,1,label,xf);
            ws->merge(0,1,0,4);
            cell->halign(HALIGN_CENTER);   //单元格水平方向居中

            cell  = ws->label(0,5,label1,xf);
            ws->merge(0,5,0,8);
            cell->halign(HALIGN_CENTER);   //单元格水平方向居中

            cell  = ws->label(0,9,label2,xf);
            ws->merge(0,9,0,12);
            cell->halign(HALIGN_CENTER);   //单元格水平方向居中



                ws->label(1,0,time,xf);

                ws->label(1,1,channel1,xf);
                ws->label(1,2,channel2,xf);
                ws->label(1,3,channel3,xf);
                ws->label(1,4,channel4,xf);

                ws->label(1,5,channel1,xf);
                ws->label(1,6,channel2,xf);
                ws->label(1,7,channel3,xf);
                ws->label(1,8,channel4,xf);

                ws->label(1,9,channel1,xf);
                ws->label(1,10,channel2,xf);
                ws->label(1,11,channel3,xf);
                ws->label(1,12,channel4,xf);


    }
    else if(g_bFootKey && m_pQTimer_showgraph->isActive())
    {
        g_bFootKeyHide = true;
        g_bFootKey = false;

        m_pQTimer_writedata->stop();
        wb.Dump(datafilename);
    }
}
char version[64];
/*触摸屏校验按钮按下超过五次，就触发触摸屏校验程序*/
void mdmedical::on_pushButton_calibrate_clicked()
{
    if((m_calibate_count++) > 5)
    {
        FILE *pfile = fopen("./res/file/version.txt", "r+");
        if (!pfile)
        {
            printf("cannot open txt result file\n");
            return;
        }
        fscanf(pfile,"%s",version);
        fclose(pfile);;

        g_bupdatestatus = true;
        if(SearchFirmwareName("/udisk","USMART.bin"))
        {
            if(CopyFile("/udisk/USMART.bin","/mnt/USMART.bin"))
            {
                m_bIsExistFirmware = true;
            }
            else
            {
                m_bIsExistFirmware = false;
            }
        }
        else
        {
                m_bIsExistFirmware = false;
        }
        mymanagerdialog->updateProgressBar->setValue(0);
        //string str =  version;
        m_calibate_count = 0;
        mymanagerdialog->label->setText(version);
        managerdialog->setWindowTitle("管理员模式");
        managerdialog->setModal(true);
        managerdialog->setWindowFlags(Qt::Window);
        managerdialog->showFullScreen();
    }
}
/*检测触摸屏校验程序是否执行完成，完成的情况下显示软件的整体界面*/
void mdmedical::DetectCalibrate()
{
    if(m_QProcess_calibration->state() == 0)
    {
        this->show();  //显示软件的整体界面
        this->managerdialog->show();
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
        lefttime = "剩余治疗时间："  + QString::number(m_minute, 10) + ":" + QString::number(m_second, 10);
        m_QLeftcureTime->setText(lefttime);

}

bool mdmedical::SearchFirmwareName(const char *basePath,const char *filename)
{
        DIR *dir;
        struct dirent *ptr;
        bool isFileExits = false;

        if ((dir=opendir(basePath)) == NULL)
        {
            perror("Open dir error...");
            return isFileExits;
        }

        while ((ptr=readdir(dir)) != NULL)
        {
            if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
                continue;
            else if(ptr->d_type == 8)    ///file
            {
                printf("d_name:%s/%s\n",basePath,ptr->d_name);
                if(strstr(ptr->d_name,filename) != NULL)
                {
                    m_bIsExistFirmware = true;
                    isFileExits = true;
                    break;
                }
            }
            //else if(ptr->d_type == 10)    ///link file
             //   printf("d_name:%s/%s\n",basePath,ptr->d_name);
//            else if(ptr->d_type == 4)    ///dir
//            {
//                memset(base,'\0',sizeof(base));
//                strcpy(base,basePath);
//                strcat(base,"/");
//                strcat(base,ptr->d_name);
//                SearchFirmwareName(base);
//            }
        }
        closedir(dir);

        return isFileExits;
}



void mdmedical::CopyUserData()
{
        DIR *dir;
        struct dirent *ptr;
        char *tmp_pos = NULL;
        char dest_dir[48] = "/udisk/";
        char src_dir[48] = "/home/scandata/";
        char *basePath = "/home/scandata";

        if ((dir=opendir(basePath)) == NULL)
        {
            perror("Open dir error...");
            return ;
        }

        while ((ptr=readdir(dir)) != NULL)
        {
            if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
                continue;
            else if(ptr->d_type == 8)    ///file
            {
                //printf("d_name:%s/%s\n",basePath,ptr->d_name);
                tmp_pos = strstr(ptr->d_name,"20");
                strcat(dest_dir,tmp_pos);
                strcat(src_dir,ptr->d_name);
                CopyFile(src_dir,dest_dir);
                printf("copy  %s  to  %s\n",src_dir,dest_dir);
//                if(strstr(ptr->d_name,filename) != NULL)
//                {
//                    m_bIsExistFirmware = true;
//                    isFileExits = true;
//                    break;
//                }
                //CopyFile(ptr->d_name,"/home/scandata/"+);
            }

        }
        closedir(dir);
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
    char time[4]={0};
    sprintf(time,"%d",second);
    ws->label(row,col,time,xf);
    second++;
    col++;


    QString data0 = QString("%1").arg(impedance[0]);
    ws->label(row,col,data0.toStdString().c_str(),xf);
    col++;
    QString data3 = QString("%1").arg(impedance[1]);
    ws->label(row,col,data3.toStdString().c_str(),xf);
    col++;
    QString data6 = QString("%1").arg(impedance[2]);
    ws->label(row,col,data6.toStdString().c_str(),xf);
    col++;
    QString data9 = QString("%1").arg(impedance[3]);
    ws->label(row,col,data9.toStdString().c_str(),xf);
    col++;



    QString data1 = QString("%1").arg(g_fCurrTmp[0]);
    ws->label(row,col,data1.toStdString().c_str(),xf);
    col++;
    QString data4 = QString("%1").arg(g_fCurrTmp[2]);
     ws->label(row,col,data4.toStdString().c_str(),xf);
     col++;
     QString data7 = QString("%1").arg(g_fCurrTmp[4]);
     ws->label(row,col,data7.toStdString().c_str(),xf);
     col++;
     QString data10 = QString("%1").arg(g_fCurrTmp[6]);
     ws->label(row,col,data10.toStdString().c_str(),xf);
     col++;



    QString data2 = QString("%1").arg(g_fCurrTmp[1]);
    ws->label(row,col,data2.toStdString().c_str(),xf);
    col++;
    QString data5 = QString("%1").arg(g_fCurrTmp[3]);
    ws->label(row,col,data5.toStdString().c_str(),xf);
    col++;
    QString data8 = QString("%1").arg(g_fCurrTmp[5]);
    ws->label(row,col,data8.toStdString().c_str(),xf);
    col++;
    QString data11 = QString("%1").arg(g_fCurrTmp[7]);
    ws->label(row,col,data11.toStdString().c_str(),xf);

    row++;
    col = 0;

    //float f = (float)(sel1.powerrate[0]-0x30) + ((float)(sel1.powerrate[2]-0x30)/10);
    //f = (float)(sel2.powerrate[0]-0x30) + ((float)(sel2.powerrate[2]-0x30)/10);
    //f = (float)(sel4.powerrate[0]-0x30) + ((float)(sel4.powerrate[2]-0x30)/10);
    //f = (float)(sel5.powerrate[0]-0x30) + ((float)(sel5.powerrate[2]-0x30)/10);
}

/*固件升级显示状态，包括进度条与升级完成提示*/
void mdmedical::ShowUpdateStr()
{
    if(g_bisUpdatesuccess)
    {
        mymanagerdialog->label->setText(g_version);
        g_bisUpdatesuccess = false;
        QMessageBox::information(this,"update window",QString::fromUtf8("更新完成"));
        if(m_pShowUpdateFinished->isActive())
            m_pShowUpdateFinished->stop();
    }

    static int i =0 ;
    if(i == 0)
    {
        mymanagerdialog->updateProgressBar->setRange(0,g_updateprocess);
        //mymanagerdialog->updateProgressBar->setMinimumSize(0,100);
        i++;
    }

    if(g_bStartProcessBar)
    {
        mymanagerdialog->updateProgressBar->setValue(sel1.m_updateprocess + sel2.m_updateprocess + sel4.m_updateprocess + sel5.m_updateprocess);

        //updateProgressBar->setMinimumSize(0,100);
        //mymanagerdialog->updateProgressBar->setVisible(1);
    }

}



void mdmedical::RecordTempture()
{

    static int count = 0;
    if(count == 0)
    {
        col = 0;
        row = 2;
        second = 1;
        xf = wb.xformat();
        ws = wb.sheet("sheet1");

        string time = "时间 秒";
            string label = "1st单位：阻抗欧姆";
            string label1 = "1st单位：℃ 针尖";
            string label2 = "1st单位：℃ 粘膜";

            string channel1 = "通道1";
            string channel2 = "通道2";
            string channel3 = "通道3";
            string channel4 = "通道4";


            cell  = ws->label(0,1,label,xf);
            ws->merge(0,1,0,4);
            cell->halign(HALIGN_CENTER);   //单元格水平方向居中

            cell  = ws->label(0,5,label1,xf);
            ws->merge(0,5,0,8);
            cell->halign(HALIGN_CENTER);   //单元格水平方向居中

            cell  = ws->label(0,9,label2,xf);
            ws->merge(0,9,0,12);
            cell->halign(HALIGN_CENTER);   //单元格水平方向居中



                ws->label(1,0,time,xf);

                ws->label(1,1,channel1,xf);
                ws->label(1,2,channel2,xf);
                ws->label(1,3,channel3,xf);
                ws->label(1,4,channel4,xf);

                ws->label(1,5,channel1,xf);
                ws->label(1,6,channel2,xf);
                ws->label(1,7,channel3,xf);
                ws->label(1,8,channel4,xf);

                ws->label(1,9,channel1,xf);
                ws->label(1,10,channel2,xf);
                ws->label(1,11,channel3,xf);
                ws->label(1,12,channel4,xf);

    }
    count++;
    char time[4]={0};
    sprintf(time,"%d",second);
    ws->label(row,col,time,xf);
    second++;
    col++;


    QString data0 = QString("%1").arg(impedance[0]);
    ws->label(row,col,data0.toStdString().c_str(),xf);
    col++;
    QString data3 = QString("%1").arg(impedance[1]);
    ws->label(row,col,data3.toStdString().c_str(),xf);
    col++;
    QString data6 = QString("%1").arg(impedance[2]);
    ws->label(row,col,data6.toStdString().c_str(),xf);
    col++;
    QString data9 = QString("%1").arg(impedance[3]);
    ws->label(row,col,data9.toStdString().c_str(),xf);
    col++;



    QString data1 = QString("%1").arg(g_fCurrTmp[0]);
    ws->label(row,col,data1.toStdString().c_str(),xf);
    col++;
    QString data4 = QString("%1").arg(g_fCurrTmp[2]);
     ws->label(row,col,data4.toStdString().c_str(),xf);
     col++;
     QString data7 = QString("%1").arg(g_fCurrTmp[4]);
     ws->label(row,col,data7.toStdString().c_str(),xf);
     col++;
     QString data10 = QString("%1").arg(g_fCurrTmp[6]);
     ws->label(row,col,data10.toStdString().c_str(),xf);
     col++;



    QString data2 = QString("%1").arg(g_fCurrTmp[1]);
    ws->label(row,col,data2.toStdString().c_str(),xf);
    col++;
    QString data5 = QString("%1").arg(g_fCurrTmp[3]);
    ws->label(row,col,data5.toStdString().c_str(),xf);
    col++;
    QString data8 = QString("%1").arg(g_fCurrTmp[5]);
    ws->label(row,col,data8.toStdString().c_str(),xf);
    col++;
    QString data11 = QString("%1").arg(g_fCurrTmp[7]);
    ws->label(row,col,data11.toStdString().c_str(),xf);

    row++;
    col = 0;

    if(count == 60)
    {
        if(m_pRecordTempture->isActive())
            m_pRecordTempture->stop();

        printf("create file finished!\n");
        wb.Dump("datafilename_tempture.xls");

    }
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

bool mdmedical::CopyFile(QString src, QString dst)
{
    if (QFile::exists(dst))
    {
        QFile::remove(dst);
    }

    if (!QFile::copy(src, dst))
    {
        printf( "copy faild\n");
        return false;
    }
    return true;
}

#if 0
void mdmedical::PlaySound(QString soundpath)
{
    StopSound();
    QStringList args;
    args << soundpath;
    mplayerSoundProcess->start(QString("/usr/bin/mplayer"), args);
}

void mdmedical::StopSound()
{
    if(mplayerSoundProcess&&mplayerSoundProcess->isOpen())
        mplayerSoundProcess->close();
}
#endif


void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt(msg);
    if(outFile.isOpen()&&outFile.isWritable())
    {
        QTextStream ts(&outFile);
        ts << txt;
    }
}



