#include "mdmedical.h"
#include "ui_mdmedical.h"



bool port[4] = {false,false,false,false};

float g_fCurrTmp[8];
float showtmp[4];
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
bool g_bisBreakGas = false;
bool g_bPrepareKey = false;
bool g_bFootKey = false;
bool g_bChargeGasKey = false;
bool g_bBreakGasKey = false;
bool g_bIsCreateAdcDetectpthread = true;
bool g_bFootKeyHide = false;
bool m_bIsfootkeyPress;
bool g_bisupdatestatus = true;
bool g_bisfinishedupdate = false;
bool g_bupdatestatus = false;
bool g_bisUpdatesuccess = false;
bool g_bisGatheringAdcValue = false;
bool g_isInsertingPipe = false;

static char g_cWhichKey = 0;
static int g_cWhichChannel;
static bool g_bKeyValue = false;  //first press or second press  true:first press  false:second press
static bool g_bRecordChannelStatus[8] = {true,true,true,true,true,true,true,true};
volatile bool g_bIsFinishiIncreasePower = true;
volatile bool g_bIsfinishReadtempture = true;
static int g_iPipeStatus = -1;
extern CCURRENTSTATUSVALUE surrentstatusvalue;
extern bool g_isSetting;
static int fd_tmp = -1;

static volatile bool g_tmp=true,g_pin=true;

volatile bool channel_mask[4]={false,false,false,false};

pthread_mutex_t g_mutex;

pthread_mutex_t g_mutex_status = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_mycond_status = PTHREAD_COND_INITIALIZER;

pthread_mutex_t g_mutex_status1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t g_mycond_status1= PTHREAD_COND_INITIALIZER;

QVector<double> temp(1200);  //zhengjian max
QVector<double> temp1(1200);  //zhengjian min
QVector<double> temp2(1200);  //nianmo Max
QVector<double> temp3(1200);  //nianmo Min
//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/

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
    int ret = -1000 ;
    if(!channel_mask[0])
    {
        sel1.IdleSend(0);
        sel1.SendData();
        ret == sel1.RecvData();
        if(ret == -1)
        {
            DIGI_TRACE(ERROR_INFO,"Idle_send sel1 Select fail!");
        }
        else if(ret == -2)
        {
            DIGI_TRACE(ERROR_INFO,"Idle_send sel1 Select timeout!");
        }
        ok1 = sel1.ParseIdleSendReturnData();
    }

    if(!channel_mask[1])
    {
        sel2.IdleSend(0);
        sel2.SendData();
        ret = sel2.RecvData();
        if(ret == -1)
        {
            DIGI_TRACE(ERROR_INFO,"Idle_send sel2 Select fail!");
        }
        else if(ret == -2)
        {
            DIGI_TRACE(ERROR_INFO,"Idle_send sel2 Select timeout!");
        }
        ok2 = sel2.ParseIdleSendReturnData();
    }

    if(!channel_mask[2])
    {
        sel4.IdleSend(0);
        sel4.SendData();
        ret = sel4.RecvData();
        if(ret == -1)
        {
            DIGI_TRACE(ERROR_INFO,"Idle_send sel4 Select fail!");
        }
        else if(ret == -2)
        {
            DIGI_TRACE(ERROR_INFO,"Idle_send sel4 Select timeout!");
        }
        ok3 = sel4.ParseIdleSendReturnData();
    }

    if(!channel_mask[3])
    {
        sel5.IdleSend(0);
        sel5.SendData();
        ret = sel5.RecvData();
        if(ret == -1)
        {
            DIGI_TRACE(ERROR_INFO,"Idle_send sel5 Select fail!");
        }
        else if(ret == -2)
        {
            DIGI_TRACE(ERROR_INFO,"Idle_send sel5 Select timeout!");
        }
        ok4 = sel5.ParseIdleSendReturnData();
    }

    return (ok1&&ok2&&ok3&&ok4);
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


