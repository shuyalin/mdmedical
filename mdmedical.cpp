#include "mdmedical.h"
#include "ui_mdmedical.h"

float currtmp;

//char val;
//void *func(void *args)
//{
//    while(1)
//    {
//        scanf("%c",&val);
//        printf("%c\n",val);
//    }
//}

float currTmp[8];
bool isTmpFinished = false;
bool isShowFinished = false;

bool g_bWaitCommand = false;

unsigned short impedance[4];

CSerial sel1;
CSerial sel2;
CSerial sel4;
CSerial sel5;

bool g_bPrerareMode = false;
bool g_bIdleMode = false;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;/*初始化互斥锁*/
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;/*初始化条件变量*/

void *SendSerialFun(void *args)
{
    CSerial* tmp = (CSerial *)args;
    tmp->ControlBuzzer(0x23,0x34,0x2);
    tmp->RecvData();
}

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

void Prepare_send()
{
    sel1.PrepareSend(0);
    sel1.SendData();
    sel1.RecvData();
    sel1.ParsePrepareSendReturnData();
    impedance[0] = sel1.impence;
    //printf("the zukang is : %hu\n",sel1.impence);
    //printf("powerrate is  %d  %d  %d\n",sel1.powerrate[0],sel1.powerrate[1],sel1.powerrate[2]);

    sel2.PrepareSend(0);
    sel2.SendData();
    sel2.RecvData();
    sel2.ParsePrepareSendReturnData();
    impedance[1] = sel2.impence;
    //printf("the zukang is : %hu\n",sel2.impence);
    //printf("powerrate is  %d  %d  %d\n",sel2.powerrate[0],sel2.powerrate[1],sel2.powerrate[2]);

    sel4.PrepareSend(0);
    sel4.SendData();
    sel4.RecvData();
    sel4.ParsePrepareSendReturnData();
    impedance[2] = sel4.impence;
    //printf("the zukang is : %hu\n",sel4.impence);
    //printf("powerrate is  %d  %d  %d\n",sel4.powerrate[0],sel4.powerrate[1],sel4.powerrate[2]);

    sel5.PrepareSend(0);
    sel5.SendData();
    sel5.RecvData();
    sel5.ParsePrepareSendReturnData();
    impedance[3] = sel5.impence;
    //printf("the zukang is : %hu\n",sel5.impence);
    //printf("powerrate is  %d  %d  %d\n",sel5.powerrate[0],sel5.powerrate[1],sel5.powerrate[2]);
}

