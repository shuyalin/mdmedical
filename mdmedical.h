#ifndef MDMEDICAL_H
#define MDMEDICAL_H

#include <QWidget>

#include <QTimer>
#include <QString>
#include <QDate>
#include <QPixmap>
#include <QBrush>
#include <QPalette>
#include <QDebug>
#include <QPalette>
#include <QMessageBox>
#include <QComboBox>
#include <QVector>
#include <QProcess>

#include <stdio.h>
#include <linux/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>


#include "qcustomplot.h"
#include "serial.h"

#define RES_POS "/mnt/mdmedical1/mdmedical/res/Background/"
//#define RES_POS "/mnt/mdmedical1/mdmedical/res/Background/"

#define SERIAL0 "/dev/ttyS0"
#define SERIAL1 "/dev/ttyS1"
#define SERIAL2 "/dev/ttyS2"
#define SERIAL3 "/dev/ttyS3"
#define SERIAL4 "/dev/ttyS4"
#define SERIAL5 "/dev/ttyS5"
#define SERIAL6 "/dev/ttyS6"
#define SERIAL7 "/dev/ttyS7"




typedef struct CCurrentInfo
{
     int cureCycle = 0;
     int targetTmp = 0;
     int maxPower = 0;
     int curePos = 0;
     int chanel = 1;

}CCUREINFORMATION;


namespace Ui {
class mdmedical;
}

class mdmedical : public QWidget
{
    Q_OBJECT

public:
    explicit mdmedical(QWidget *parent = 0);
    ~mdmedical();


    void InitCureReriodSubWidgets();
    void InitTargetTempSubWidgets();
    void InitMaxPowerSubWidgets();
    void InitCurePosSubWidgets();

private:
    QTimer *m_pQTimer_showtime;
    QTimer *m_pQTimer_showgraph;
    QTimer *m_pQTimer_showtmp;
    QTimer *m_pQTimer_showimpedance;
    QTimer *m_pQTimer_preparestatus;
    QTimer *m_pQTimer_detectkey;
    QTimer *m_pQTimer_detectcalibrate;

    QCustomPlot *widget;
    int closegraph;
    int setvalue;
    //CSerial selOperation;
    CCUREINFORMATION originalInfo;
//    CSerial sel1;
//    CSerial sel2;
//    CSerial sel4;
//    CSerial sel5;
    QComboBox  *m_QComboBox_cureperiod;
    QComboBox  *m_QComboBox_targettemp;
    QComboBox  *m_QComboBox_maxpower;
    QComboBox  *m_QComboBox_curepos;

    QLabel *m_QLabel_pic1;

    QProcess *m_QProcess_calibration;

    int m_plot_i;
    int m_calibate_count;
public slots:
    void ShowTimeCurrent(void);
    void makeGraph();
    void HideAllWidget();
    void HShowAllWidget();
    void ItemNewPosition();
    void OriginalPosition();
    void ShowCurrentTmp();
    void showCurrentImpedance();
    void ChangePrepareStatus();

private slots:
    void on_pushButton_footkey_clicked();
   void GetCureCycleCurrentValue();
   void GetTargetTmpCurrentValue();
   void GetMaxPowerCurrentValue();
   void GetCurePosCurrentValue();
   void GetChanel1Value();
   void GetChanel2Value();
   void GetChanel3Value();
   void GetChanel4Value();
   void DetectKey();
   void DetectCalibrate();





   void on_pushButton_waitmode_clicked();

   void on_pushButton_calibrate_clicked();

private:
    Ui::mdmedical *ui;
};

#endif // MDMEDICAL_H