void *GetCurrTmp(__attribute__((unused))void *args)
{
        unsigned int i,j;
        unsigned short currTmp1[32]={0};
        unsigned short currTmp2[32]={0};
        float tmp1[8]={0},tmp2[8]={0};
        struct timeval start,end;
        char temp = 0;
        char pin_status = 1;
        int count1=0;
        int cycle_times = 0,channelcheckcount = 0;
        bool tempchannelstatus[8][3] = {true};
        fd_tmp = open("/dev/mdmedical_tmp", O_RDWR);
        if(fd_tmp < 0)
        {
            printf("open device failed!\n");
        }
        usleep(2*1000);
        while(1)
        {
                cycle_times++;

#if 1
                write(fd_tmp,&pin_status,1);
                for(i=3;i<11;i++)
                {
                    g_cWhichChannel = i - 3;
                    ioctl(fd_tmp,i,12);
                    usleep(1200);
                    read(fd_tmp,&g_iPipeStatus,4);
                    if(g_iPipeStatus)
                    {
                        tempchannelstatus[g_cWhichChannel][channelcheckcount] = true;
                        if(i==3)
                        {
                            //printf("ggggggggggggg %d %d  %d\n",count1+1,cycle_times,i);
                            count1++;
                        }
                    }
                    else
                    {
                        if(i==3)
                        {
                            //printf("sssssssssss %d %d  %d\n",count1,cycle_times,i);
                        }
                        tempchannelstatus[g_cWhichChannel][channelcheckcount] = false;
                    }
                }


                usleep(10*1000);

#endif

                    gettimeofday(&start,NULL);
                    write(fd_tmp,&temp,1);
                    for(i=3;i<11;i++)
                    {
                        if(channel_mask[(i-3)/2])
                            continue;
                        ioctl(fd_tmp,i,12);
                        read(fd_tmp,currTmp1,32);
                        tmp1[i-3] = fliter(currTmp1);
                    }
                    gettimeofday(&end,NULL);
                    //printf("chazhi is %d\n",(end.tv_sec*1000000+end.tv_usec-start.tv_usec-start.tv_sec*1000000));
                    if((end.tv_sec*1000000+end.tv_usec-start.tv_usec-start.tv_sec*1000000) >= 10000){}
                    else
                    {
                        usleep(10*1000-(end.tv_sec*1000000+end.tv_usec-start.tv_usec-start.tv_sec*1000000));
                    }
                    usleep(10*1000);

#if 1
                    for(i=3;i<11;i++)
                    {
                        if(channel_mask[(i-3)/2])
                            continue;
                        ioctl(fd_tmp,i,12);
                        read(fd_tmp,&currTmp2,32);
                        tmp2[i-3] = fliter(currTmp2);
                    }
#endif

                    g_bIsFinishiIncreasePower = true;
                    for(int j=0;j<8;j++)
                    {
                        g_fCurrTmp[j] = (tmp1[j] + tmp2[j])/2;

                    }

 /************************************以上读温度************************************************/
#if 0
                    usleep(10*1000);

                    write(fd_tmp,&pin_status,1);
                    for(i=3;i<11;i++)
                    {
                        g_cWhichChannel = i - 3;
                        ioctl(fd_tmp,i,12);
                        usleep(1200);
                        read(fd_tmp,&g_iPipeStatus,4);
                        if(g_iPipeStatus)
                        {
                            tempchannelstatus[g_cWhichChannel][channelcheckcount] = true;
                            if(i==3)
                            {
                                printf("ggggggggggggg %d %d  %d\n",count1,cycle_times,i);
                                count1++;
                            }
                        }
                        else
                        {
                            if(i==3)
                            {
                                //printf("sssssssssss %d %d  %d\n",count1,cycle_times,i);
                                count1++;
                            }
                            tempchannelstatus[g_cWhichChannel][channelcheckcount] = false;
                        }
                    }
#endif
 /************************************检测 通道************************************************/
#if 1

                        channelcheckcount++;
                        if(channelcheckcount == 3)
                        {
                            for(i=0;i<8;i++)
                            {
                                    if(tempchannelstatus[i][0] && tempchannelstatus[i][1] &&tempchannelstatus[i][2])
                                    {
                                        g_bRecordChannelStatus[i] = true;
                                        if(i == 0)
                                        {
                                            //printf("ffffffffffff\n");
                                        }
                                    }
                                    else
                                    {
                                        g_bRecordChannelStatus[i] = false;
                                        //printf("ffffffffffff\n");
                                    }
                            }
                            channelcheckcount=0;
                        }
#endif
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

  #endif
                    //usleep(400*1000);
        }
}

#if 0
void *ReadPinStatus(__attribute__((unused))void *args)
{
    char pin_status = 1;
    bool last=false,current=false,next=false;

    for(;;)
    {
        if(fd_tmp < 0)
        {
            usleep(100*1000);
            continue;
        }
        else
        {
            write(fd_tmp,&pin_status,1);
            break;
        }
    }
    while(1)
    {

        if(g_tmp)
        {
            usleep(10);
            continue;
        }
            //printf("kkkkkkkkkkkkkkkkkkkkkkkk\n");
            usleep(150);
            read(fd_tmp,&g_iPipeStatus,4);
            if(g_iPipeStatus)
            {
                last = true;
            }
            else
            {
                last = false;
            }
            usleep(150);
            read(fd_tmp,&g_iPipeStatus,4);
            if(g_iPipeStatus)
            {
                current = true;
            }
            else
            {
                current = false;
            }
            usleep(150);
            read(fd_tmp,&g_iPipeStatus,4);
            if(g_iPipeStatus)
            {
                next = true;
            }
            else
            {
                next = false;
            }

            if(last && current && next)
            {
                g_bRecordChannelStatus[g_cWhichChannel] = true;
            }
            else
            {
                g_bRecordChannelStatus[g_cWhichChannel] = false;
            }

            g_tmp = true;
            g_pin = false;
    }
}
#endif