void *sel_send_func(void *args)
{
    sel1.openSerial(SERIAL1,115200);
    sel2.openSerial(SERIAL2,115200);
    sel4.openSerial(SERIAL4,115200);
    sel5.openSerial(SERIAL5,115200);
     pthread_cond_signal(&cond);
    while(1)
    {
        if(g_bPrerareMode && g_bWaitCommand)
        {
            Prepare_send();
            usleep(800*1000);
        }
        else if(g_bIdleMode)
        {
            g_bWaitCommand = Idle_send();
            usleep(1000*1000);
        }
        usleep(10*1000);
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
void *GetCurrTmp(void *args)
{
        unsigned short tmp,count = 0,min = 10000,max = 0;
        unsigned int sum_tmp = 0,i;
        printf("start read temp!\n");
        int fd = open("/dev/mdmedical_tmp", O_RDWR);
        if(fd < 0)
        {
            printf("open device failed!\n");
        }
        usleep(2*1000);
        while(1)
        {
            //isTmpFinished = false;
            for(i=3;i<11;i++)
            {
                ioctl(fd,i,12);
                usleep(2*1000);
                while(count < 10)
                {                                    
                    read(fd,&tmp,2);
                    count++;
                    sum_tmp += tmp;
                    if(tmp > max)
                        max = tmp;
                    if(tmp < min)
                        min = tmp;
                }

                currTmp[i-3] = (float)((sum_tmp-min-max)*120.0/8/4096);
                count = 0;
                sum_tmp = 0;
                 tmp = 0;
//                 if(i == 0 || i == 1)
//                        printf("the value is  %d    %.1f\n",i,currTmp[i]);
            }
            isTmpFinished = true;
            while(!isShowFinished)
                usleep(1*1000);
        }
}

mdmedical::mdmedical(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mdmedical)
{
    ui->setupUi(this);

    //    if(!selOperation.openSerial(115200))
    //        QMessageBox::information(this,"open serial ","error");
        widget = new QCustomPlot(this);
        widget->setGeometry(10,240,400,300);
        widget->setVisible(0);
        widget->addGraph();
        widget->addGraph();
        widget->addGraph();

        closegraph = 0;

        m_pQTimer_showtime = new QTimer(this);
        m_pQTimer_showgraph = new QTimer(this);
        m_pQTimer_showtmp = new QTimer(this);
        m_pQTimer_showimpedance = new QTimer(this);

         m_QComboBox_cureperiod = new QComboBox(this);
         InitCureReriodSubWidgets();
         m_QComboBox_targettemp = new QComboBox(this);
         InitTargetTempSubWidgets();
         m_QComboBox_maxpower = new QComboBox(this);
         InitMaxPowerSubWidgets();
         m_QComboBox_curepos = new QComboBox(this);
         InitCurePosSubWidgets();



        connect(m_pQTimer_showtime, SIGNAL(timeout()), this, SLOT(ShowTimeCurrent()));
        connect(m_pQTimer_showgraph, SIGNAL(timeout()), this, SLOT(makeGraph()));
        connect(m_pQTimer_showtmp, SIGNAL(timeout()), this, SLOT(ShowCurrentTmp()));
        connect(m_pQTimer_showimpedance, SIGNAL(timeout()), this, SLOT(showCurrentImpedance()));

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

        ui->label_tmp1->setGeometry(280,100,45,20);  //chanel   1
        ui->label_tmp2->setGeometry(454,278,45,20);  //chanel   1  nianmo
        ui->label_tmp3->setGeometry(280,460,45,20);
        ui->label_tmp4->setGeometry(100,267,45,20);

        ui->label_tmp5->setGeometry(280,163,45,20);
        ui->label_tmp6->setGeometry(400,267,45,20);
        ui->label_tmp7->setGeometry(280,400,45,20);
        ui->label_tmp8->setGeometry(155,278,45,20);

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

        ui->pushButton_preparemode->setGeometry(800,600,100,30);
        ui->pushButton_preparemode->setText(QString::fromUtf8("预备模式"));

        ui->pushButton_waitmode->setGeometry(800,650,100,30);
        ui->pushButton_waitmode->setText(QString::fromUtf8("待机模式"));

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



        ui->label_devidestatus->setGeometry(0,540,200,20);
        ui->label_devidestatus->setText(QString::fromUtf8("设备正常"));

        ui->pushButton_addgas->setGeometry(40,120,60,30);
        ui->pushButton_losegas->setGeometry(110,120,60,30);
        ui->label_gasvalue->setGeometry(80,80,40,20);

        ui->label_gasvalue->setText("150");
        ui->pushButton_addgas->setText(QString::fromUtf8("充气"));
        ui->pushButton_losegas->setText(QString::fromUtf8("放气"));

        ui->pushButton_footkey->setGeometry(450,360,70,30);
        ui->pushButton_footkey->setText(QString::fromUtf8("脚踏按键"));

        QPixmap pixmap(QString::fromUtf8(RES_POS"step2.png"));//当前文件夹下面的图片
        QPalette palette = this->palette();
        palette.setBrush(backgroundRole(),QBrush(pixmap.scaled(// 缩放背景图.
                                                               QSize(800,600),
                                                                      Qt::IgnoreAspectRatio,
                                                                      Qt::SmoothTransformation)));
        setPalette(palette);

//        sel1.openSerial(115200);
//        sel2.openSerial(115200);
//        sel4.openSerial(115200);
//        sel5.openSerial(115200);

        m_pQTimer_showtime->start(1000);

        m_pQTimer_showtmp->start(1000);

        pthread_t tid;
        int ret;
        ret = pthread_create(&tid,NULL,GetCurrTmp,NULL);

        pthread_t sel_send_tid;
        int ret1;
        ret1 = pthread_create(&sel_send_tid,NULL,sel_send_func,NULL);
/*
        pthread_t sel_recv_tid;
        int ret2;
        ret2 = pthread_create(&sel_recv_tid,NULL,sel_recv_func,NULL);
*/
}

mdmedical::~mdmedical()
{
    delete ui;
}


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

    m_QComboBox_cureperiod->setEditable(true);
    m_QComboBox_cureperiod->setMaxVisibleItems(10);
     m_QComboBox_cureperiod->insertItem(55, tr("Insert item"));
     m_QComboBox_cureperiod->insertSeparator(55);

}

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

    m_QComboBox_targettemp->setEditable(true);
    m_QComboBox_targettemp->setMaxVisibleItems(10);
     m_QComboBox_targettemp->insertItem(46, tr("Insert item"));
     m_QComboBox_targettemp->insertSeparator(46);
}

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

    m_QComboBox_maxpower->setEditable(true);
    m_QComboBox_maxpower->setMaxVisibleItems(10);
     m_QComboBox_maxpower->insertItem(21, tr("Insert item"));
     m_QComboBox_maxpower->insertSeparator(21);

}

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

    m_QComboBox_curepos->setEditable(true);
    m_QComboBox_curepos->setMaxVisibleItems(10);
     m_QComboBox_curepos->insertItem(81, tr("Insert item"));
     m_QComboBox_curepos->insertSeparator(81);



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

