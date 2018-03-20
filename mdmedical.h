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

#define RES_POS "/mnt/mdmedical1/mdmedical/res/Background/"
//#define RES_POS "/mnt/mdmedical1/mdmedical/res/Background/"

namespace Ui {
class mdmedical;
}

class mdmedical : public QWidget
{
    Q_OBJECT

public:
    explicit mdmedical(QWidget *parent = 0);
    ~mdmedical();

private:
    QTimer *m_pQTimer_showtime;
    QTimer *m_pQTimer_showgraph;
    QTimer *m_pQTimer_showtmp;
    QCustomPlot *widget;
    int closegraph;
    int setvalue;
    //CSerial selOperation;
    //CCUREINFORMATION originalInfo;
public slots:
    void ShowTimeCurrent(void);
    void makeGraph();
    void HideAllWidget();
    void HShowAllWidget();
    void ItemNewPosition();
    void OriginalPosition();
    void ShowCurrentTmp();

private slots:
    void on_pushButton_footkey_clicked();
//    void GetCureCycleCurrentValue();
//    void GetTargetTmpCurrentValue();
//    void GetMaxPowerCurrentValue();
//    void GetCurePosCurrentValue();
//    void GetChanel1Value();
//    void GetChanel2Value();
//    void GetChanel3Value();
//    void GetChanel4Value();

private:
    Ui::mdmedical *ui;
};

#endif // MDMEDICAL_H