/*检测按键按下，/dev/mdmedical_buttons为按键的驱动程序的设备节点，驱动采用中断方式检测按键是否按下*/
void *detect_key_func(__attribute__((unused))void *args)
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
            g_cWhichKey = 1;
            g_bKeyValue = !g_bKeyValue;
            printf(" %d\n",key_val);
        }
        else if(key_val == 0x2)
        {
             g_cWhichKey = 2;
             g_bKeyValue = !g_bKeyValue;
            printf(" %d\n",key_val);
        }
        else if(key_val == 0x3)
        {
            //g_cWhichKey = 3;
            //g_bKeyValue = !g_bKeyValue;
            printf(" %d\n",key_val);
        }
        else if(key_val == 0x4)
        {
             //g_cWhichKey = 4;
             //g_bKeyValue = !g_bKeyValue;
            printf(" %d\n",key_val);
        }
        else if(key_val == 0x5)
        {

            printf(" %d\n",key_val);
        }
        else if(key_val == 0x6)
        {

            printf(" %d\n",key_val);
        }
        else if(key_val == 0x7)
        {

            printf(" %d\n",key_val);
        }
        else if(key_val == 0x8)
        {

            printf(" %d\n",key_val);
        }


        //printf("key_val = 0x%x\n", key_val);
    }
}

void *deteAdcValve(__attribute__((unused))void *args)
{
    bool bIsBreakingGas = false;
    int fd = open("/dev/mdmedical_adc", O_RDWR);
    if(fd < 0)
    {
        printf("open /dev/mdmedical_adc failed!\n");
    }
        int  AdValue=0;
        char val = 2, flag1 = 1,flag2 = 1,flag3 = 1,Air_pressure = 0;
        while(g_bisGatheringAdcValue)
        {
          read(fd,&AdValue,4);
          //printf("fffffffff %d\n",AdValue);
          usleep(200*1000);
          Air_pressure = (char)((0.032 * AdValue - 0.1)/0.04);
           if(AdValue > 40 && flag1 == 1)
           {
               flag1 = 0;
               flag2 = 1;
               flag3 = 1;
               val = 3;
               write(fd,&val,4);

           }
           else if(AdValue <= 40 && AdValue >= 38 && flag2 == 1)
           {
               flag1 = 1;
               flag2 = 0;
               flag3 = 1;
               val = 4;
               write(fd,&val,4);

           }
           else if(AdValue < 38 && flag3 == 1)
           {
               flag1 = 1;
               flag2 = 1;
               flag3 = 0;

               if(!bIsBreakingGas)
               {
                   val = 2;
                   write(fd,&val,4);
               }
               else
               {
                   if(AdValue < 10)
                   {
                       val = 4;
                       bIsBreakingGas  = false;
                       write(fd,&val,4);
                   }
               }

           }

           if(g_bisBreakGas)
           {
               g_bisBreakGas = false;
               bIsBreakingGas = true;
               val = 3;
               write(fd,&val,4);
           }
        }
        g_bIsCreateAdcDetectpthread = true;
        close(fd);
        return NULL;
}

char g_version[14];
int g_updateprocess;
bool g_bStartProcessBar = false;
void *update_firmware(__attribute__((unused))void *args)
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
        return NULL;
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

        mysetpage = NULL;

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

        mysysteminfo = new Ui_SystemInfo();
        systeminfo = new QDialog(this);// superFactoryMode;
        mysysteminfo->setupUi(systeminfo);

        mysetpage = new CSettingPage;



        pthread_mutex_init(&g_mutex, NULL);



      sel1.openSerial(SERIAL1,115200);
      sel2.openSerial(SERIAL2,115200);
      sel4.openSerial(SERIAL4,115200);
      sel5.openSerial(SERIAL5,115200);


        m_QProcess_calibration = new QProcess();

        groupBox_channelselect = new QGroupBox(this);
        groupBox_channelselect->setGeometry(800,350,200,150);
        groupBox_channelselect->setTitle(QString::fromUtf8("通道选择"));

        pushButton_td1 = new QPushButton(groupBox_channelselect);
        pushButton_td2 = new QPushButton(groupBox_channelselect);
        pushButton_td3 = new QPushButton(groupBox_channelselect);
        pushButton_td4 = new QPushButton(groupBox_channelselect);

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
        m_pQTimerFlushShowInfo = new QTimer(this);

        m_pQTimerShowUpdateFinished = new QTimer(this);

        m_pQTimerRecordTempture  = new QTimer(this);

         m_QLabel_pic1 = new QLabel(this);
         m_QLeftcureTime = new QLabel(this);

         progressBar1 = new QProgressBar(this);
         progressBar2 = new QProgressBar(this);
         progressBar3 = new QProgressBar(this);
         progressBar4 = new QProgressBar(this);



         promitwidget = new QWidget(this);
         promitwidget->setGeometry(0,650,1024,768-650);
         promitwidget->setStyleSheet(QString::fromUtf8("border:1px solid gray"));

          m_QLabel_operationprompttitle = new QLabel(promitwidget);
          m_QLabel_operationprompttitle->setGeometry(600,3,100,25);
          m_QLabel_operationprompttitle->setText(QString::fromUtf8("操作提示"));

          m_QLabel_operationpromp = new QLabel(promitwidget);
          m_QLabel_operationpromp->setGeometry(100,35,650,25);
          m_QLabel_operationpromp->setText(QString::fromUtf8("按“系统设置”进入系统治疗参数和系统时间设定，若按默认治疗参数进行治疗请按“下一步”"));

          m_QPushButton_nextstep = new QPushButton(promitwidget);
          m_QPushButton_nextstep->setGeometry(800,40,60,30);
          m_QPushButton_nextstep->setText(QString::fromUtf8("下一步"));


         connect(m_QPushButton_nextstep,SIGNAL(clicked()),this,SLOT(NextStep()));
         connect(mymanagerdialog->pushButton_ts,SIGNAL(clicked()),this,SLOT(TsModify()));
         connect(mymanagerdialog->pushButton_updata,SIGNAL(clicked()),this,SLOT(Updata()));
         connect(mymanagerdialog->pushButton_copy,SIGNAL(clicked()),this,SLOT(CopyUserData()));
         connect(mymanagerdialog->pushButton_exit,SIGNAL(clicked()),this,SLOT(ExitManagerPage()));

        connect(mysysteminfo->pushButton_sure,SIGNAL(clicked()),this,SLOT(ExitSystemInfoPage()));

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
        connect(m_pQTimerShowUpdateFinished, SIGNAL(timeout()), this, SLOT(ShowUpdateStr()));

        connect(m_pQTimerRecordTempture, SIGNAL(timeout()), this, SLOT(RecordTempture()));
        connect(m_pQTimerFlushShowInfo, SIGNAL(timeout()), this, SLOT(FlushShowInfo()));
        connect(pushButton_td1,SIGNAL(clicked()),this,SLOT(GetChanel1Value()));
        connect(pushButton_td2,SIGNAL(clicked()),this,SLOT(GetChanel2Value()));
        connect(pushButton_td3,SIGNAL(clicked()),this,SLOT(GetChanel3Value()));
        connect(pushButton_td4,SIGNAL(clicked()),this,SLOT(GetChanel4Value()));

        //connect(m_QComboBox_cureperiod, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetCureCycleCurrentValue()));
        //connect(m_QComboBox_targettemp, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetTargetTmpCurrentValue()));
        //connect(m_QComboBox_maxpower, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetMaxPowerCurrentValue()));
        //connect(m_QComboBox_curepos, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(GetCurePosCurrentValue()));


