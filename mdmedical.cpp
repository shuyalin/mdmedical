#include "mdmedical.h"
#include "ui_mdmedical.h"

float currtmp;

char val;
void *func(void *args)
{
    while(1)
    {
        scanf("%c",&val);
        printf("%c\n",val);
    }
}

float currTmp[8];
bool isTmpFinished = false;
bool isShowFinished = false;

//void *SendSerialFun(void *args)
//{
//    CSerial* tmp = (CSerial *)args;
//    tmp->ControlBuzzer(0x23,0x34,0x2);
//    tmp->RecvData();
//}

void *GetCurrTmp(void *args)
{
        unsigned short tmp,count = 0,min = 10000,max = 0;
        unsigned int sum_tmp = 0,i;

        int fd = open("/dev/mdmedical_tmp", O_RDWR);
        if(fd < 0)
        {
            printf("open device failed!\n");
        }
        usleep(2*1000);
        while(1)
        {
            for(i=0;i<8;i++)
            {
                while(count < 10)
                {
                    ioctl(fd,i+1,12);
                    usleep(2*1000);
                    read(fd,&tmp,2);
                    count++;
                    sum_tmp += tmp;
                    if(tmp > max)
                        max = tmp;
                    if(tmp < min)
                        min = tmp;
                }

                currTmp[i] = (float)((sum_tmp-min-max)*120.0/8/4096);
                count = 0;
                sum_tmp = 0;
                 tmp = 0;
                 //printf("the value is  %d    %.1f\n",i,currTmp[i]);
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
        connect(m_pQTimer_showtime, SIGNAL(timeout()), this, SLOT(ShowTimeCurrent()));
        connect(m_pQTimer_showgraph, SIGNAL(timeout()), this, SLOT(makeGraph()));
        connect(m_pQTimer_showtmp, SIGNAL(timeout()), this, SLOT(ShowCurrentTmp()));
        //connect(ui->spinBox_zlzq,SIGNAL(valueChanged(int)),this,SLOT(GetCureCycleCurrentValue()));
        //connect(ui->spinBox_targetTmp,SIGNAL(valueChanged(int)),this,SLOT(GetTargetTmpCurrentValue()));
        //connect(ui->spinBox_Max,SIGNAL(valueChanged(int)),this,SLOT(GetMaxPowerCurrentValue()));
        //connect(ui->spinBox_zlwz,SIGNAL(valueChanged(int)),this,SLOT(GetCurePosCurrentValue()));

        //connect(ui->pushButton_td1,SIGNAL(clicked()),this,SLOT(GetChanel1Value()));
        //connect(ui->pushButton_td2,SIGNAL(clicked()),this,SLOT(GetChanel2Value()));
        //connect(ui->pushButton_td3,SIGNAL(clicked()),this,SLOT(GetChanel3Value()));
        //connect(ui->pushButton_td4,SIGNAL(clicked()),this,SLOT(GetChanel4Value()));


        ui->label_showcurtime->setGeometry(1,1,400,50);
        ui->label_showcurtime->setText("");

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

        ui->label_tmp1->setText(QString::fromUtf8("15.6"));
        ui->label_tmp2->setText(QString::fromUtf8("15.6"));
        ui->label_tmp3->setText(QString::fromUtf8("15.6"));
        ui->label_tmp4->setText(QString::fromUtf8("15.6"));

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


        ui->label_impedance1->setText("9527");
        ui->label_impedance2->setText("9527");
        ui->label_impedance3->setText("9527");
        ui->label_impedance4->setText("9527");




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

        ui->pushButton_td1->setGeometry(680,255,40,20);
        ui->pushButton_td2->setGeometry(710,230,40,20);
        ui->pushButton_td3->setGeometry(740,255,40,20);
        ui->pushButton_td4->setGeometry(710,280,40,20);



        ui->label_tagetTmp->setGeometry(680,75,100,20);
        ui->label_tagetTmp->setText(QString::fromUtf8("目标温度"));

        ui->label_Max->setGeometry(680,140,100,20);
        ui->label_Max->setText(QString::fromUtf8("最大功率"));

        ui->spinBox_targetTmp->setGeometry(695,99,100,20);
        ui->spinBox_targetTmp->setValue(38);
        //int curvalue = ui->spinBox_targetTmp->value();
        //qDebug()<<curvalue<<endl;

        ui->spinBox_Max->setGeometry(695,164,100,20);
        ui->spinBox_Max->setValue(10);


        ui->label_tdxz->setGeometry(680,200,80,20);
        ui->label_tdxz->setText(QString::fromUtf8("通道选择"));

        ui->label_zlwz->setGeometry(680,320,100,20);
        ui->label_zlwz->setText(QString::fromUtf8("治疗位置(cm)"));

        ui->spinBox_zlwz->setGeometry(695,344,100,20);
        ui->spinBox_zlwz->setValue(12);

        ui->label_zlzq->setGeometry(680,10,100,20);
        ui->label_zlzq->setText(QString::fromUtf8("治疗周期"));

        ui->spinBox_zlzq->setGeometry(695,34,100,20);
        ui->spinBox_zlzq->setValue(2);

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

        m_pQTimer_showtime->start(1000);
        m_pQTimer_showtmp->start(1000);
        pthread_t tid;
        int ret;
        ret = pthread_create(&tid,NULL,GetCurrTmp,NULL);
}

mdmedical::~mdmedical()
{
    delete ui;
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
        ui->label_tmp2->setText(data2.mid(0,4));
        ui->label_tmp3->setText(data3.mid(0,4));
        ui->label_tmp4->setText(data4.mid(0,4));
        ui->label_tmp5->setText(data5.mid(0,4));
        ui->label_tmp6->setText(data6.mid(0,4));
        ui->label_tmp7->setText(data7.mid(0,4));
        ui->label_tmp8->setText(data8.mid(0,4));

        isShowFinished = true;
    }

}


void mdmedical::makeGraph()
{

    if((closegraph++) == 100)
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
    ui->spinBox_targetTmp->setVisible(0);
    ui->spinBox_Max->setVisible(0);
    ui->label_tdxz->setVisible(0);
    ui->label_zlwz->setVisible(0);
    ui->spinBox_zlwz->setVisible(0);
    ui->pushButton_td1->setVisible(0);
    ui->pushButton_td2->setVisible(0);
    ui->pushButton_td3->setVisible(0);
    ui->pushButton_td4->setVisible(0);
    ui->label_zlzq->setVisible(0);
    ui->spinBox_zlzq->setVisible(0);
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
    ui->spinBox_targetTmp->setVisible(1);
    ui->spinBox_Max->setVisible(1);
    ui->label_tdxz->setVisible(1);
    ui->label_zlwz->setVisible(1);
    ui->spinBox_zlwz->setVisible(1);
    ui->pushButton_td1->setVisible(1);
    ui->pushButton_td2->setVisible(1);
    ui->pushButton_td3->setVisible(1);
    ui->pushButton_td4->setVisible(1);
    ui->label_zlzq->setVisible(1);
    ui->spinBox_zlzq->setVisible(1);
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
    //HideAllWidget();
    pthread_t senddata_tid;
//    if(-1 == pthread_create(&senddata_tid,NULL,SendSerialFun,&selOperation))  //modify
//    {
//        printf("create pthread failed!\n");
//    }


   // qDebug()<<originalInfo.chanel<<"  "<<originalInfo.cureCycle<<"  "<<originalInfo.curePos<<"  "<<originalInfo.maxPower<<"  "<<originalInfo.targetTmp<<endl;
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

#if 0
void mdmedical::GetCureCycleCurrentValue()
{
    originalInfo.cureCycle = ui->spinBox_zlzq->value();
}

void mdmedical::GetTargetTmpCurrentValue()
{
    originalInfo.targetTmp = ui->spinBox_targetTmp->value();
}
void mdmedical::GetMaxPowerCurrentValue()
{
    originalInfo.maxPower = ui->spinBox_Max->value();
}
void mdmedical::GetCurePosCurrentValue()
{
    originalInfo.curePos = ui->spinBox_zlwz->value();
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
#endif
