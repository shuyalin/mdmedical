#ifndef MYSETTINT_H
#define MYSETTINT_H

#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QString>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QRadioButton>
#include <QProgressBar>
#include <QButtonGroup>
#include <QSlider>
#include <QHBoxLayout>
#include <QDebug>
#include <QLineEdit>
#include <QRegExpValidator>

class CSetting : public QWidget
{
    Q_OBJECT
public:
    explicit CSetting(QWidget *parent = 0);
    ~CSetting();

signals:

public slots:

private:
    QToolButton* m_pGeneralInfoBtn; /// 基本信息
    QToolButton* m_pParamSetBtn; /// 参数设置
    QToolButton* m_pNetSetBtn; /// 网络设置
    QToolButton* m_pUseSignBtn; /// 使用登记
    QToolButton* m_pAboutusBtn; /// 关于我们
    QToolButton* m_pRemoteControlBtn; /// 远程控制
    QToolButton* m_pRestartBtn; /// 重启
    QToolButton* m_pShutdownBtn; /// 关机
    QToolButton* m_pExitBtn; /// 退出，只是退出设置界面
    QStackedWidget* m_pMainStackWidget; /// 主堆栈窗体，存放几个详细设置界面


};


#endif // MYSETTINT_H