//        labelList.at(i)->setStyleSheet("QLabel{background:rgb(234,71,6);"
//                                               "color:white;"
//                                               "image: url(:/Picture/abnormal);}");

        ui->label_showcurtime->setGeometry(350,10,400,50);
        ui->label_showcurtime->setText("");

        ui->label_tmp1->setGeometry(260,100,65,20);  //chanel   1
        ui->label_tmp2->setGeometry(260,163,65,20);  //chanel   1  nianmo (454,278,65,20);
        ui->label_tmp3->setGeometry(400,305,65,20);
        ui->label_tmp4->setGeometry(400,250,65,20);

        ui->label_tmp5->setGeometry(260,467,65,20);
        ui->label_tmp6->setGeometry(260,400,65,20);(280,467,65,20);//400,267,65,20
        ui->label_tmp7->setGeometry(130,250,65,20);//280,400,65,20
        ui->label_tmp8->setGeometry(130,305,65,20);



        ui->label_impedance1->setGeometry(260,130,65,20);
        ui->label_impedance2->setGeometry(400,278,65,20);
        ui->label_impedance3->setGeometry(260,430,65,20);
        ui->label_impedance4->setGeometry(130,278,65,20);

        ui->label_impedance1->setText("");
        ui->label_impedance2->setText("");
        ui->label_impedance3->setText("");
        ui->label_impedance4->setText("");

        ui->label_tmp1->setText("");
        ui->label_tmp2->setText("");
        ui->label_tmp3->setText("");
        ui->label_tmp4->setText("");

        ui->label_tmp5->setText("");
        ui->label_tmp6->setText("");
        ui->label_tmp7->setText("");
        ui->label_tmp8->setText("");

        ui->label_tmp1->setAlignment(Qt::AlignHCenter);
        ui->label_tmp2->setAlignment(Qt::AlignHCenter);
        ui->label_tmp3->setAlignment(Qt::AlignHCenter);
        ui->label_tmp4->setAlignment(Qt::AlignHCenter);
        ui->label_tmp5->setAlignment(Qt::AlignHCenter);
        ui->label_tmp6->setAlignment(Qt::AlignHCenter);
        ui->label_tmp7->setAlignment(Qt::AlignHCenter);
        ui->label_tmp8->setAlignment(Qt::AlignHCenter);

        ui->label_gasvalue->setAlignment(Qt::AlignHCenter);



        ui->label_tmp1->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
        ui->label_tmp2->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
        ui->label_tmp3->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
        ui->label_tmp4->setStyleSheet ("background-color:red;color:red;border-radius:4px;");

        ui->label_tmp5->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
        ui->label_tmp6->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
        ui->label_tmp7->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
        ui->label_tmp8->setStyleSheet ("background-color:red;color:red;border-radius:4px;");




        ui->label_impedance1->setStyleSheet ("background-color:gray;color:black;border-radius:4px;");
        ui->label_impedance2->setStyleSheet ("background-color:gray;color:black;border-radius:4px;");
        ui->label_impedance3->setStyleSheet ("background-color:gray;color:black;border-radius:4px;");
        ui->label_impedance4->setStyleSheet ("background-color:gray;color:black;border-radius:4px;");

        ui->label_gasvalue->setStyleSheet ("background-color:white;color:black;");




        ui->groupBox_zlzq->setGeometry(800,20,200,100);
        ui->groupBox_zlzq->setTitle(QString::fromUtf8("治疗周期"));
        ui->label_zlzqIcon->setGeometry(810,50,30,30);
        ui->label_zlzqIcon->setPixmap(QPixmap(MAINPAGE"period.png"));
        ui->label_zlzq->setGeometry(850,50,130,30);
        ui->label_zlzq->setText("1:00");
        ui->groupBox_zlzq->setStyleSheet(
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


        ui->groupBox_tagetTmp->setGeometry(800,130,200,100);
        ui->groupBox_tagetTmp->setTitle(QString::fromUtf8("目标温度（℃）"));
        ui->label_tagetIcon->setGeometry(810,160,30,30);
        ui->label_tagetIcon->setPixmap(QPixmap(MAINPAGE"temp.png"));
        ui->label_tagetTmp->setGeometry(850,160,130,30);
        ui->label_tagetTmp->setText("50");
        ui->groupBox_tagetTmp->setStyleSheet(
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




        ui->groupBox_Max->setGeometry(800,240,200,100);
        ui->groupBox_Max->setTitle(QString::fromUtf8("最大功率"));
        ui->label_MaxIcon->setGeometry(810,270,30,30);
        ui->label_MaxIcon->setPixmap(QPixmap(MAINPAGE"power.png"));
        ui->label_Max->setGeometry(850,270,130,30);
        ui->label_Max->setText("1.0");
        ui->groupBox_Max->setStyleSheet(
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




        pushButton_td1->setGeometry(70,30,30,30);
        pushButton_td2->setGeometry(111,65,30,30);
        pushButton_td3->setGeometry(70,100,30,30);
        pushButton_td4->setGeometry(30,65,30,30);
        pushButton_td1->setStyleSheet("background-color: rgb(0, 0, 0);color:white");
        pushButton_td2->setStyleSheet("background-color: rgb(0, 0, 0);color:white");
        pushButton_td3->setStyleSheet("background-color: rgb(0, 0, 0);color:white");
        pushButton_td4->setStyleSheet("background-color: rgb(0, 0, 0);color:white");
        pushButton_td1->setText("1");
        pushButton_td2->setText("2");
        pushButton_td3->setText("3");
        pushButton_td4->setText("4");

        groupBox_channelselect->setStyleSheet(
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




        ui->groupBox_coldwaterstream->setGeometry(800,510,200,100);
        ui->groupBox_coldwaterstream->setTitle(QString::fromUtf8("冷却流量（ml）"));
        ui->label_coldwaterstreamIcon->setGeometry(810,540,30,30);
        ui->label_coldwaterstreamIcon->setPixmap(QPixmap(MAINPAGE"water.png"));
        ui->label_coldwaterstream->setGeometry(850,540,130,30);
        ui->label_coldwaterstream->setText("100");
        ui->groupBox_coldwaterstream->setStyleSheet(
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




        ui->pushButton_waitmode->setGeometry(900,150,100,30);
        ui->pushButton_waitmode->setText(QString::fromUtf8("待机模式"));
        ui->pushButton_waitmode->setVisible(0);
        ui->pushButton_calibrate->setGeometry(900,100,100,30);
        ui->pushButton_calibrate->setText(QString::fromUtf8("管理员模式"));
        ui->pushButton_calibrate->setVisible(0);








        ui->label_devidestatus->setGeometry(0,740,200,20);
        ui->label_devidestatus->setText(QString::fromUtf8("设备正常"));
        ui->label_devidestatus->setVisible(0);
        //ui->pushButton_addgas->setGeometry(40,120,60,30);
        //ui->pushButton_losegas->setGeometry(110,120,60,30);
        //ui->label_gasvalue->setGeometry(80,80,40,20);

        //ui->label_gasvalue->setText("150");
        //ui->pushButton_addgas->setText(QString::fromUtf8("充气"));
        //ui->pushButton_losegas->setText(QString::fromUtf8("放气"));
        ui->pushButton_addgas->setVisible(0);
        ui->pushButton_losegas->setVisible(0);
        ui->label_gasvalue->setVisible(0);


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
       ui->pushButton_showcam->setVisible(0);

       ui->pushButton_takephoto->setGeometry(450,660,80,35);
       ui->pushButton_takephoto->setText(QString::fromUtf8("拍照"));
       ui->pushButton_takephoto->setVisible(0);
       ui->pushButton_closecam->setGeometry(550,660,80,35);
       ui->pushButton_closecam->setText(QString::fromUtf8("关闭"));
       ui->pushButton_closecam->setVisible(0);


        ui->pushButton_setting->setGeometry(10,20,80,40);
        ui->pushButton_setting->setText(QString::fromUtf8("系统设置"));

        ui->pushButton_systeminfo->setGeometry(10,90,80,40);
        ui->pushButton_systeminfo->setText(QString::fromUtf8("系统信息"));


        InitMapValue();
         pthread_mutex_init(&g_mutex,NULL);

#if 0
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
#endif
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
        pthread_t tid_rs;
        int ret1;
        ret1 = pthread_create(&tid_rs,NULL,ReadPinStatus,NULL);
        if(ret != 0)
        {
            printf("create read channel status pthread failed!\n");
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


//mdmedical::MainStep()
//{
//    ui->pushButton_td1->setText("1");
//    ui->pushButton_td2->setText("2");
//    ui->pushButton_td3->setText("3");
//    ui->pushButton_td4->setText("4");
//}

mdmedical::~mdmedical()
{
    pthread_mutex_destroy(&g_mutex); //销毁互斥锁
    delete ui;
}


void mdmedical::InitMapValue()
{
    int i = 0;
    for(i = 0; i < 55; i++)
    {
        mymapperiod.insert(i,curePeriodValue[i]);
    }
    for(i = 0; i < 46; i++)
    {
        mymaptemp.insert(i,cureTempValue[i]);
    }
    for(i = 0; i < 21; i++)
    {
        mymappower.insert(i,curePowerValue[i]);
    }
    for(i = 0; i < 81; i++)
    {
        mymapwater.insert(i,cureWaterValue[i]);
    }
}

void mdmedical::ShowTimeCurrent(void)
{
    bool red=true;
    bool black=true;
    QTime qtimeObj = QTime::currentTime();
    QDate qdateObj = QDate::currentDate();
    QString strtime = qtimeObj.toString("hh:mm:ss");
    QString strDate = qdateObj.toString("yyyy-MM-dd"); //星期、月份、天、年
    strDate.append(" ");
    strDate.append(strtime);
    ui->label_showcurtime->setText(strDate);

    if(g_bRecordChannelStatus[0]&&g_bRecordChannelStatus[1]&&g_bRecordChannelStatus[2]&&g_bRecordChannelStatus[3]\
            &&g_bRecordChannelStatus[4]&&g_bRecordChannelStatus[5]&&g_bRecordChannelStatus[6]&&g_bRecordChannelStatus[7])
    {
        if(black)
        {
            black = false;
            red = true;
            QPalette pa;
            pa.setColor(QPalette::WindowText,Qt::black);
            m_QLabel_operationprompttitle->setPalette(pa);
            m_QLabel_operationpromp->setPalette(pa);
            m_QLabel_operationpromp->setText(QString::fromUtf8("按“系统设置”进入系统治疗参数和系统时间设定，若按默认治疗参数进行治疗请按“下一步”"));
            m_QPushButton_nextstep->setEnabled(true);
        }
    }
    else
    {
        if(red)
        {
            black = true;
            red = false;
            QPalette pa;
            pa.setColor(QPalette::WindowText,Qt::red);
            m_QLabel_operationprompttitle->setPalette(pa);
            m_QLabel_operationpromp->setPalette(pa);
            m_QLabel_operationpromp->setText(QString::fromUtf8("请插入治疗导管或检查治疗导管连接！”"));
            m_QPushButton_nextstep->setEnabled(false);
        }
    }
}

/*显示温度*/
void mdmedical::ShowCurrentTmp(void)
{
        if(!channel_mask[0])
        {
            if(g_bRecordChannelStatus[0])
            {
                ui->label_tmp1->setStyleSheet ("background-color:green;color:black;border-radius:4px;");
                QString data1 = QString("%1").arg(g_fCurrTmp[0]);
                ui->label_tmp1->setText(data1.mid(0,4));
            }
            else
            {
                ui->label_tmp1->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
                ui->label_tmp1->setText("");
            }

            if(g_bRecordChannelStatus[1])
            {
                ui->label_tmp2->setStyleSheet ("background-color:green;color:black;border-radius:4px;");
                QString data1 = QString("%1").arg(g_fCurrTmp[1]);
                ui->label_tmp2->setText(data1.mid(0,4));
            }
            else
            {
                ui->label_tmp2->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
                ui->label_tmp2->setText("");
            }
        }
        else
        {
            ui->label_tmp1->setStyleSheet ("background-color:white;color:red;border-radius:4px;");
            ui->label_tmp1->setText("");
            ui->label_tmp2->setStyleSheet ("background-color:white;color:red;border-radius:4px;");
            ui->label_tmp2->setText("");
        }
        if(!channel_mask[1])
        {
            if(g_bRecordChannelStatus[2])
            {
                ui->label_tmp3->setStyleSheet ("background-color:green;color:black;border-radius:4px;");
                QString data1 = QString("%1").arg(g_fCurrTmp[2]);
                ui->label_tmp3->setText(data1.mid(0,4));
            }
            else
            {
                ui->label_tmp3->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
                ui->label_tmp3->setText("");
            }

            if(g_bRecordChannelStatus[3])
            {
                ui->label_tmp4->setStyleSheet ("background-color:green;color:black;border-radius:4px;");
                QString data1 = QString("%1").arg(g_fCurrTmp[3]);
                ui->label_tmp4->setText(data1.mid(0,4));
            }
            else
            {
                ui->label_tmp4->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
                ui->label_tmp4->setText("");
            }
        }
        else
        {
            ui->label_tmp3->setStyleSheet ("background-color:white;color:red;border-radius:4px;");
            ui->label_tmp3->setText("");
            ui->label_tmp4->setStyleSheet ("background-color:white;color:red;border-radius:4px;");
            ui->label_tmp4->setText("");
        }
        if(!channel_mask[2])
        {
            if(g_bRecordChannelStatus[4])
            {
                ui->label_tmp5->setStyleSheet ("background-color:green;color:black;border-radius:4px;");
                QString data1 = QString("%1").arg(g_fCurrTmp[4]);
                ui->label_tmp5->setText(data1.mid(0,4));
            }
            else
            {
                ui->label_tmp5->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
                ui->label_tmp5->setText("");
            }

            if(g_bRecordChannelStatus[5])
            {
                ui->label_tmp6->setStyleSheet ("background-color:green;color:black;border-radius:4px;");
                QString data1 = QString("%1").arg(g_fCurrTmp[5]);
                ui->label_tmp6->setText(data1.mid(0,4));
            }
            else
            {
                ui->label_tmp6->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
                ui->label_tmp6->setText("");
            }
        }
        else
        {
            ui->label_tmp5->setStyleSheet ("background-color:white;color:red;border-radius:4px;");
            ui->label_tmp5->setText("");
            ui->label_tmp6->setStyleSheet ("background-color:white;color:red;border-radius:4px;");
            ui->label_tmp6->setText("");
        }
        if(!channel_mask[3])
        {
            if(g_bRecordChannelStatus[6])
            {
                ui->label_tmp7->setStyleSheet ("background-color:green;color:black;border-radius:4px;");
                QString data1 = QString("%1").arg(g_fCurrTmp[6]);
                ui->label_tmp7->setText(data1.mid(0,4));
            }
            else
            {
                ui->label_tmp7->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
                ui->label_tmp7->setText("");
            }

            if(g_bRecordChannelStatus[7])
            {
                ui->label_tmp8->setStyleSheet ("background-color:green;color:black;border-radius:4px;");
                QString data1 = QString("%1").arg(g_fCurrTmp[7]);
                ui->label_tmp8->setText(data1.mid(0,4));
            }
            else
            {
                ui->label_tmp8->setStyleSheet ("background-color:red;color:red;border-radius:4px;");
                ui->label_tmp8->setText("");
            }
        }
        else
        {
            ui->label_tmp7->setStyleSheet ("background-color:white;color:red;border-radius:4px;");
            ui->label_tmp7->setText("");
            ui->label_tmp8->setStyleSheet ("background-color:white;color:red;border-radius:4px;");
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
        m_pQTimerShowUpdateFinished->start(100);
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
void mdmedical::ExitSystemInfoPage()
{
    systeminfo->close();;
}

/*治疗模式下绘制温度曲线图与控制功率的操作*/
void mdmedical::makeGraph()
{
    static int count = 0;
    int i;

    if(closegraph %2 == 0)
        ShowLeftCureTime();



    if((closegraph++) ==  (surrentstatusvalue.cureCycle*10+60)*2 || g_bFootKeyHide)
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
            if(g_fCurrTmp[i] < surrentstatusvalue.targetTmp/* &&( (currTmp[0]-lasttmp1)<5.0 || (currTmp[2]-lasttmp2)<5.0 || (currTmp[4]-lasttmp3)<5.0 || (currTmp[6]-lasttmp4)<5.0 &&(limit_count<4))*/)
            {
                port[i] = true;
                g_bPwmiMode = true;
                limit_count++;
                //printf("PWM increase %d  %f\n",originalInfo.targetTmp,temp[closegraph]);
            }
            else if(temp[closegraph] < surrentstatusvalue.targetTmp /*&& ((currTmp[0]-lasttmp1)>5.0 || (currTmp[2]-lasttmp2)>5.0 || (currTmp[4]-lasttmp3)>5.0 || (currTmp[6]-lasttmp4)>5.0)*/)
            {
                port[i] = true;
                g_bPwmdMode = true;
                //printf("%d  %f\n",originalInfo.targetTmp,temp[closegraph]);
            }
            else if(temp[closegraph] > surrentstatusvalue.targetTmp)
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
    pushButton_td1->setVisible(0);
    pushButton_td2->setVisible(0);
    pushButton_td3->setVisible(0);
    pushButton_td4->setVisible(0);
    ui->label_zlzq->setVisible(0);
    ui->label_devidestatus->setVisible(0);
    ui->label_impedance1->setVisible(0);
    ui->label_impedance2->setVisible(0);
    ui->label_impedance3->setVisible(0);
    ui->label_impedance4->setVisible(0);
    //ui->pushButton_addgas->setVisible(0);
    //ui->pushButton_losegas->setVisible(0);
    //ui->label_gasvalue->setVisible(0);
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
    pushButton_td1->setVisible(1);
    pushButton_td2->setVisible(1);
    pushButton_td3->setVisible(1);
    pushButton_td4->setVisible(1);
    ui->label_zlzq->setVisible(1);
    ui->label_devidestatus->setVisible(1);
    ui->label_impedance1->setVisible(1);
    ui->label_impedance2->setVisible(1);
    ui->label_impedance3->setVisible(1);
    ui->label_impedance4->setVisible(1);
    //ui->pushButton_addgas->setVisible(1);
    //ui->pushButton_losegas->setVisible(1);
    //ui->label_gasvalue->setVisible(1);
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

#if 1
void mdmedical::GetChanel1Value()
{
    if(!channel_mask[0])
    {
        pushButton_td1->setStyleSheet("background-color: white;color:black");
        channel_mask[0] = true;
    }
    else
    {
        pushButton_td1->setStyleSheet("background-color: black;color:white");
        channel_mask[0] = false;
    }
}
void mdmedical::GetChanel2Value()
{
    if(!channel_mask[1])
    {
        pushButton_td2->setStyleSheet("background-color: white;color:black");
        channel_mask[1] = true;
    }
    else
    {
        pushButton_td2->setStyleSheet("background-color: black;color:white");
        channel_mask[1] = false;
    }
}
void mdmedical::GetChanel3Value()
{
    if(!channel_mask[2])
    {
        pushButton_td3->setStyleSheet("background-color: white;color:black");
        channel_mask[2] = true;
    }
    else
    {
        pushButton_td3->setStyleSheet("background-color: black;color:white");
        channel_mask[2] = false;
    }
}
void mdmedical::GetChanel4Value()
{
    if(!channel_mask[3])
    {
        pushButton_td4->setStyleSheet("background-color: white;color:black");
        channel_mask[3] = true;
    }
    else
    {
        pushButton_td4->setStyleSheet("background-color: black;color:white");
        channel_mask[3] = false;
    }
}

#endif
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

    if(g_cWhichKey == 1)
    {
        g_cWhichKey = 0;
        if(g_bKeyValue)
        {
            g_bIdleMode = true;
            g_bPrerareMode = true;
            g_bPrepareKey = false;
            m_pQTimer_showimpedance->start(1000);
            m_pQTimer_preparestatus->start(1000);
        }
        else
        {
            ;
        }
    }
    else if(g_cWhichKey == 2)
    {
        g_cWhichKey = 0;
        if(!m_pQTimer_showgraph->isActive())
        {
            m_pQTimer_showgraph->start(500);
            m_pQTimer_showpowerrate1->start(1000);
            m_pQTimer_writedata->start(1000);
            ItemNewPosition();
            g_iIndexpower = surrentstatusvalue.maxPower;

            //surrentstatusvalue.cureCycle = m_QComboBox_cureperiod->currentIndex();
            m_minute = surrentstatusvalue.cureCycle / 6 + 1;
            m_second = surrentstatusvalue.cureCycle % 6 *10;

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
        else
        {
            g_bFootKeyHide = true;
            g_bFootKey = false;

            m_pQTimer_writedata->stop();
            wb.Dump(datafilename);
        }
    }
    else if(g_cWhichKey == 3)
    {
        g_cWhichKey = 0;
        if(g_bIsCreateAdcDetectpthread)
        {
            g_bIsCreateAdcDetectpthread = false;
            g_bisGatheringAdcValue = true;

            pthread_t tid = -1;
             int ret = pthread_create(&tid,NULL,deteAdcValve,NULL);
             if(ret == -1)
             {
                 printf("create deteAdcValve pthread failed!\n");
             }
             else
             {
                 pthread_detach(tid);
             }
        }
    }
    else if(g_cWhichKey == 4)
    {
        g_cWhichKey = 0;
        g_bisBreakGas = true;
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
        fclose(pfile);

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
        if(m_pQTimerShowUpdateFinished->isActive())
            m_pQTimerShowUpdateFinished->stop();
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
        if(m_pQTimerRecordTempture->isActive())
            m_pQTimerRecordTempture->stop();

        printf("create file finished!\n");
        wb.Dump("datafilename_tempture.xls");

    }
}



void mdmedical::FlushShowInfo()
{
    ui->label_zlzq->setText(mymapperiod.value(surrentstatusvalue.cureCycle));
    ui->label_tagetTmp->setText(mymaptemp.value(surrentstatusvalue.targetTmp));
    ui->label_Max->setText(mymappower.value(surrentstatusvalue.maxPower));
    ui->label_coldwaterstream->setText(mymapwater.value(surrentstatusvalue.curePos));
    if(g_isSetting)
    {
        if(m_pQTimerFlushShowInfo->isActive())
            m_pQTimerFlushShowInfo->stop();
        g_isSetting = false;
    }
}

void mdmedical::NextStep()
{
    this->promitwidget->setVisible(0);
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




void mdmedical::on_pushButton_setting_clicked()
{
    m_pQTimerFlushShowInfo->start(1000);
    mysetpage->showFullScreen();
    mysetpage->show();
}

void mdmedical::on_pushButton_systeminfo_clicked()
{
    systeminfo->setWindowTitle("系统信息");
    systeminfo->setModal(true);
    systeminfo->setWindowFlags(Qt::Window);
    systeminfo->showFullScreen();
}