void mdmedical::ShowCurrentTmp(void)
{
    //isShowFinished = false;
    if(isTmpFinished)
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

        isShowFinished = true;
    }

}

void mdmedical::showCurrentImpedance(void)
{
    ui->label_impedance1->setText(QString::number(impedance[0],10));
    ui->label_impedance2->setText(QString::number(impedance[1],10));
    ui->label_impedance3->setText(QString::number(impedance[2],10));
    ui->label_impedance4->setText(QString::number(impedance[3],10));

}

void mdmedical::makeGraph()
{

    if((closegraph++) == originalInfo.cureCycle*60 || (originalInfo.targetTmp < currTmp[originalInfo.chanel-1]))
    {
        if(m_pQTimer_showtime->isActive())
            m_pQTimer_showtime->stop();

        closegraph = 0;
        //HShowAllWidget();
        OriginalPosition();
        widget->setVisible(0);
        ui->pushButton_footkey->setVisible(1);

        QPixmap pixmap(QString::fromUtf8(RES_POS"step2.png"));//当前文件夹下面的图片
        QPalette palette = this->palette();
        palette.setBrush(backgroundRole(),QBrush(pixmap.scaled(// 缩放背景图.
                                                               QSize(800,600),
                                                                      Qt::IgnoreAspectRatio,
                                                                      Qt::SmoothTransformation)));
        setPalette(palette);
    }
    widget->replot();



    QVector<double> x(200),y0(200),y1(200),y2(200);
    for(int i=0;i<200;i++)
    {
        if(i == 0)
        {
            x[i]=i;
            y0[i]=rand()%3+35;
            y1[i]=50;
            y2[i]=setvalue;

        }
        else
        {
            if(i%2==0)
            {
                x[i]=i;
                y0[i]=rand()%2+y0[i-1];
                y1[i]=50;
                y2[i]=setvalue;
            }
            else
            {
                x[i]=i;
                y0[i]=y0[i-1]-rand()%2;
                y1[i]=50;
                y2[i]=setvalue;
            }
        }
    }

    //创建QCustomPlot,添加曲线graph,并设置曲线的数据
    //QCustomPlot *customPlot=new QCustomPlot;

    widget->xAxis->setRange(0,200);
    widget->yAxis->setRange(30,80);

    widget->graph(0)->setName("fist line");
    widget->graph(0)->setData(x,y0);

    widget->graph(1)->setName("second line");
    widget->graph(1)->setData(x,y1);
    widget->graph(1)->setPen(QPen(QColor(255,0,0)));

    widget->graph(2)->setName("third line");
    widget->graph(2)->setData(x,y2);
    widget->graph(2)->setPen(QPen(QColor(255,0,0)));

    widget->graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20))); //设置曲线下方颜色

    widget->xAxis->setSubTickCount(9);
    //widget->xAxis->setTicks(false); //刻度是否可见
    //widget->xAxis->setTickStep(100);



    widget->xAxis->setLabel(QString::fromUtf8("时间"));
    widget->yAxis->setLabel(QString::fromUtf8("温度"));


    //设置坐标背景色
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    widget->axisRect()->setBackground(axisRectGradient);




    //设置画布背景色
//     QLinearGradient plotGradient;
//     plotGradient.setStart(0, 0);
//     plotGradient.setFinalStop(0, 350);
//     plotGradient.setColorAt(0, QColor(80, 80, 80));
//     plotGradient.setColorAt(1, QColor(50, 50, 50));
//     widget->setBackground(plotGradient);




    //显示铭文
    widget->legend->setVisible(true);

    //设置customPlot可进行拽托或伸缩
    widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    widget->axisRect()->setRangeDrag(Qt::Horizontal);  //设置允许在某一方向上拽托
    widget->axisRect()->setRangeZoom(Qt::Horizontal);  //设置允许在某一方向上伸缩
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

void mdmedical::on_pushButton_footkey_clicked()
{
    qDebug()<<originalInfo.chanel<<"  "<<originalInfo.cureCycle<<"  "<<originalInfo.curePos<<"  "<<originalInfo.maxPower<<"  "<<originalInfo.targetTmp<<endl;
    ItemNewPosition();

    m_pQTimer_showgraph->start(100);
    widget->setVisible(1);
    ui->pushButton_footkey->setVisible(0);
    QPixmap pixmap(QString::fromUtf8(RES_POS"step3.png"));//当前文件夹下面的图片
    QPalette palette = this->palette();
    palette.setBrush(backgroundRole(),QBrush(pixmap.scaled(// 缩放背景图.
                                                           QSize(800,600),
                                                                  Qt::IgnoreAspectRatio,
                                                                  Qt::SmoothTransformation)));
    setPalette(palette);
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

void mdmedical::GetCureCycleCurrentValue()
{
    originalInfo.cureCycle = m_QComboBox_cureperiod->currentIndex();
    qDebug()<<"originalInfo.cureCycle is "<<originalInfo.cureCycle;
}

void mdmedical::GetTargetTmpCurrentValue()
{
    originalInfo.targetTmp = m_QComboBox_targettemp->currentIndex();
}
void mdmedical::GetMaxPowerCurrentValue()
{
    originalInfo.maxPower = m_QComboBox_maxpower->currentIndex();
}
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


void mdmedical::on_pushButton_preparemode_clicked()
{
    //g_bPrerareMode = true;
    g_bIdleMode = true;
    m_pQTimer_showimpedance->start(1000);

}

void mdmedical::on_pushButton_waitmode_clicked()
{
    g_bPrerareMode = false;
    if(m_pQTimer_showimpedance->isActive())
    {
        m_pQTimer_showimpedance->stop();
    }

    ui->label_impedance1->setText("");
    ui->label_impedance2->setText("");
    ui->label_impedance3->setText("");
    ui->label_impedance4->setText("");

}
